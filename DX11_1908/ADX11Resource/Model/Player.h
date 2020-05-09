#pragma once

class Player
{
private:
	PlayerLHand* leftHand;
	FlashLight* light;

	float oldYPos;

	bool leftHandRenderOn;
public:
	Player();
	~Player();

	void Update();
	void Render();
	void PostRender();

	bool GetleftHandRenderCheck() { return leftHandRenderOn; }
	//PlayerHead* GetPlayerHead() { return head; }
};
