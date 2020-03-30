#include "Framework.h"
#include "ParticleScene.h"
#include "Objects/Particle/Rain.h"
#include "Objects/Particle/Spark.h"
#include "Objects/Particle/Snow.h"

ParticleScene::ParticleScene()
{
	rain = new Rain();
	snow = new Snow();

	spark = new Spark();

	sky = new CubeSky();
}

ParticleScene::~ParticleScene()
{
	delete rain;
	delete spark;
	delete snow;
	delete sky;
}

void ParticleScene::Update()
{
	rain->Update();
	snow->Update();
	spark->Update();
	//sky->Update();

	if (KEYPRESS(VK_LBUTTON))
	{
		spark->Play(Vector3(0, 0, 0));
	}
}

void ParticleScene::PreRender()
{
}

void ParticleScene::Render()
{
	sky->Render();
	//rain->Render();
	spark->Render();
	snow->Render();
}

void ParticleScene::PostRender()
{
}
