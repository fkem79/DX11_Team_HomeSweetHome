#include "Framework.h"

Kaya::Kaya()
	: moveSpeed(30.0f), rotSpeed(2.0f), state(IDLE), accelation(10.0f),
	deceleration(3.0f), velocity(0, 0, 0)
{
	model = new ModelAnimator(L"ModelAnimationInstancing");
	transform = model->AddTransform();
	ReadData();	
	model->SetEndEvent(ATTACK, bind(&Kaya::SetIdle, this));

	model->PlayClip(0, state);

	transform->scale = Vector3(0.05f, 0.05f, 0.05f);
}

Kaya::~Kaya()
{
	delete model;
}

void Kaya::Update()
{	
	Input();
	Move();
	Rotate();

	transform->position.SetY(terrain->GetHeight(transform->position));

	model->Update();	
}

void Kaya::Render()
{
	model->Render();
}

void Kaya::Input()
{
	if (KEYPRESS(VK_UP))
		velocity -= transform->GetForward() * accelation * DELTA;
	if (KEYPRESS(VK_DOWN))
		velocity += transform->GetForward() * accelation * DELTA;

	if (KEYPRESS(VK_RIGHT))
		transform->rotation += transform->GetUp() * rotSpeed * DELTA;
	if (KEYPRESS(VK_LEFT))
		transform->rotation -= transform->GetUp() * rotSpeed * DELTA;

	if (KEYDOWN(VK_SPACE))
		SetAnimation(ATTACK);

	if (KEYDOWN(VK_LBUTTON))
	{
		path.clear();

		terrain->ComputePicking(&destPos);		

		Ray ray;
		ray.position = transform->position;
		ray.direction = (destPos - transform->position).Normal();

		float distance = Distance(ray.position, destPos);

		if (aStar->IsCollisionObstacle(ray, distance))
		{
			int startIndex = aStar->FindCloseNode(transform->position);
			int endIndex = aStar->FindCloseNode(destPos);

			aStar->Reset();

			path = aStar->FindPath(startIndex, endIndex);

			aStar->MakeDirectPath(transform->position, destPos, path);

			path.insert(path.begin(), destPos);

			int pathSize = path.size();

			while (path.size() > 2)
			{
				vector<Vector3> tempPath;
				for (int i = 1; i < path.size() - 1; i++)
					tempPath.push_back(path[i]);

				Vector3 start = path.back();
				Vector3 end = path.front();

				aStar->MakeDirectPath(start, end, tempPath);

				path.clear();
				path.push_back(end);

				for (Vector3 temp : tempPath)
					path.push_back(temp);

				path.push_back(start);

				if (pathSize == path.size())
					break;
				else
					pathSize = path.size();
			}
		}
		else
		{
			path.push_back(destPos);
		}
	}

}

void Kaya::Move()
{
	if (state == ATTACK)
		return;

	MovePath();

	float magnitude = velocity.Length();

	if (magnitude > 1.0f)
		velocity.Normalize();

	if (magnitude > 0.1f)
	{
		transform->position += velocity * moveSpeed * DELTA;

		SetAnimation(RUN);

		velocity = XMVectorLerp(velocity.data, XMVectorZero(), deceleration * DELTA);
	}
	else
	{
		SetAnimation(IDLE);
	}	
}

void Kaya::MovePath()
{
	if (path.empty())
		return;

	Vector3 dest = path.back();

	Vector3 direction = dest - transform->position;
	velocity = direction.Normal();

	if (direction.Length() < 0.3f)
		path.pop_back();
}

void Kaya::Rotate()
{
	if (velocity.Length() < 0.1f)
		return;

	Vector3 start = transform->GetForward();
	Vector3 end = velocity.Normal();

	float cosValue = start.Dot(end);
	float angle = acos(cosValue);

	if (angle < 0.1f)
		return;

	Vector3 cross = start.Cross(end);

	if (cross.GetY() > 0.0f)
		transform->rotation -= kUp * DELTA * rotSpeed;
	else
		transform->rotation += kUp * DELTA * rotSpeed;
}

void Kaya::SetAnimation(AnimState value)
{
	if (state != value)
	{
		state = value;
		model->PlayClip(0, value);
	}
}

void Kaya::SetIdle()
{
	SetAnimation(IDLE);
}

void Kaya::ReadData()
{
	string name = "Kaya";

	/*
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Capoeira.fbx");
	reader->ExportAnimClip(0, name + "/Capoeira");
	delete reader;
	

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Idle.fbx");
	reader->ExportAnimClip(0, name + "/Idle");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Running.fbx");
	reader->ExportAnimClip(0, name + "/Running");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Attack.fbx");
	reader->ExportAnimClip(0, name + "/Attack");
	delete reader;
	*/

	string path = name + "/" + name;
	model->ReadMaterial(path);
	model->ReadMesh(path);
	model->ReadClip(name + "/Capoeira");
	model->ReadClip(name + "/Idle");
	model->ReadClip(name + "/Running");
	model->ReadClip(name + "/Attack");
}
