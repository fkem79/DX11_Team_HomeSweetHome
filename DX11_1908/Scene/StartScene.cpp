#include "Framework.h"
#include "StartScene.h"

StartScene::StartScene()
{
}

StartScene::~StartScene()
{
}

void StartScene::Update()
{
	render2D->Update();
}

void StartScene::PreRender()
{
}

void StartScene::Render()
{
	render2D->Render();
}

void StartScene::PostRender()
{
	if (ImGui::Button("Start"))
	{
		Start();
	}
}

void StartScene::Begin()
{
	Texture2D();

	SOUND->Add("StartScene_bgm", "Sounds/A_PretaChasing01.OGG", true);

	SOUND->Play("StartScene_bgm");
}

void StartScene::End()
{
	delete render2D;

	SOUND->Stop("StartScene_bgm");
}

void StartScene::Start()
{
	SCENE->ChangeScene("play");
}

void StartScene::Exit()
{

}

void StartScene::Texture2D()
{
	render2D = new Render2D();
	Texture* texture = Texture::Add(L"Textures/start.jpg");

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