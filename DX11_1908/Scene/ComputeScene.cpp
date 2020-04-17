#include "Framework.h"
#include "ComputeScene.h"

ComputeScene::ComputeScene()
{
	//Raw();
	//Intersection();
	Texture2D();	
}

ComputeScene::~ComputeScene()
{
	delete render2D;
}

void ComputeScene::Update()
{
	render2D->Update();
}

void ComputeScene::PreRender()
{
	
}

void ComputeScene::Render()
{
	
}

void ComputeScene::PostRender()
{
	render2D->Render();
}

void ComputeScene::Raw()
{
	ComputeShader* shader = new ComputeShader(L"ByteAddress");

	struct Output
	{
		UINT groupID[3];
		UINT groupThreadID[3];
		UINT dispatchThreadID[3];
		UINT groupIndex;
	};

	UINT size = 10 * 8 * 3 * 2;

	Output* output = new Output[size];
	RawBuffer* buffer = new RawBuffer(nullptr, sizeof(Output) * size);

	ID3D11UnorderedAccessView* uav = buffer->UAV();

	shader->Set();

	DC->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

	DC->Dispatch(2, 1, 1);

	buffer->Copy(output, sizeof(Output)* size);

	FILE* file;
	fopen_s(&file, "../Cs.csv", "w");

	for (UINT i = 0; i < size; i++)
	{
		Output temp = output[i];

		fprintf
		(
			file,
			"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
			i,
			temp.groupID[0], temp.groupID[1], temp.groupID[2],
			temp.groupThreadID[0], temp.groupThreadID[1], temp.groupThreadID[2],
			temp.dispatchThreadID[0], temp.dispatchThreadID[1], temp.dispatchThreadID[2],
			temp.groupIndex
		);
	}

	fclose(file);
}

void ComputeScene::Intersection()
{
	ComputeShader* shader = new ComputeShader(L"Intersection");

	struct InputDesc
	{
		UINT index;
		Float3 v0, v1, v2;
	};

	UINT width = 16;
	UINT height = 16;

	UINT index = 0;
	Float3* vertices = new Float3[width * height];

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			index = z * width + x;
			vertices[index] = Float3((float)x, 0.0f, (float)z);
		}
	}

	index = 0;
	UINT indexCount = (width - 1) * (height - 1) * 6;
	UINT* indices = new UINT[indexCount];

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices[index + 0] = width * z + x;
			indices[index + 1] = width * (z + 1) + x;
			indices[index + 2] = width * z + x + 1;

			indices[index + 3] = width * z + x + 1;
			indices[index + 4] = width * (z + 1) + x;
			indices[index + 5] = width * (z + 1) + x + 1;

			index += 6;
		}
	}

	InputDesc* input = new InputDesc[indexCount / 3];
	for (UINT i = 0; i < indexCount / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		input[i].v0 = vertices[index0];
		input[i].v1 = vertices[index1];
		input[i].v2 = vertices[index2];

		input[i].index = i;
	}

	////////////////////////////////////////////////////

	struct OutputDesc
	{
		UINT picked;
		float u;
		float v;
		float distance;
	};

	UINT size = 1 * (indexCount / 3);

	StructuredBuffer* buffer = new StructuredBuffer(
		input, sizeof(InputDesc), size, sizeof(OutputDesc), size);

	class RayBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float3 position;
			float padding;

			Float3 direction;
			float padding2;
		}data;

		RayBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.position = Float3(3.0f, 5.0f, 1.0f);
			data.direction = Float3(0, -1, 0);
		}
	}rayBuffer;	

	rayBuffer.SetCSBuffer(0);

	shader->Set();

	ID3D11ShaderResourceView* srv = buffer->SRV();
	DC->CSSetShaderResources(0, 1, &srv);

	ID3D11UnorderedAccessView* uav = buffer->UAV();
	DC->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

	DC->Dispatch(1, 1, 1);

	OutputDesc* output = new OutputDesc[size];
	buffer->Copy(output, sizeof(OutputDesc)* size);

	FILE* file;
	fopen_s(&file, "../Picked.csv", "w");

	for (UINT i = 0; i < size; i++)
	{
		OutputDesc temp = output[i];

		fprintf
		(
			file,
			"%d,%.0f,%.0f,%.0f\n",
			temp.picked,
			temp.u, temp.v, temp.distance
		);
	}

	fclose(file);
}

void ComputeScene::Texture2D()
{
	render2D = new Render2D();
	Texture* texture = Texture::Add(L"Textures/test.jpg");	

	render2D->scale.SetX(Device::Get()->GetWidth());
	render2D->scale.SetY(Device::Get()->GetHeight());

	render2D->position.SetX(Device::Get()->GetWidth() * 0.5f);
	render2D->position.SetY(Device::Get()->GetHeight() * 0.5f);

	ComputeShader* shader = new ComputeShader(L"Texture2D");
	shader->Set();

	ID3D11Texture2D* src;
	texture->GetSRV()->GetResource((ID3D11Resource**)&src);

	TextureBuffer* buffer = new TextureBuffer(src);

	DC->CSSetShaderResources(0, 1, &buffer->SRV());
	DC->CSSetUnorderedAccessViews(0, 1, &buffer->UAV(), nullptr);

	UINT width = buffer->Width();
	UINT height = buffer->Height();
	UINT page = buffer->Page();

	float x = ((float)width / 32.0f) < 1.0f ? 1.0f : ((float)width / 32.0f);
	float y = ((float)height / 32.0f) < 1.0f ? 1.0f : ((float)height / 32.0f);	

	DC->Dispatch((UINT)ceil(x), (UINT)ceil(y), page);

	ID3D11UnorderedAccessView* emptyUAV = nullptr;
	DC->CSSetUnorderedAccessViews(0, 1, &emptyUAV, nullptr);

	render2D->SetSRV(buffer->OutputSrv());
}

