#include "Framework.h"
#include "ObjectCreateManager.h"

ObjectCreateManager::ObjectCreateManager()
	:mapToolWindow(false), addXPos(0.0f), check(false), addNameWindow(false), saveNameWindow(false), loadNameWindow(false),
	fileCheck(true), allObjBoxRenderOn(true),curObjIndex(-1), totalObjNum(0), shaderMode(1)
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
	{
		addXPos = 10;
		addZPos = 10;
	}
	else
	{
		addXPos = Random(totalObj[totalObj.size() - 1]->position.x - 5.0f, totalObj[totalObj.size() - 1]->position.x + 5.0f);
		addZPos = Random(totalObj[totalObj.size() - 1]->position.z - 5.0f, totalObj[totalObj.size() - 1]->position.z + 5.0f);
	}

	if (totalObj.size() <= 0)
		curObjIndex = -1;

	if (KEYPRESS(VK_LBUTTON))
	{
		Ray ray = CAMERA->GetRay();

		for (ModelSingle* ms : totalObj)
		{
			if (ms->GetCollBox()->IsCollision(ray))
			{
				//ms->SetCheck(!check);
				curObjIndex = ms->GetModelNum();
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
			saveNameWindow = true;
			MapToolSave();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			loadNameWindow = true;
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

	if (saveNameWindow)
	{
		MapToolSave();
	}

	if (loadNameWindow)
	{
		MapToolLoad();
	}
}

void ObjectCreateManager::ObjectSettingWindow()
{
	ImGui::InputInt(" ", &curObjIndex, 5);
	{
		if (curObjIndex >= totalObj.size())
			curObjIndex = totalObj.size() - 1;
		else if (curObjIndex < 0)
			curObjIndex = -1;
	}

	UINT num = curObjIndex;
	{
		ImGui::BeginChildFrame(1, ImVec2(400, 150));
		ImGui::BeginChild("g1", ImVec2(400, 150), false);

		if (curObjIndex != -1)
		{
			ImGui::Text("Name : %s objNum : %d Shader : %s",
				totalObj[num]->GetModelName().c_str(), totalObj[num]->GetModelNum(), totalObj[num]->GetShaderName().c_str());

			ImGui::Separator();

			ImGui::Text("Obj");
			ImGui::InputFloat3("Obj Position", totalObj[num]->position.data.m128_f32, 3);
			ImGui::InputFloat3("Obj Scale", totalObj[num]->scale.data.m128_f32, 3);
			ImGui::InputFloat3("Obj Rotation", totalObj[num]->rotation.data.m128_f32, 3);
		}
		else
		{
			ImGui::Text("Select Object or Add Object");
			ImGui::Text("Not Obj");
		}

		ImGui::EndChild();
		ImGui::EndChildFrame();
	}
}

void ObjectCreateManager::MapToolAddWindow()
{
	ImGui::Begin("Add file Name", &addNameWindow);
	ImGui::InputText(" ", curFileName, 20);
	ImGui::SameLine();
	if (ImGui::Button("OK"))
	{
		string temp = curFileName;

		if (fileCheck = Path::ExistFile("ModelData/Models/" + temp + ".fbx"))
		{
			string shader;

			if (shaderMode ==1)
				shader = "ModelSingle";
			else if(shaderMode ==2)
				shader = "ModelInstancing";

			ModelSingle* model = new ModelSingle(temp, shader);
			model->SetModelNum(totalObjNum++);
			model->scale *= 0.1f;
			model->rotation = { 1.57f, 0, 0 };
			model->position = { addXPos , 1, addZPos };
			totalObj.push_back(model);

			curObjIndex = totalObj.size() - 1;
			addNameWindow = false;
		}
	}
	ImGui::BeginChildFrame(2, ImVec2(150, 100));
	ImGui::BeginChild("box1", ImVec2(150, 100), false);
	
	ImGui::RadioButton("ModelSingle", &shaderMode, 1);
	ImGui::RadioButton("ModelInstancing", &shaderMode, 2);

	ImGui::EndChild();
	ImGui::EndChildFrame();
	ImGui::SameLine();
	if (!fileCheck)
		ImGui::Text("Not Exist");

	if (ImGui::Button("Close"))
		addNameWindow = false;

	ImGui::End();
}

void ObjectCreateManager::MapToolSameAdd()
{
	string temp = curFileName;

	if (fileCheck = Path::ExistFile("ModelData/Models/" + temp + ".fbx"))
	{
		ModelSingle* model = new ModelSingle(temp);
		model->SetModelNum(totalObjNum++);
		model->scale *= 0.1f;
		model->rotation = { 1.57f, 0, 0 };
		model->position = { addXPos, 1, 10 };
		totalObj.push_back(model);

		addXPos = totalObj.size() * 10.0f;
		addNameWindow = false;
	}
}

void ObjectCreateManager::MapToolCopy()
{
	UINT num = curObjIndex;

	ModelSingle* model = new ModelSingle(totalObj[num]->GetModelName(), totalObj[num]->GetShaderName());
	model->SetModelNum(totalObjNum++);
	model->scale *= totalObj[num]->scale;
	model->rotation = totalObj[num]->rotation;
	model->position = { totalObj[num]->position.x, totalObj[num]->position.y, totalObj[num]->position.z };

	model->GetCollBox()->position = totalObj[num]->GetCollBox()->position;
	model->GetCollBox()->rotation = totalObj[num]->GetCollBox()->rotation;
	model->GetCollBox()->scale = totalObj[num]->GetCollBox()->scale;

	totalObj.push_back(model);
	curObjIndex = totalObj.size()-1;
}

void ObjectCreateManager::MapToolDelete()
{
	if (totalObj.size() <= 0 || curObjIndex <0)
		return;

	UINT num = curObjIndex;
	auto obj = totalObj.begin() + num;

	delete *obj;

	totalObj.erase(obj);
	curObjIndex = -1;
	totalObjNum = totalObj.size();

	UINT i = 0;
	for (ModelSingle* ms : totalObj)
		ms->SetModelNum(i++);
}

void ObjectCreateManager::MapToolSave()
{
	ImGui::Begin("save data name", &saveNameWindow);

	ImGui::Text("Ex) mapData");
	ImGui::InputText(" ", saveLoadName, 20);
	ImGui::SameLine();

	if (ImGui::Button("SAVE"))
	{
		//BinaryWriter writer(L"TextData/ObjectMapData.map");
		string tempName = saveLoadName;
		BinaryWriter writer("TextData/" + tempName+ ".map");

		vector<ObjData> datas;

		for (ModelSingle* model : totalObj)
		{
			ObjData objData;

			objData.objName = model->GetModelName();
			objData.objNum = model->GetModelNum();
			objData.objShader = model->GetShaderName();

			objData.Position = model->position;
			objData.Rotation = model->rotation;
			objData.Scale = model->scale;

			datas.push_back(objData);
		}

		writer.UInt(datas.size());

		for (ObjData data : datas)
		{
			writer.String(data.objName);
			writer.UInt(data.objNum);
			writer.String(data.objShader);

			writer.Float(data.Position.x);
			writer.Float(data.Position.y);
			writer.Float(data.Position.z);

			writer.Float(data.Rotation.x);
			writer.Float(data.Rotation.y);
			writer.Float(data.Rotation.z);

			writer.Float(data.Scale.x);
			writer.Float(data.Scale.y);
			writer.Float(data.Scale.z);
		}
		saveNameWindow = false;
	}

	if (ImGui::Button("Close"))
		saveNameWindow = false;

	ImGui::End();
}

void ObjectCreateManager::MapToolLoad()
{
	ImGui::Begin("load data name", &loadNameWindow);
	
	ImGui::Text("Ex) mapData ");
	ImGui::InputText(" ", saveLoadName, 20);
	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		string tempName = saveLoadName;
		if (Path::ExistFile("TextData/"+ tempName +".map"))
		{
			BinaryReader reader("TextData/" + tempName + ".map");

			UINT size = reader.UInt();

			for (ModelSingle* ms : totalObj)
				delete ms;

			totalObj.clear();

			totalObj.reserve(size);

			for (UINT i = 0; i < size; i++)
			{
				string modelName = reader.String();
				UINT modelNum = reader.UInt();
				string shaderName = reader.String();

				ModelSingle* model = new ModelSingle(modelName, shaderName);
				model->SetModelNum(modelNum);

				model->position = { reader.Float(), reader.Float(), reader.Float() };
				model->rotation = { reader.Float(), reader.Float(), reader.Float() };
				model->scale = { reader.Float(), reader.Float(), reader.Float() };

				totalObj.push_back(model);
			}

			curObjIndex = totalObj.size() - 1;
			totalObjNum = totalObj.size();
			strcpy_s(curFileName, totalObj[curObjIndex]->GetModelName().c_str());
			addXPos = totalObj.size() * 10.0f;

			loadNameWindow = false;
		}
		else
			fileCheck = false;	
	}

	if (!fileCheck)
		ImGui::Text("Not Exist");

	if (ImGui::Button("Close"))
	{
		loadNameWindow = false;
		fileCheck = true;
	}

	ImGui::End();
}
