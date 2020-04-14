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
}

MapScene::~MapScene()
{
	delete player;
	delete belle;
	
	delete tile1;
}

void MapScene::Update()
{
	player->Update();
	belle->Update();

	tile1->Update();
}

void MapScene::PreRender()
{
}

void MapScene::Render()
{
	player->Render();
	belle->Render();
	tile1->Render();
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
	
	// º¸·ù...
	//GizmoTest(); 
}

void MapScene::GizmoTest()
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	if (KEYPRESS('1'))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (KEYPRESS('2'))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (KEYPRESS('3'))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	
	// Belle·Î test
	ImGuizmo::DecomposeMatrixToComponents(belle->GetTransform()->GetWorld().r[0].m128_f32,
		belle->GetTransform()->position.data.m128_f32, 
		belle->GetTransform()->rotation.data.m128_f32, 
		belle->GetTransform()->scale.data.m128_f32);
	ImGui::InputFloat3("Tr", belle->GetTransform()->position.data.m128_f32, 3);
	ImGui::InputFloat3("Rt", belle->GetTransform()->rotation.data.m128_f32, 3);
	ImGui::InputFloat3("Sc", belle->GetTransform()->scale.data.m128_f32, 3);
	ImGuizmo::RecomposeMatrixFromComponents(belle->GetTransform()->position.data.m128_f32, 
		belle->GetTransform()->rotation.data.m128_f32,
		belle->GetTransform()->scale.data.m128_f32, belle->GetTransform()->GetWorld().r[0].m128_f32);
	
	ImGuizmo::DrawCube(CAMERA->GetCamView()->r->m128_f32, CAMERA->GetCamRotation()->r->m128_f32, objectMatrix);
	//ImGui::CaptureMouseFromApp();
	//ImGuizmo::DrawCube(cameraView, cameraProjection, objectMatrix);
	//ImGuizmo::Manipulate(VP->GetView()->r->m128_f32, VP->GetProjection()->r->m128_f32,
	//	mCurrentGizmoOperation, mCurrentGizmoMode, belle->GetTransform()->GetWorld().r[0].m128_f32,
		//nullptr, nullptr, nullptr);

	//
	//ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation,
	//	mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL,
	//  boundSizingSnap ? boundsSnap : NULL);
}
