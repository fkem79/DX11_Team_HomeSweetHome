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

	GizmoTest();
}

void MapScene::GizmoTest()
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
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

	
}
