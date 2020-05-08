#include "Framework.h"
#include "ObjectCreateManager.h"

ObjectCreateManager::ObjectCreateManager()
	:mapToolWindow(false), addXPos(0.0f), check(false), addNameWindow(false), saveNameWindow(false), loadNameWindow(false),
	fileCheck(true), allObjBoxRenderOn(true), selectObjBoxRenderOn(true),
	curObjIndex(-1), curModelIndex(-1),totalObjNum(0), shaderMode(1), shaderName(L"InstancingFlashLight")
{
	//ModelInstancing
	//InstancingFlashLight
}

ObjectCreateManager::~ObjectCreateManager()
{
	for (ModelRender* obj : insTotalObj)
		delete obj;

	insTotalObj.clear();
}

void ObjectCreateManager::Update()
{
	addXPos = Random(1.0f, 20.0f);
	addZPos = Random(1.0f, 20.0f);

	if (KEYPRESS(VK_LBUTTON) && allObjBoxRenderOn)
	{
		Ray ray = CAMERA->GetRay();

		for (auto totalObj : insTotalObj)
		{
			int boxIndex = 0;
			
			for (BoxCollider* coll : *totalObj->GetTotalCollBox())
			{
				if (coll->IsCollision(ray))
				{
					curModelIndex = totalObj->GetModelNum();
					curObjIndex = boxIndex;
					break;
				}
				boxIndex++;
			}
			boxIndex = 0;
		}
	}

	int mIndex = 0;
	for (auto obj : insTotalObj)
	{
		int objIndex = 0;
		for (BoxCollider* boxColl : *obj->GetTotalCollBox())
		{
			if (mIndex == curModelIndex && curObjIndex == objIndex)
				boxColl->SetColor(Float4(1, 0, 0, 1));
			else
				boxColl->SetColor(Float4(0, 1, 0, 1));

			objIndex++;
		}
		obj->Update();
		mIndex++;
		objIndex = 0;
	}

}

void ObjectCreateManager::Render()
{
	for (ModelRender* obj : insTotalObj)
		obj->Render();

	if (curModelIndex > -1 && curObjIndex > -1 && curModelIndex < insTotalObj.size() && selectObjBoxRenderOn)
		insTotalObj[curModelIndex]->GetCollBox(curObjIndex)->Render();
}

void ObjectCreateManager::PostRender()
{
	ObjectCreateWindow();
}

