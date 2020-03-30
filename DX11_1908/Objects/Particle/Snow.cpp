#include "Framework.h"
#include "Snow.h"

Snow::Snow()
	: Particle(L"Textures/Landscape/Snow.png", 500)
{
	shader = Shader::Add(L"Snow");
	shader->CreateGeometryShader();

	Create();

	blendState[1]->Alpha(true);
}

Snow::~Snow()
{
	delete snowBuffer;
}

void Snow::Update()
{
	snowBuffer->data.time += DELTA;
}

void Snow::Render()
{
	snowBuffer->SetVSBuffer(10);

	Particle::Render();

	ImGui::SliderFloat3("Velocity", (float*)&snowBuffer->data.velocity, -50, 50);
	ImGui::ColorEdit4("Color", (float*)&snowBuffer->data.color);
	ImGui::SliderFloat("Distance", &snowBuffer->data.drawDistance, 0, 2000);
	ImGui::SliderFloat3("Origin", (float*)&snowBuffer->data.origin, -50, 50);
	ImGui::SliderFloat("Time", &snowBuffer->data.time, 0, 2000);
}

void Snow::Create()
{
	vertices.resize(particleCount);

	Float3 size = Float3(10, 10, 10);

	for (UINT i = 0; i < particleCount; i++)
	{
		float scale;
		scale = Random(0.1f, 0.4f);		

		Float3 position;
		position.x = Random(-size.x, size.x);
		position.y = Random(-size.y, size.y);
		position.z = Random(-size.z, size.z);

		Float2 random;
		random.x = Random(0.0f, 1.0f);
		random.y = Random(0.0f, 1.0f);

		vertices[i].position = position;
		vertices[i].scale = scale;
		vertices[i].random = random;
	}

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexSnow), particleCount);

	snowBuffer = new SnowBuffer();
	snowBuffer->data.extent = size;
}
