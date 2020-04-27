#include "Framework.h"

EndButton::EndButton()
{
	Create();

	button = new Button(render2D, false);
}

EndButton::~EndButton()
{
	delete render2D;
	delete button;
}

void EndButton::Update()
{
	//button->SetParent(render2D->GetWorldPointer());
	button->SetCheck(false);
	render2D->Update();
	button->Update();
}

void EndButton::Render()
{
	button->Render();
	render2D->Render();
}

void EndButton::Create()
{
	render2D = new Render2D();
	Texture* texture = Texture::Add(L"Textures/Exit.jpg");

	render2D->scale.SetX(90);
	render2D->scale.SetY(20);

	render2D->position.SetX(Device::Get()->GetWidth() * 0.25f);
	render2D->position.SetY(Device::Get()->GetWidth() * 0.1f);

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
