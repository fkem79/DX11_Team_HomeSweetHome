#pragma once

class FixityCam : public Camera
{
public:
	FixityCam();
	~FixityCam();

	void Update() override;
	void SetView(Matrix& view) { matView = view; }
};