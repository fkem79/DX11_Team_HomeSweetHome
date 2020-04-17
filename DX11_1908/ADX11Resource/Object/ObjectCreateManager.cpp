#include "Framework.h"
#include "ObjectCreateManager.h"

ObjectCreateManager::ObjectCreateManager()
	:mapToolWindow(false), totalObjTestX(0.0f), check(false), addNameWindow(false), fileCheck(true), curObjNum(100), objNum(0)
{
	objNumbers.push_back(1);
	objNumbers.push_back(2);
	objNumbers.push_back(3);

	objNames.push_back("fan");
	objNames.push_back("desk");
	objNames.push_back("closet");
}

ObjectCreateManager::~ObjectCreateManager()
{
	for (ModelSingle* obj : totalObj)
		delete obj;

	totalObj.clear();
}

void ObjectCreateManager::AddObject(ModelSingle* addObj)
{
}

void ObjectCreateManager::Update()
{
	
	if (KEYPRESS(VK_LBUTTON))
	{
		Ray ray = CAMERA->GetRay();

		for (ModelSingle* ms : totalObj)
		{
			if (ms->GetCollBox()->IsCollision(ray))
			{
				//ms->SetCheck(!check);
				curObjNum = ms->GetModelNum();
			}
		}
			
	}

	for (ModelSingle* obj : totalObj)
		obj->Update();
}

void ObjectCreateManager::Render()
{
	for (ModelSingle* obj : totalObj)
		obj->Render();
}

void ObjectCreateManager::PostRender()
{
	for (ModelSingle* obj : totalObj)
		obj->PostRender();

	ObjectCreateWindow();
	
}

void ObjectCreateManager::ObjectCreateWindow()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("MapToolWindow", "Maptool Window"))
			{
				mapToolWindow = true;
			}
			if (ImGui::MenuItem("test", "test")) { /* do something */ }

			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();


	if (mapToolWindow)
	{
		ImGui::Begin("MapTool Window (test Version)", &mapToolWindow);

		ObjectSettingWindow();

		if (ImGui::Button("Add"))
		{
			addNameWindow = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			BinaryWriter writer(L"TextData/ObjectMapData.map");

			vector<ObjData> datas;

			for (ModelSingle* model : totalObj)
			{
				ObjData objData;
		
				objData.objNameNum = FindNameNum(model->GetModelName());
				objData.objNum = model->GetModelNum();

				objData.Position = model->position;
				objData.Rotation = model->rotation;
				objData.Scale = model->scale;

				objData.collPos = model->GetCollBox()->position;
				objData.collRot = model->GetCollBox()->rotation;
				objData.collScale = model->GetCollBox()->scale;

				datas.push_back(objData);
			}

			writer.UInt(datas.size());
			writer.Byte(datas.data(), sizeof(ObjData) * datas.size());
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			BinaryReader reader(L"TextData/ObjectMapData.map");

			UINT size = reader.UInt();

			vector<ObjData> datas(size);
			void* temp = (void*)datas.data();
			reader.Byte(&temp, sizeof(ObjData) * size);

			for (ModelSingle* ms : totalObj)
				delete ms;

			totalObj.clear();

			for (UINT i = 0; i < size; i++)
			{
				//ModelSingle* model = new ModelSingle("fan");
				
				ModelSingle* model = new ModelSingle(objNames[datas[i].objNameNum]);
				model->SetModelNum(datas[i].objNum);

				model->scale = datas[i].Scale;
				model->rotation = datas[i].Rotation;
				model->position = datas[i].Position;

				model->GetCollBox()->position = datas[i].collPos;
				model->GetCollBox()->rotation = datas[i].collRot;
				model->GetCollBox()->scale = datas[i].collScale;

				totalObj.push_back(model);
			}
			totalObjTestX -= 10.0f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Close"))
			mapToolWindow = false;
		ImGui::End();
	}

	if (addNameWindow)
	{
		ImGui::Begin("Add file Name", &addNameWindow);
		ImGui::InputText(" ", curStr, 20);
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			string temp = curStr;

			if (fileCheck = Path::ExistFile("ModelData/Models/" + temp + ".fbx"))
			{
				ModelSingle* model = new ModelSingle(temp);
				model->SetModelNum(++objNum);
				model->scale *= 0.1f;
				model->rotation = { 1.6f, 0, 0 };
				model->position = { totalObjTestX, 1, 10 };
				totalObj.push_back(model);

				totalObjTestX += 10.0f;
				addNameWindow = false;
				
			}
		}

		if (!fileCheck)
			ImGui::Text("Not Exist");

		if (ImGui::Button("Close"))
			addNameWindow = false;

		ImGui::End();
	}
}

void ObjectCreateManager::ObjectSettingWindow()
{
	if (curObjNum == 100)
		return;

	UINT num = curObjNum - 1;
	{
		ImGui::BeginChildFrame(1, ImVec2(400, 200));
		ImGui::BeginChild("g1", ImVec2(400, 200), false);

		ImGui::Text("Name %s objNum %d", totalObj[num]->GetModelName().c_str(), totalObj[num]->GetModelNum());

		ImGui::Separator();

		ImGui::Text("Obj");
		ImGui::InputFloat3("Obj Position", totalObj[num]->position.data.m128_f32, 3);
		ImGui::InputFloat3("Obj Scale", totalObj[num]->scale.data.m128_f32, 3);
		ImGui::InputFloat3("Obj Rotation", totalObj[num]->rotation.data.m128_f32, 3);

		ImGui::Text("CollBox");
		ImGui::InputFloat3("Position", totalObj[num]->GetCollBox()->position.data.m128_f32, 3);
		ImGui::InputFloat3("Scale", totalObj[num]->GetCollBox()->scale.data.m128_f32, 3);
		ImGui::InputFloat3("Rotation", totalObj[num]->GetCollBox()->rotation.data.m128_f32, 3);

		ImGui::EndChild();
		ImGui::EndChildFrame();
		
	}
}

UINT ObjectCreateManager::FindNameNum(string fineName)
{
	UINT count = 0;
	for (string str : objNames)
	{
		if (str == fineName)
			return count;

		count++;
	}
}
