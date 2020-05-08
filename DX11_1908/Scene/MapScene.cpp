#include "Framework.h"
#include "MapScene.h"

MapScene::MapScene()
{
	player = new Player();
	
	belle = new Belle();
	belle->GetTransform()->position = { 30.0f, 0.0f, 50.0f };

	Environment::Get()->GetLight()->data.ambient = { 0.1, 0.1, 0.1, 1 };

	ocm = new ObjectCreateManager();

	buffer = new LightInfoBuffer();
	LightInfo info;
	//info.type = LightInfo::POINT;
	//info.inner = 85.0f;
	info.inner =  88.256f;//89.419f;
	//info.outer = 30.0f; 
	info.outer = 14.712f;//16.258f;  
	info.range = 62.0f;
	info.type = LightInfo::SPOT;
	info.color = Float4(1, 1, 1, 1);
	info.position = Float3(-5, 5, 0);
	info.direction = Float3(0, 0, 1);

	buffer->Add(info);

	
}

MapScene::~MapScene()
{
	delete player;
	delete belle;

	delete ocm;
}

void MapScene::Update()
{
	buffer->data.lights[0].position = CAMERA->position/* + CAMERA->GetForward()*/;
	static Vector3 prev_forworad = CAMERA->GetForward();
	if (!(prev_forworad == CAMERA->GetForward()))
	{
		prev_forworad = CAMERA->GetForward();
		buffer->data.lights[0].direction = CAMERA->GetForward();
	}
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

	buffer->SetPSBuffer(3);
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

	ImGui::Text("LightInfo");
	for (UINT i = 0; i < buffer->data.lightCount; i++)
	{
		ImGui::SliderInt("Type" + i, (int*)&buffer->data.lights[i].type, 0, 3);
		ImGui::ColorEdit4("Color" + i, (float*)&buffer->data.lights[i].color);
		ImGui::SliderFloat3("Position" + i, (float*)&buffer->data.lights[i].position, -100, 100);
		ImGui::SliderFloat("Range" + i, &buffer->data.lights[i].range, 0, 100);
		ImGui::InputFloat3("Direction" + i, (float*)&buffer->data.lights[i].direction);
		//ImGui::SliderFloat3("Direction" + i, (float*)&buffer->data.lights[i].direction, -1, 1);
		ImGui::SliderFloat("Outer" + i, &buffer->data.lights[i].outer, 0, 180);
		ImGui::SliderFloat("Inner" + i, &buffer->data.lights[i].inner, 0, 180);
		ImGui::SliderFloat("Length" + i, &buffer->data.lights[i].length, 0, 100);
	}

	ocm->PostRender();
}

