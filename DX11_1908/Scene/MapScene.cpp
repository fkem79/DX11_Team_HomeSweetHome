#include "Framework.h"
#include "MapScene.h"

MapScene::MapScene()
{
	Environment::Get()->GetLight()->data.ambient = { 0.05f, 0.05f, 0.05f, 1 };

	player = new Player();
	belle = new Belle();
	belle->GetTransform()->position = { 30.0f, 0.0f, 50.0f };
	ocm = new ObjectCreateManager();
	CreateLight();
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
	//static Vector3 prev_forworad = { CAMERA->GetForward().x*1.5f,  CAMERA->GetForward().y, CAMERA->GetForward().z*1.5f };
	static Vector3 prev_forworad = CAMERA->GetForward();
	if (!(prev_forworad == CAMERA->GetForward()))
	{
		prev_forworad = CAMERA->GetForward();
		buffer->data.lights[0].direction = CAMERA->GetForward();
	}

	if (!player->GetleftHandRenderCheck())
		buffer->data.lights[0].color = Float4(0, 0, 0, 0);
	else
		buffer->data.lights[0].color = Float4(1, 1, 1, 1);

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
	
	string lightNumBlank = "";
	for (int i = 0; i < buffer->data.lightCount; i++)
	{
		ImGui::Separator();
		ImGui::Text("Light Num : %d", i);
	
		lightNumBlank += " ";

		ImGui::SliderInt(("Type"+lightNumBlank).c_str(), (int*)&buffer->data.lights[i].type, 0, 3);
		ImGui::ColorEdit4(("Color" +lightNumBlank).c_str(), (float*)&buffer->data.lights[i].color);
		ImGui::InputFloat3(("Position" + lightNumBlank).c_str(), (float*)&buffer->data.lights[i].position);
		ImGui::SliderFloat(("Range" + lightNumBlank).c_str(), &buffer->data.lights[i].range, 0, 500);
		ImGui::InputFloat3(("Directon" + lightNumBlank).c_str(), (float*)&buffer->data.lights[i].direction);
		ImGui::SliderFloat(("Outer" + lightNumBlank).c_str(), &buffer->data.lights[i].outer, 0, 180);
		ImGui::SliderFloat(("Inner" + lightNumBlank).c_str(), &buffer->data.lights[i].inner, 0, 180);
		ImGui::SliderFloat(("Length" + lightNumBlank).c_str(), &buffer->data.lights[i].length, 0, 100);
	}

	ocm->PostRender();
}

void MapScene::CreateLight()
{
	buffer = new LightInfoBuffer();
	LightInfo info;
	//info.inner = 85.0f;
	info.inner = 88.256f;//89.419f;
	//info.outer = 30.0f; 
	info.outer = 14.712f;//16.258f;  
	info.type = LightInfo::SPOT;
	info.range = 110.0f;
	info.color = Float4(1, 1, 1, 1);
	info.position = Float3(-5, 5, 0);
	info.direction = Float3(0, 0, 1);

	buffer->Add(info);

	LightInfo info1;
	info1.inner = 88.256f;
	info1.outer = 14.712f;  
	info1.range = 48.5f;
	info1.type = LightInfo::POINT;
	info1.color = Float4(1, 1, 1, 1);
	info1.position = Float3(20.0f, 15.0f, 49.0f);
	info1.direction = Float3(0, 0, 1);

	buffer->Add(info1);

	LightInfo info2;
	info2.inner = 88.256f;
	info2.outer = 14.712f;
	info2.range = 48.5f;
	info2.type = LightInfo::POINT;
	info2.color = Float4(1, 1, 1, 1);
	info2.position = Float3(20.0f, 15.0f, 30.0f);
	info2.direction = Float3(0, 0, 1);

	buffer->Add(info2);

}

