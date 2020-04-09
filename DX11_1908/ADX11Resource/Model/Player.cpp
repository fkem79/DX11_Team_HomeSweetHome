#include "Framework.h"
#include "Player.h"

Player::Player()
	:oldYPos(0.0f)
{
	head = new PlayerHead();
	leftHand = new PlayerLHand();

	head->GetTransform()->position = { 10.0f, 0.0f, 10.0f };
	head->GetTransform()->rotation = { 0, 180.0f, 0 };
	leftHand->GetTransform()->SetParent(head->GetTransform()->GetWorldPointer());
	leftHand->GetTransform()->position = { 3.0f, 160.0f, 0.0f };

	CAMERA->SetTarget(head->GetTransform());
}

Player::~Player()
{
	delete head;
	delete leftHand;
}

void Player::Update()
{
	head->Update();
	leftHand->Update();
	//CharAndCamSet();
	//��� ����
}

void Player::Render()
{
	head->Render();
	leftHand->Render();
}

void Player::PostRender()
{
	ImGui::Text("Player");
	ImGui::Text("Head  Pos X %f, Y %f, Z %f", head->GetTransform()->position.GetX(),
		head->GetTransform()->position.GetY(), head->GetTransform()->position.GetZ());

	ImGui::Text("LHand Pos X %f, Y %f, Z %f", leftHand->GetTransform()->position.GetX(),
		leftHand->GetTransform()->position.GetY(), leftHand->GetTransform()->position.GetZ());

	ImGui::Text("LHand Rot X %f, Y %f", leftHand->GetTransform()->rotation.GetX(),
		leftHand->GetTransform()->rotation.GetY() );
}

void Player::CharAndCamSet()
{
	if (CAMERA->GetTargetTransform() == nullptr)
		return;

	float val = CAMERA->GetTargetOffset().GetY() - oldYPos;

	//if (CAMERA->GetTargetOffset().GetY() > 7.0f)
	if (val > 0.0f)
	{
		//leftHand->GetTransform()->position.SetY(leftHand->GetTransform()->position.GetY() + 10.0f * DELTA);
		//leftHand->GetTransform()->rotation.SetX(leftHand->GetTransform()->rotation.GetX() + 10.0f * DELTA);
		leftHand->GetTransform()->rotation.SetX(3.0f);

		if (leftHand->GetTransform()->rotation.GetX() > 5.0f)
			leftHand->GetTransform()->rotation.SetX(5.0f);
	}
	//else if (CAMERA->GetTargetOffset().GetY() < 7.0f)
	else if (val < 0.0f)
	{
		//leftHand->GetTransform()->position.SetY(leftHand->GetTransform()->position.GetY() - 10.0f * DELTA);
		//leftHand->GetTransform()->rotation.SetX(leftHand->GetTransform()->rotation.GetX() - 10.0f * DELTA);
		leftHand->GetTransform()->rotation.SetX(-1.0f);

		if (leftHand->GetTransform()->rotation.GetX() < -5.0f)
			leftHand->GetTransform()->rotation.SetX(-5.0f);
	}

	oldYPos = CAMERA->GetTargetOffset().GetY();
		
}
