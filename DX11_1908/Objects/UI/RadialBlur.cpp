#include "Framework.h"
#include "RadialBlur.h"

RadialBlur::RadialBlur()
	: Render2D(L"RadialBlur")
{
	buffer = new BlurBuffer();
}

RadialBlur::~RadialBlur()
{
	delete buffer;
}

void RadialBlur::Render()
{
	buffer->SetPSBuffer(10);

	Render2D::Render();

	ImGui::Text("RadialBlur Option");
	ImGui::SliderInt("Count", &buffer->data.count, 1, 50);
	ImGui::SliderFloat("Radius", &buffer->data.radius, 0, 30.0f);
	ImGui::SliderFloat("Amount", &buffer->data.amount, 0, 30.0f);
}
