#include "Framework.h"
#include "InstancingModelScene.h"

InstancingModelScene::InstancingModelScene()
{	
	Kaya();
}

InstancingModelScene::~InstancingModelScene()
{
	delete kaya;
}

void InstancingModelScene::Update()
{
	kaya->Update();
}

void InstancingModelScene::PreRender()
{
}

void InstancingModelScene::Render()
{
	kaya->Render();
}

void InstancingModelScene::PostRender()
{
}

void InstancingModelScene::Kaya()
{	
	string name = "Kaya";
	
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;
	

	//kaya = new ModelRender(L"ModelInstancing");
	kaya = new ModelAnimator(L"ModelAnimationInstancing");
	kaya->ReadMaterial(name + "/" + name);
	kaya->ReadMesh(name + "/" + name);	
	kaya->ReadClip(name + "/Idle");
	kaya->ReadClip(name + "/Running");
	kaya->ReadClip(name + "/Attack");

	UINT instanceNum = 0;

	vector<ModelClip*>* clips = kaya->GetModel()->GetClips();

	for (float z = -5; z < 5; z++)
	{
		for (float x = -5; x < 5; x++)
		{			
			Transform* transform = kaya->AddTransform();
			transform->scale = { 0.05f, 0.05f, 0.05f };
			transform->position = { x * 10, 0, z * 10 };
			transform->UpdateWorld();

			kaya->PlayClip(instanceNum, Random(0, clips->size()), Random(1.0f, 10.0f));
			instanceNum++;
		}
	}

	kaya->UpdateTransforms();	
}
