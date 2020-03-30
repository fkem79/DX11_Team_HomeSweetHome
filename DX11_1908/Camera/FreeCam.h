#pragma once

class FreeCam : public Camera
{
private:
	float moveSpeed;
	float rotSpeed;	
public:
	FreeCam();

	void Update() override;
};