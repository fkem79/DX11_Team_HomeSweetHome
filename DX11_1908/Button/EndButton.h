#pragma once

class EndButton
{
private:
	Render2D* render2D;
	Button* button;
public:
	EndButton();
	~EndButton();

	void Update();
	void Render();

	void Create();

	void SetEvent(Event value) { button->SetEvent(value); }

};