void ObjectCreateManager::ObjectCreateWindow()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("MapToolWindow", "Maptool Window"))
				mapToolWindow = true;
	
			if (ImGui::MenuItem("test", "test")) {  }

			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	if (mapToolWindow)
	{
		ImGui::Begin("MapTool Window", &mapToolWindow);

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
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			loadNameWindow = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Close"))
			mapToolWindow = false;

		//ImGui::Checkbox("Object Box Render OnOff", &allObjBoxRenderOn);
		ImGui::Text("All Model Count(insTotalObj) : %d", insTotalObj.size());
		int transCount = 0;
		for (auto total : insTotalObj)
		{
			total->SetBoxCollRenderCheck(allObjBoxRenderOn);
			for (auto mr : *total->GetTransforms())
				transCount++;
		}
		ImGui::Text("All Transform(Obj) Count : %d", transCount);
		ImGui::Checkbox("All Coll Box Render", &allObjBoxRenderOn);
		ImGui::Checkbox("Select Obj Coll Box Render", &selectObjBoxRenderOn);

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
	ImGui::InputInt("Model Index", &curModelIndex,20);
	{
		if(insTotalObj.size()<=0)
			curModelIndex = -1;
		else if (curModelIndex >= insTotalObj.size())
			curModelIndex = insTotalObj.size() - 1;
		else if (curModelIndex < 0)
			curModelIndex = -1;
	}
	//ImGui::SameLine();
	ImGui::InputInt("Obj Index", &curObjIndex,10);
	{
		if (curModelIndex < 0 || curObjIndex < 0)
			curObjIndex = -1;
		else if (curObjIndex >= insTotalObj[curModelIndex]->GetTransformsSize())
			curObjIndex = insTotalObj[curModelIndex]->GetTransformsSize() - 1;
		//else
			//curObjIndex = -1;
	}
	
	//ImGui::Text(" Model Index: %d, Obj Index : %d", curModelIndex, curObjIndex);
	{
		ImGui::BeginChildFrame(1, ImVec2(350, 120));
		ImGui::BeginChild("g1", ImVec2(350, 120), false);

		if (curModelIndex !=-1 && curObjIndex != -1)
		{
			/* 
				model index�� ���õ� ���� ��ȣ
				obj index�� ���õ� �𵨿��� �ν��Ͻ����� ������ ���� �� ������� �� ��� °�� �������
				ex) bed �߰� -> model index : 0, obj Index : 0
					bed �� �� �� �߰� -> model index : 0, obj Index : 1
					desk �߰� -> model index : 1, obj Index :0
					�� 2���� ��, 3���� ������Ʈ
			 */
			ImGui::Text("Name: %s, Model Index: %d, Obj Index : %d", 
				insTotalObj[curModelIndex]->GetModelName().c_str(), curModelIndex, curObjIndex);

			ImGui::InputFloat3("model pos", insTotalObj[curModelIndex]->GetTransform(curObjIndex)->position.data.m128_f32);
			ImGui::InputFloat3("model rot", insTotalObj[curModelIndex]->GetTransform(curObjIndex)->rotation.data.m128_f32);
			ImGui::InputFloat3("model sca", insTotalObj[curModelIndex]->GetTransform(curObjIndex)->scale.data.m128_f32);
			ImGui::Text("transforms count :%d , collbox count : %d",
				insTotalObj[curModelIndex]->GetTransformsSize(), insTotalObj[curModelIndex] ->GetCollBoxSize() );
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
			// �ϴ� �޽��� ���׸��� �ִ��� ������ �𸣴ϱ� �׳� �����Ѵ�.
			ModelReader* mReader = new ModelReader();
			mReader->ReadFile("ModelData/Models/" + temp + ".fbx");
			mReader->ExportMaterial(temp + "/" + temp);
			mReader->ExportMesh(temp + "/" + temp);
			delete mReader;

			int index = SearchModelName(temp);
			if (index == -1) // �̹� totalObj�� �����ϴ��� Ȯ���Ѵ�.
			{
				// ���� �� �ϸ� �׳� ����� �߰��Ѵ�.
				ModelRender* model = new ModelRender(shaderName);
				model->SetModelNum(insTotalObj.size());

				model->SetModelName(temp);
				model->ReadMaterial(temp + "/" + temp);
				model->ReadMesh(temp + "/" + temp);

				Transform* trans = model->AddTransform();
				trans->scale *= 0.1f;
				trans->rotation = { 1.57f, 0, 0 };
				trans->position = { addXPos , 1, addZPos };
				trans->UpdateWorld();
				model->UpdateTransforms();

				insTotalObj.push_back(model);
			}
			else
			{
				// �����ϸ� ���� �������ϰ� ���ο� Ʈ�������� �߰��ؼ� �� �� �߰��� ���ָ� �ȴ�.
				Transform* trans = insTotalObj[index]->AddTransform();
				trans->scale *= 0.1f;
				trans->rotation = { 1.57f, 0, 0 };
				trans->position = { addXPos , 1, addZPos };
				trans->UpdateWorld();
				insTotalObj[index]->UpdateTransforms();
			}
			
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
	if (curModelIndex <= -1 || curModelIndex >= insTotalObj.size())
		return;

	string temp = insTotalObj[curModelIndex]->GetModelName();

	if (Path::ExistFile("ModelData/Models/" + temp + ".fbx"))
	{
		int index = SearchModelName(temp);
		
		Transform* trans = insTotalObj[index]->AddTransform();
		trans->scale *= 0.1f;
		trans->rotation = { 1.57f, 0, 0 };
		trans->position = { addXPos , 1, addZPos };
		trans->UpdateWorld();
		insTotalObj[index]->UpdateTransforms();
	}
}

void ObjectCreateManager::MapToolCopy()
{
	if (curModelIndex <= -1 || curModelIndex >= insTotalObj.size())
		return;

	string temp = insTotalObj[curModelIndex]->GetModelName();

	if (Path::ExistFile("ModelData/Models/" + temp + ".fbx"))
	{
		int index = SearchModelName(temp);

		Transform* trans = insTotalObj[index]->AddTransform();
		trans->scale = insTotalObj[index]->GetTransform(curObjIndex)->scale;
		trans->rotation = insTotalObj[index]->GetTransform(curObjIndex)->rotation;
		trans->position = insTotalObj[index]->GetTransform(curObjIndex)->position;
		trans->UpdateWorld();
		insTotalObj[index]->UpdateTransforms();

		curObjIndex = insTotalObj[index]->GetTransformsSize()-1;
	}
}

void ObjectCreateManager::MapToolDelete()
{
	if (curModelIndex <= -1 || curModelIndex >= insTotalObj.size())
		return;

	UINT modelNum = curModelIndex;
	UINT num = curObjIndex;

	Transform* transTemp = insTotalObj[curModelIndex]->GetTransform(num);
	auto dNum = insTotalObj[modelNum]->GetTransforms()->begin() + num;
	insTotalObj[modelNum]->GetTransforms()->erase(dNum);

	BoxCollider* boxTemp = insTotalObj[curModelIndex]->GetCollBox(num);
	auto tNum = insTotalObj[modelNum]->GetTotalCollBox()->begin() + num;
	insTotalObj[modelNum]->GetTotalCollBox()->erase(tNum);

	delete boxTemp;
	delete transTemp;

	if(insTotalObj[modelNum]->GetTransformsSize() <=0)
	{
		ModelRender* modelTemp = insTotalObj[modelNum];
		auto mNum = insTotalObj.begin() + modelNum;
		insTotalObj.erase(mNum);

		delete modelTemp;
	
		UINT i = 0;
		for (ModelRender* mr : insTotalObj)
			mr->SetModelNum(i++);
	}

	curModelIndex = -1;
	curObjIndex = -1;
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

		for (ModelRender* mr : insTotalObj)
		{
			for(Transform* tr : *mr->GetTransforms())
			{
				ObjData objData;

				objData.objName = mr->GetModelName();
				objData.objNum = mr->GetModelNum();

				objData.Position = tr->position;
				objData.Rotation = tr->rotation;
				objData.Scale = tr->scale;

				datas.push_back(objData);
			}
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
			/* ��ú��� : �ε��� �Ÿ� �ٽ� �����ϰ� �ε��ϸ� ���� ����
			{
				for (ModelRender* obj : insTotalObj)
					delete obj;

				insTotalObj.clear();
				//insTotalObj.reserve(size);
			}*/

			for (UINT i = 0; i < size; i++)
			{
				string temp = reader.String();
				if (Path::ExistFile("ModelData/Models/" + temp + ".fbx"))
				{
					ModelReader* mReader = new ModelReader();
					mReader->ReadFile("ModelData/Models/" + temp + ".fbx");
					mReader->ExportMaterial(temp + "/" + temp);
					mReader->ExportMesh(temp + "/" + temp);
					delete mReader;

					int index = SearchModelName(temp);
					if (index == -1)
					{
						ModelRender* model = new ModelRender(shaderName);
						model->SetModelNum(reader.UInt());
						model->SetModelName(temp);
						model->ReadMaterial(temp + "/" + temp);
						model->ReadMesh(temp + "/" + temp);

						Transform* trans = model->AddTransform();
						trans->position = { reader.Float(), reader.Float(), reader.Float() };
						trans->rotation = { reader.Float(), reader.Float(), reader.Float() };
						trans->scale = { reader.Float(), reader.Float(), reader.Float() };
						trans->UpdateWorld();
						model->UpdateTransforms();

						insTotalObj.push_back(model);
					}
					else
					{
						Transform* trans = insTotalObj[index]->AddTransform();
						UINT tempInt = reader.UInt();
						trans->position = { reader.Float(), reader.Float(), reader.Float() };
						trans->rotation = { reader.Float(), reader.Float(), reader.Float() };
						trans->scale = { reader.Float(), reader.Float(), reader.Float() };
						trans->UpdateWorld();
						insTotalObj[index]->UpdateTransforms();
					}
				}
			}
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

int ObjectCreateManager::SearchModelName(string name)
{
	int rIndex = 0;
	for (ModelRender* mr : insTotalObj)
	{
		if (mr->GetModelName() == name)
			return rIndex;

		rIndex++;
	}
	return -1;
}
