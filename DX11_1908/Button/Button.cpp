#include "Framework.h"

Button::Button(Render2D* texture, bool start)
	: texture(texture), smallScale(0.8f), intValue(0), voidValue(nullptr),
	event(nullptr), intEvent(nullptr), voidEvent(nullptr),
	state(NONE), color(1, 1, 1, 1), check(start), select(false)
{
	shader = Shader::Add(L"Render2D");

	Create();

	if (start)
	{
		min = Vector3(texture->scale.GetX() * 0.5f, texture->scale.GetY() * 0.5f, 0);
		max = Vector3(texture->scale.GetX() * 1.5f, texture->scale.GetY() * 1.5f, 0);
	}
	else
	{
		min = Vector3(texture->scale.GetX() * 0.5f, texture->scale.GetY() * 0.5f, 0);
		max = Vector3(texture->scale.GetX() * 1.5f, texture->scale.GetY() * 1.5f, 0);
	}

	coll = new BoxCollider(min, max);

	if (start)
	{
		coll->position.SetX(Device::Get()->GetWidth() * 0.14f);
		coll->position.SetY(Device::Get()->GetWidth() * 0.21f);
	}
	else
	{
		coll->position.SetX(Device::Get()->GetWidth() * 0.18f);
		coll->position.SetY(Device::Get()->GetWidth() * 0.082f);
	}
}

Button::~Button()
{
	delete coll;
}

void Button::Update()
{
	render2D->Update();

	Float2 Mposition = Float2(M_position.x, M_position.y);

	if (coll->IsCollision(Mposition, check))
	{
		select = true;
		if (check)
		{
			render2D->scale.SetX(140);
			render2D->scale.SetY(50);

			render2D->position.SetX(Device::Get()->GetWidth() * 0.25f);
			render2D->position.SetY(Device::Get()->GetWidth() * 0.25f);
		}
		else
		{
			render2D->scale.SetX(90);
			render2D->scale.SetY(20);

			render2D->position.SetX(Device::Get()->GetWidth() * 0.25f);
			render2D->position.SetY(Device::Get()->GetWidth() * 0.1f);
		}

		if (KEYDOWN(VK_LBUTTON))
			state = DOWN;

		if (KEYUP(VK_LBUTTON))
		{
			if (state == DOWN)
			{
				if (event != nullptr)
					event();
				if (intEvent != nullptr)
					intEvent;
				if (voidEvent != nullptr)
					voidEvent;

				return;
			}

			state = OVER;
		}

		if (state == DOWN)
		{
			//texture->scale = Vector3(texture->scale.GetX() * smallScale, texture->scale.GetY() * smallScale, 1);
		}
		//else
			//texture->scale = Vector3(140, 40, 1);
	}
	else
	{
		//texture->scale = Vector3(140, 40, 1);
		select = false;
		if (KEYUP(VK_LBUTTON))
			state = NONE;
	}

	coll->UpdateWorld();
	UpdateWorld();
}

void Button::Render()
{
	coll->Render();

	if (select)
		render2D->Render();
}

void Button::Create()
{
	render2D = new Render2D();
	Texture* texture = Texture::Add(L"Textures/Selected.png");

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