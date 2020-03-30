#include "Framework.h"
#include "Rain.h"

Rain::Rain()
	: Particle(L"Textures/Landscape/Rain.png", 500)
{
	shader = Shader::Add(L"Rain");
	shader->CreateGeometryShader();

	Create();
}

Rain::~Rain()
{
	delete rainBuffer;
}

void Rain::Update()
{
	rainBuffer->data.time += DELTA;
}

void Rain::Render()
{
	rainBuffer->SetVSBuffer(10);
	Particle::Render();

	ImGui::SliderFloat3("Velocity", (float*)&rainBuffer->data.velocity, -50, 50);
	ImGui::ColorEdit4("Color", (float*)&rainBuffer->data.color);
	ImGui::SliderFloat("Distance", &rainBuffer->data.distance, 0, 2000);
	ImGui::SliderFloat3("Origin", (float*)&rainBuffer->data.origin, -50, 50);
}

void Rain::Create()
{
	vertices.resize(particleCount);

	Float3 size = Float3(10, 10, 10);

	for (UINT i = 0; i < particleCount; i++)
	{
		Float2 scale;
		scale.x = Random(0.1f, 0.4f);
		scale.y = Random(4.0f, 8.0f);

		Float3 position;
		position.x = Random(-size.x, size.x);
		position.y = Random(-size.y, size.y);
		position.z = Random(-size.z, size.z);

		vertices[i].position = position;
		vertices[i].size = scale;
	}

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexSize), particleCount);

	rainBuffer = new RainBuffer();
	rainBuffer->data.size = size;	
}
