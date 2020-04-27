#pragma once

class StartButton
{
private:
	Render2D* render2D;
	Button* button;
public:
	StartButton();
	~StartButton();

	void Update();
	void Render();

	void Create();

	void SetEvent(Event value) { button->SetEvent(value); }
};