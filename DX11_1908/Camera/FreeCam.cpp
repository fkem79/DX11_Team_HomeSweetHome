#include "Framework.h"

FreeCam::FreeCam()
	: moveSpeed(20.0f), rotSpeed(1.5f)
{	
}

void FreeCam::Update()
{
	{//Move
		if (KEYPRESS(VK_RBUTTON))
		{
			if (KEYPRESS('W'))
				position += forward * moveSpeed * DELTA;
			if (KEYPRESS('S'))
				position -= forward * moveSpeed * DELTA;
			if (KEYPRESS('A'))
				position -= right * moveSpeed * DELTA;
			if (KEYPRESS('D'))
				position += right * moveSpeed * DELTA;
			if (KEYPRESS('Q'))
				position -= up * moveSpeed * DELTA;
			if (KEYPRESS('E'))
				position += up * moveSpeed * DELTA;
		}

		position += forward * Keyboard::Get()->GetWheel() * moveSpeed * DELTA;
	}

	{//Rotation
		Vector3 value = MOUSEPOS - oldPos;

		if (KEYPRESS(VK_RBUTTON))
		{
			rotation.SetX(rotation.GetX() + value.GetY() * rotSpeed * DELTA);
			rotation.SetY(rotation.GetY() + value.GetX() * rotSpeed * DELTA);

			Rotation();
		}

		oldPos = MOUSEPOS;
	}

	View();
}
