#include "Framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	//coll = new BoxCollider();
	coll2 = new SphereCollider();
	coll = new BoxCollider();
}

CollisionScene::~CollisionScene()
{
	delete coll;
	delete coll2;
}

void CollisionScene::Update()
{
	if (KEYDOWN(VK_LBUTTON))
	{
		Ray ray = CAMERA->GetRay();

		if (coll->IsCollision(ray))
		{
			coll->SetColor(Float4(1, 0, 0, 1));
		}
		else
		{
			coll->SetColor(Float4(0, 1, 0, 1));
		}
	}

	if (KEYPRESS('I'))
		coll->position += kForward * DELTA;
	if (KEYPRESS('K'))
		coll->position -= kForward * DELTA;

	if (KEYPRESS('J'))
		coll->position -= kRight * DELTA;
	if (KEYPRESS('L'))
		coll->position += kRight * DELTA;

	if (KEYPRESS('U'))
		coll->position += kUp * DELTA;
	if (KEYPRESS('O'))
		coll->position -= kUp * DELTA;

	if (KEYPRESS(VK_NUMPAD4))
		coll->rotation -= kUp * DELTA;
	if (KEYPRESS(VK_NUMPAD6))
		coll->rotation += kUp * DELTA;

	if (KEYPRESS(VK_NUMPAD8))
		coll->rotation -= kRight * DELTA;
	if (KEYPRESS(VK_NUMPAD2))
		coll->rotation += kRight * DELTA;

	if (KEYPRESS(VK_NUMPAD7))
		coll->scale -= kRight * DELTA;
	if (KEYPRESS(VK_NUMPAD9))
		coll->scale += kRight * DELTA;

	if (KEYPRESS(VK_NUMPAD1))
		coll->scale -= kUp * DELTA;
	if (KEYPRESS(VK_NUMPAD3))
		coll->scale += kUp * DELTA;
		
	if (coll->IsCollision(coll2))
	{
		coll2->SetColor(Float4(1, 0, 0, 1));
	}
	else
	{
		coll2->SetColor(Float4(0, 1, 0, 1));
	}
}

void CollisionScene::PreRender()
{
}

void CollisionScene::Render()
{
	coll->Render();
	coll2->Render();
}

void CollisionScene::PostRender()
{
}
