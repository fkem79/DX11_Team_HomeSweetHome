#include "Framework.h"
#include "ObjectCreateManager.h"

ObjectCreateManager::ObjectCreateManager()
	:mapToolWindow(false), addXPos(0.0f), check(false), addNameWindow(false), saveNameWindow(false), loadNameWindow(false),
	fileCheck(true), allObjBoxRenderOn(true), curObjIndex(-1), curModelIndex(-1),totalObjNum(0), shaderMode(1)
{

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

	if (KEYPRESS(VK_LBUTTON))
	{
		Ray ray = CAMERA->GetRay();

		for (auto totalObj : insTotalObj)
		{
			int boxIndex = 0;
			indexCount = 0;
			for (BoxCollider* coll : *totalObj->GetTotalCollBox())
			{
				if (coll->IsCollision(ray))
				{
					//ms->SetCheck(!check);
					//curObjIndex = ms->GetObjectNum();
					curModelIndex = totalObj->GetModelNum();
					//curObjIndex = boxIndex;
					curObjIndex = indexCount;
					break;
				}
				boxIndex++;
				indexCount++;
			}
			boxIndex = 0;
			indexCount = 0;
		}
	}

	for (auto obj : insTotalObj)
	{
		obj->Update();
	}

}

void ObjectCreateManager::Render()
{
	for (ModelRender* obj : insTotalObj)
		obj->Render();
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
			for (auto mr : *total->GetTransforms())
				transCount++;
		}
		ImGui::Text("All Transform Count : %d", transCount);

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
		ImGui::BeginChildFrame(1, ImVec2(400, 150));
		ImGui::BeginChild("g1", ImVec2(400, 150), false);

		if (curModelIndex !=-1 && curObjIndex != -1)
		{
			/* 
				model index는 선택된 모델의 번호
				obj index는 선택된 모델에서 인스턴싱으로 개수를 여러 개 만들었을 때 몇번 째로 만든건지
				ex) bed 추가 -> model index : 0, obj Index : 0
					bed 한 개 더 추가 -> model index : 0, obj Index : 1
					desk 추가 -> model index : 1, obj Index :0
					총 2개의 모델, 3개의 오브젝트
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
			// 일단 메쉬랑 메테리얼 있는지 없는지 모르니까 그냥 생성한다.
			ModelReader* mReader = new ModelReader();
			mReader->ReadFile("ModelData/Models/" + temp + ".fbx");
			mReader->ExportMaterial(temp + "/" + temp);
			mReader->ExportMesh(temp + "/" + temp);
			delete mReader;

			int index = SearchModelName(temp);
			if (index == -1) // 이미 totalObj에 존재하는지 확인한다.
			{
				// 존재 안 하면 그냥 만들고 추가한다.
				ModelRender* model = new ModelRender(L"ModelInstancing");
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
				// 존재하면 새로 생성안하고 새로운 트랜스폼만 추가해서 한 개 추가만 해주면 된다.
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
			/* 잠시보류 : 로드할 거면 다시 실행하고 로드하면 오류 없음
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
						ModelRender* model = new ModelRender(L"ModelInstancing");
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
