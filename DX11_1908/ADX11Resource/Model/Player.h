#pragma once
class Player
{
private:
	PlayerHead* head;
	PlayerLHand* leftHand;

	float oldYPos;
public:
	Player();
	~Player();

	void Update();
	void Render();
	void PostRender();

	void CharAndCamSet();
};
