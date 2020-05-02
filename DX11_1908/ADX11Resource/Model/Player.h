#pragma once

class Player
{
private:
	PlayerLHand* leftHand;
	FlashLight* light;

	float oldYPos;
public:
	Player();
	~Player();

	void Update();
	void Render();
	void PostRender();

	//PlayerHead* GetPlayerHead() { return head; }
};
