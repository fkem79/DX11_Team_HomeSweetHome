#include "Framework.h"
#include "ObjectCreateManager.h"

ObjectCreateManager::ObjectCreateManager()
	:mapToolWindow(false), totalObjTestX(0.0f), check(false), addNameWindow(false), fileCheck(true), allObjBoxRenderOn(true),
	curObjNum(1000), totalObjNum(0)
{
	
}

ObjectCreateManager::~ObjectCreateManager()
{
	for (ModelSingle* obj : totalObj)
		delete obj;

	totalObj.clear();
}

void ObjectCreateManager::Update()
{
	if (totalObj.size() <= 0)
		curObjNum = 1000;

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
	{
		if(!allObjBoxRenderOn)
			obj->SetBoxRenderCheck(false);
		else
			obj->SetBoxRenderCheck(true);
		
		obj->Update();
	}
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
		if (ImGui::Button("Same Add"))
		{
			MapToolSameAdd();
		}
		ImGui::SameLine();
		if (ImGui::Button("Copy"))
		{
			MapToolCopy();
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			MapToolDelete();
		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			MapToolSave();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			MapToolLoad();
		}
		ImGui::SameLine();
		if (ImGui::Button("Close"))
			mapToolWindow = false;

		ImGui::Checkbox("Object Box Render OnOff", &allObjBoxRenderOn);
		ImGui::Text("All Object Count : %d", totalObj.size());

		ImGui::End();
	}

	if (addNameWindow)
	{
		MapToolAddWindow();
	}
}

void ObjectCreateManager::ObjectSettingWindow()
{
	if (curObjNum == 1000)
		return;

	UINT num = curObjNum;
	{
		ImGui::BeginChildFrame(1, ImVec2(400, 200));
		ImGui::BeginChild("g1", ImVec2(400, 200), false);

		ImGui::Text("Name : %s objNum : %d", totalObj[num]->GetModelName().c_str(), totalObj[num]->GetModelNum());

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

void ObjectCreateManager::MapToolAddWindow()
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
			model->SetModelNum(totalObjNum++);
			model->scale *= 0.1f;
			model->rotation = { 1.6f, 0, 0 };
			model->position = { totalObjTestX, 1, 10 };
			totalObj.push_back(model);

			totalObjTestX = totalObj.size() * 10.0f;
			addNameWindow = false;
		}
	}

	if (!fileCheck)
		ImGui::Text("Not Exist");

	if (ImGui::Button("Close"))
		addNameWindow = false;

	ImGui::End();
}

void ObjectCreateManager::MapToolSameAdd()
{
	string temp = curStr;

	if (fileCheck = Path::ExistFile("ModelData/Models/" + temp + ".fbx"))
	{
		ModelSingle* model = new ModelSingle(temp);
		model->SetModelNum(totalObjNum++);
		model->scale *= 0.1f;
		model->rotation = { 1.6f, 0, 0 };
		model->position = { totalObjTestX, 1, 10 };
		totalObj.push_back(model);

		totalObjTestX = totalObj.size() * 10.0f;
		addNameWindow = false;
	}
}

void ObjectCreateManager::MapToolCopy()
{
	UINT num = curObjNum;

	ModelSingle* model = new ModelSingle(totalObj[num]->GetModelName());
	model->SetModelNum(totalObjNum++);
	model->scale *= totalObj[num]->scale;
	model->rotation = totalObj[num]->rotation;
	model->position = { totalObj[num]->position.x + 10, totalObj[num]->position.y,  totalObj[num]->position.z };

	model->GetCollBox()->position = totalObj[num]->GetCollBox()->position;
	model->GetCollBox()->rotation = totalObj[num]->GetCollBox()->rotation;
	model->GetCollBox()->scale = totalObj[num]->GetCollBox()->scale;

	totalObj.push_back(model);

	totalObjTestX = totalObj.size() * 10.0f;
}

void ObjectCreateManager::MapToolDelete()
{
	if (totalObj.size() <= 0)
		return;

	UINT num = curObjNum;
	totalObj.erase(totalObj.begin() + num);
	curObjNum = 1000;
	totalObjNum = totalObj.size();

	UINT i = 0;
	for (ModelSingle* ms : totalObj)
		ms->SetModelNum(i++);
}

void ObjectCreateManager::MapToolSave()
{
	BinaryWriter writer(L"TextData/ObjectMapData.map");

	vector<ObjData> datas;

	for (ModelSingle* model : totalObj)
	{
		ObjData objData;

		objData.objName = model->GetModelName();
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

	for (ObjData data : datas)
	{
		writer.String(data.objName);
		writer.UInt(data.objNum);

		writer.Float(data.Position.x);
		writer.Float(data.Position.y);
		writer.Float(data.Position.z);

		writer.Float(data.Rotation.x);
		writer.Float(data.Rotation.y);
		writer.Float(data.Rotation.z);

		writer.Float(data.Scale.x);
		writer.Float(data.Scale.y);
		writer.Float(data.Scale.z);

		writer.Float(data.collPos.x);
		writer.Float(data.collPos.y);
		writer.Float(data.collPos.z);

		writer.Float(data.collRot.x);
		writer.Float(data.collRot.y);
		writer.Float(data.collRot.z);

		writer.Float(data.collScale.x);
		writer.Float(data.collScale.y);
		writer.Float(data.collScale.z);
	}
}

void ObjectCreateManager::MapToolLoad()
{
	BinaryReader reader(L"TextData/ObjectMapData.map");

	UINT size = reader.UInt();

	for (ModelSingle* ms : totalObj)
		delete ms;

	totalObj.clear();

	for (UINT i = 0; i < size; i++)
	{
		ModelSingle* model = new ModelSingle(reader.String());
		model->SetModelNum(reader.UInt());

		model->position = { reader.Float(), reader.Float(), reader.Float() };
		model->rotation = { reader.Float(), reader.Float(), reader.Float() };
		model->scale = { reader.Float(), reader.Float(), reader.Float() };

		model->GetCollBox()->position = { reader.Float(), reader.Float(), reader.Float() };
		model->GetCollBox()->rotation = { reader.Float(), reader.Float(), reader.Float() };
		model->GetCollBox()->scale = { reader.Float(), reader.Float(), reader.Float() };

		totalObj.push_back(model);
	}

	curObjNum = totalObj.size()-1;
	totalObjNum = totalObj.size();
	strcpy_s(curStr, totalObj[curObjNum]->GetModelName().c_str());
	totalObjTestX = totalObj.size() * 10.0f;
}
