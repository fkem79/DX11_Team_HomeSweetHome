#include "Framework.h"
#include "Player.h"

Player::Player()
	:oldYPos(0.0f)
{
	//head = new PlayerHead();
	leftHand = new PlayerLHand();
	light = new FlashLight();

	head->GetTransform()->position = { 10.0f, 0.0f, 10.0f };
	head->GetTransform()->rotation = { 0, 180.0f, 0 };
	//leftHand->GetTransform()->SetParent(head->GetTransform()->GetWorldPointer());
	if(CAMERA->GetCamNum() ==1)
		leftHand->GetTransform()->SetParent(CAMERA->GetCamInvView());

	leftHand->GetTransform()->position = { -0.5f, -1.6f, -0.5f };
	leftHand->GetTransform()->rotation = { 0.0f, 9.6f, 0.0f };
	leftHand->GetTransform()->scale = { 0.1f, 0.1f, 0.1f };
	//leftHand->GetTransform()->position = { 3.0f, 160.0f, 0.0f };

	light->GetFlashLight()->GetTransform(0)->SetParent(leftHand->GetTransform()->GetWorldPointer());
	light->GetFlashLight()->GetTransform(0)->position = { 10, 11, -21 };
	light->GetFlashLight()->GetTransform(0)->scale = { 1,1,1 };

	CAMERA->SetTarget(head->GetTransform());
	
}

Player::~Player()
{
	//delete head;
	delete leftHand;
	delete light;
}

void Player::Update()
{
	//head->Update();
	leftHand->Update();
	light->Update();
}

void Player::Render()
{
	//head->Render();
	leftHand->Render();
	light->Render();
}

void Player::PostRender()
{
	ImGui::Text("Player");
	
	ImGui::InputFloat3("Head pos", head->GetTransform()->position.data.m128_f32);

	ImGui::InputFloat3("LH pos", leftHand->GetTransform()->position.data.m128_f32);
	ImGui::InputFloat3("LH rot", leftHand->GetTransform()->rotation.data.m128_f32);
	ImGui::InputFloat3("LH sca", leftHand->GetTransform()->scale.data.m128_f32);

	ImGui::InputFloat3("l pos", light->GetFlashLight()->GetTransform(0)->position.data.m128_f32);
	ImGui::InputFloat3("l rot", light->GetFlashLight()->GetTransform(0)->rotation.data.m128_f32);
}
