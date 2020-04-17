#pragma once

class Button : public Transform
{
public:
	enum State
	{
		NONE,
		DOWN,
		OVER
	};
protected:
	Shader* shader;
	
	Render2D texture;

	State state;

	int intValue;
	void* voidValue;

	Event event;
	EventInt intEvent;
	EventVoid voidEvent;

public:
	Button();
	~Button();

};