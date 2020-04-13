#include "Framework.h"
#include "FrustumScene.h"

FrustumScene::FrustumScene()
	//: drawCount(0)
{
	/*
	frustum = new Frustum();

	for (float z = -5.0f; z < 5.0f; z++)
	{
		for (float y = -5.0f; y < 5.0f; y++)
		{
			for (float x = -5.0f; x < 5.0f; x++)
			{
				Sphere* sphere = new Sphere(0.3f);
				sphere->position = Vector3(x, y, z);
				sphere->position.y = 10;
				spheres.push_back(sphere);
			}
		}
	}*/

	data = new TerrainData(L"Textures/HeightMaps/HeightMap.png");
	terrain = new QuadTreeTerrain(data);
}

FrustumScene::~FrustumScene()
{
	/*
	for (Sphere* sphere : spheres)
		delete sphere;

	delete frustum;
	*/

	delete data;
	delete terrain;
}

void FrustumScene::Update()
{
	/*
	frustum->Update();

	for (Sphere* sphere : spheres)
		if(frustum->ContainCube(sphere->position, sphere->GetRadius()))
			sphere->Update();
			*/
	terrain->Update();
}

void FrustumScene::PreRender()
{
}

void FrustumScene::Render()
{
	/*
	drawCount = 0;
	for (Sphere* sphere : spheres)
	{
		if (frustum->ContainCube(sphere->position, sphere->GetRadius()))
		{
			sphere->Render();
			drawCount++;
		}
	}*/
	terrain->Render();
}

void FrustumScene::PostRender()
{
	//ImGui::Text("DrawCount : %d", drawCount);
	terrain->PostRender();
}
