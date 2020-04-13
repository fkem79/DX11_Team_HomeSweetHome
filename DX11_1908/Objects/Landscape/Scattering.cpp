#include "Framework.h"

Scattering::Scattering()
	: theta(0), phi(0), radius(10), slices(32), stacks(16)
{
	targets[0] = new RenderTarget();
	targets[1] = new RenderTarget();

	depthStencil = new DepthStencil();

	rayleigh2D = new Render2D();
	rayleigh2D->position = Vector3(100, 150, 0);
	rayleigh2D->scale = Vector3(200, 100, 0);
	rayleigh2D->SetSRV(targets[0]->GetSRV());

	mie2D = new Render2D();
	mie2D->position = Vector3(100, 50, 0);
	mie2D->scale = Vector3(200, 100, 0);
	mie2D->SetSRV(targets[1]->GetSRV());

	scatterShader = Shader::Add(L"Scattering");
	targetShader = Shader::Add(L"ScatteringTarget");

	worldBuffer = new WorldBuffer();
	targetBuffer = new TargetBuffer();
	scatterBuffer = new FloatBuffer();

	CreateSphere();
	CreateQuad();

	starField = Texture::Add(L"Textures/Landscape/NightSky.png");

	depthMode[0] = new DepthStencilState();
	depthMode[1] = new DepthStencilState();
	depthMode[1]->DepthEnable(false);
}

Scattering::~Scattering()
{
	delete depthMode[0];
	delete depthMode[1];

	delete worldBuffer;
	delete targetBuffer;
	delete scatterBuffer;

	delete[] quadVertices;
	delete quadBuffer;

	delete vertexBuffer;
	delete indexBuffer;

	delete targets[0];
	delete targets[1];

	delete depthStencil;

	delete rayleigh2D;
	delete mie2D;
}

void Scattering::Update()
{	
	Float3 camPos = CAMERA->position;
	Matrix world = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);

	worldBuffer->SetWorld(world);

	rayleigh2D->Update();
	mie2D->Update();
}

void Scattering::PreRender()
{
	//targets[0]->Set(depthStencil);
	//targets[1]->Set(depthStencil);
	RenderTarget::Sets(targets, 2, depthStencil);

	quadBuffer->Set();
	IASetPT();

	targetBuffer->SetPSBuffer(11);
	targetShader->Set();

	DC->Draw(6, 0);
}

void Scattering::Render()
{
	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	worldBuffer->SetVSBuffer(1);

	DC->PSSetShaderResources(10, 1, &targets[0]->GetSRV());
	DC->PSSetShaderResources(11, 1, &targets[1]->GetSRV());
	starField->Set(12);

	scatterBuffer->data.indices[0] = LIGHT->data.direction.y;
	scatterBuffer->SetPSBuffer(10);

	scatterShader->Set();

	depthMode[1]->SetState();
	DC->DrawIndexed(indexCount, 0, 0);
	depthMode[0]->SetState();
}

void Scattering::PostRender()
{
	rayleigh2D->Render();
	mie2D->Render();
}

void Scattering::CreateSphere()
{
	UINT domeCount = slices;
	UINT latitude = stacks;
	UINT longitude = domeCount;

	vertexCount = longitude * latitude * 2;
	indexCount = (longitude - 1) * (latitude - 1) * 2 * 8;

	VertexUV* vertices = new VertexUV[vertexCount];

	UINT index = 0;
	for (UINT i = 0; i < longitude; i++)
	{
		float xz = 100.0f * (i / (longitude - 1.0f)) * XM_PI / 180.0f;

		for (UINT j = 0; j < latitude; j++)
		{
			float y = XM_PI * j / (latitude - 1);

			vertices[index].position.x = sinf(xz) * cosf(y);
			vertices[index].position.y = cosf(xz);
			vertices[index].position.z = sinf(xz) * sinf(y);
			Vector3 temp = vertices[index].position;
			temp *= radius;
			vertices[index].position = temp;

			vertices[index].uv.x = 0.5f / (float)longitude + i / (float)longitude;
			vertices[index].uv.y = 0.5f / (float)latitude + j / (float)latitude;

			index++;
		}
	}

	for (UINT i = 0; i < longitude; i++)
	{
		float xz = 100.0f * (i / (longitude - 1.0f)) * XM_PI / 180.0f;

		for (UINT j = 0; j < latitude; j++)
		{
			float y = (XM_PI * 2.0f) - (XM_PI * j / (latitude - 1));

			vertices[index].position.x = sinf(xz) * cosf(y);
			vertices[index].position.y = cosf(xz);
			vertices[index].position.z = sinf(xz) * sinf(y);
			Vector3 temp = vertices[index].position;
			temp *= radius;
			vertices[index].position = temp;

			vertices[index].uv.x = 0.5f / (float)longitude + i / (float)longitude;
			vertices[index].uv.y = 0.5f / (float)latitude + j / (float)latitude;

			index++;
		}
	}

	index = 0;
	UINT* indices = new UINT[indexCount * 3];

	for (UINT i = 0; i < longitude - 1; i++)
	{
		for (UINT j = 0; j < latitude - 1; j++)
		{
			indices[index++] = i * latitude + j;
			indices[index++] = (i + 1) * latitude + j;
			indices[index++] = (i + 1) * latitude + (j + 1);

			indices[index++] = (i + 1) * latitude + (j + 1);
			indices[index++] = i * latitude + (j + 1);
			indices[index++] = i * latitude + j;
		}
	}

	UINT offset = latitude * longitude;
	for (UINT i = 0; i < longitude - 1; i++)
	{
		for (UINT j = 0; j < latitude - 1; j++)
		{
			indices[index++] = offset + i * latitude + j;
			indices[index++] = offset + (i + 1) * latitude + (j + 1);
			indices[index++] = offset + (i + 1) * latitude + j;

			indices[index++] = offset + i * latitude + (j + 1);
			indices[index++] = offset + (i + 1) * latitude + (j + 1);
			indices[index++] = offset + i * latitude + j;
		}
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexUV), vertexCount);
	indexBuffer = new IndexBuffer(indices, indexCount);

	delete[] vertices;
	delete[] indices;
}

void Scattering::CreateQuad()
{
	quadVertices = new VertexUV[6];

	quadVertices[0].position = Float3(-1, -1, 0);
	quadVertices[1].position = Float3(-1, 1, 0);
	quadVertices[2].position = Float3(1, -1, 0);
	quadVertices[3].position = Float3(1, -1, 0);
	quadVertices[4].position = Float3(-1, 1, 0);
	quadVertices[5].position = Float3(1, 1, 0);

	quadVertices[0].uv = Float2(0, 1);
	quadVertices[1].uv = Float2(0, 0);
	quadVertices[2].uv = Float2(1, 1);
	quadVertices[3].uv = Float2(1, 1);
	quadVertices[4].uv = Float2(0, 0);
	quadVertices[5].uv = Float2(1, 0);

	quadBuffer = new VertexBuffer(quadVertices, sizeof(VertexUV), 6);
}
