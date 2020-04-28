#include "Framework.h"
#include "MapScene.h"

MapScene::MapScene()
{
	player = new Player();
	player->GetPlayerHead()->GetTransform()->position = { 0, 5, 0 };

	belle = new Belle();
	belle->GetTransform()->position = { 30.0f, 0.0f, 50.0f };


	ocm = new ObjectCreateManager();
}

MapScene::~MapScene()
{
	delete player;
	delete belle;

	delete ocm;
}

void MapScene::Update()
{
	player->Update();
	belle->Update();

	ocm->Update();
}

void MapScene::PreRender()
{
}

void MapScene::Render()
{
	player->Render();
	belle->Render();
	
	ocm->Render();
}

void MapScene::PostRender()
{	
	ImGui::Separator();

	ImGui::BeginChildFrame(1, ImVec2(400, 100));
	ImGui::BeginChild("g1", ImVec2(400, 100), false);

	player->PostRender();
	ImGui::Separator();
	ImGui::Text("Belle Pos X %f, Y %f, Z %f", belle->GetTransform()->position.GetX(),
		belle->GetTransform()->position.GetY(), belle->GetTransform()->position.GetZ());

	ImGui::EndChild();
	ImGui::EndChildFrame();

	ImGui::InputFloat3("Tr1", belle->GetTransform()->position.data.m128_f32, 3);
	ImGui::InputFloat3("Rt1", belle->GetTransform()->rotation.data.m128_f32, 3);
	ImGui::InputFloat3("Sc1", belle->GetTransform()->scale.data.m128_f32, 3);

	ocm->PostRender();
}

