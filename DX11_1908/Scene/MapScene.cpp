#include "Framework.h"
#include "MapScene.h"

MapScene::MapScene()
{
	player = new Player();
	player->GetPlayerHead()->GetTransform()->position = { 0, 5, 0 };

	belle = new Belle();
	belle->GetTransform()->position = { 30.0f, 0.0f, 50.0f };

	tile1 = new ModelSingle("tile_02");
	tile1->position = { 0,1,0 };
	tile1->rotation = { 1.57, 0, 0 };
	tile1->scale *= 0.5f;
	tile1->SetBoxRenderCheck(false);

	tile2 = new ModelSingle("ceiling_01");
	tile2->position = { 0,35,0 };
	tile2->rotation = { 1.57, 0, 0 };
	tile2->scale *= 0.2f;
	tile2->SetBoxRenderCheck(false);

	ocm = new ObjectCreateManager();
}

MapScene::~MapScene()
{
	delete player;
	delete belle;

	delete tile1;
	delete tile2;
	delete ocm;
}

void MapScene::Update()
{
	player->Update();
	belle->Update();

	tile1->Update();
	tile2->Update();
	ocm->Update();
}

void MapScene::PreRender()
{
}

void MapScene::Render()
{
	belle->Render();

	player->Render();
	
	tile1->Render();
	tile2->Render();

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

	ImGui::InputFloat3("Tr1", belle->GetTransform()->position.data.m128_f32, 3);
	ImGui::InputFloat3("Rt1", belle->GetTransform()->rotation.data.m128_f32, 3);
	ImGui::InputFloat3("Sc1", belle->GetTransform()->scale.data.m128_f32, 3);

	ocm->PostRender();
}

