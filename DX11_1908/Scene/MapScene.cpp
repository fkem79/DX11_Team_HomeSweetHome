#include "Framework.h"
#include "MapScene.h"

MapScene::MapScene()
{
	player = new Player();

	belle = new Belle();
	belle->GetTransform()->position = { 30.0f, 0.0f, 50.0f };
	
}

MapScene::~MapScene()
{
	delete player;
	delete belle;
	
}

void MapScene::Update()
{
	player->Update();
	belle->Update();
}

void MapScene::PreRender()
{
}

void MapScene::Render()
{
	player->Render();
	belle->Render();
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
}
