#include "Framework.h"
#include "MapScene.h"

MapScene::MapScene()
{
	player = new Player();

	belle = new Belle();
	belle->GetTransform()->position = { 30.0f, 0.0f, 50.0f };

	tile1 = new ModelSingle("tile_big01");
	tile1->position = { 0,1,0 };
	tile1->rotation = { 1.6, 0, 0 };
	tile1->SetBoxRenderCheck(false);

	ocm = new ObjectCreateManager();
}

MapScene::~MapScene()
{
	delete player;
	delete belle;

	delete tile1;
	delete ocm;
}

void MapScene::Update()
{
	player->Update();
	belle->Update();

	tile1->Update();
	ocm->Update();
}

void MapScene::PreRender()
{
}

void MapScene::Render()
{
	player->Render();
	belle->Render();
	
	tile1->Render();
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

	ImGui::InputFloat3("Tr", tile1->position.data.m128_f32, 3);
	ImGui::InputFloat3("Rt", tile1->rotation.data.m128_f32, 3);
	ImGui::InputFloat3("Sc", tile1->scale.data.m128_f32, 3);

	ocm->PostRender();
}

