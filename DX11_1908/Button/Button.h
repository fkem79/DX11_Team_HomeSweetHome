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

	float smallScale;
protected:
	Shader* shader;
	BoxCollider* coll;

	Render2D* texture;

	Vector3 min, max;

	Float4 color;

	State state;

	int intValue;
	void* voidValue;

	Event event;
	EventInt intEvent;
	EventVoid voidEvent;

	bool check;

	Render2D* render2D;
	bool select;
public:
	Button(Render2D* texture, bool start = true);
	~Button();

	void Update();
	void Render();

	void Create();

	void SetIntValue(int value) { intValue = value; }
	void SetVoidValue(void* value) { voidValue = value; }

	void SetEvent(Event Event) { event = Event; }
	void SetIntEvent(EventInt Event) { intEvent = Event; }
	void SetVoidEvent(EventVoid Event) { voidEvent = Event; }

	void SetCheck(bool value) { check = value; }
};