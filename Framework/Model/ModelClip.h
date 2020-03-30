#pragma once

class ModelClip
{
private:
	friend class Model;

	string name;

	float duration;
	float frameRate;
	UINT frameCount;

	unordered_map<string, KeyFrame*> keyFrameMap;

public:
	ModelClip();
	~ModelClip();

	KeyFrame* GetKeyFrame(string name);

	float Duration() { return duration; }
	float FrameRate() { return frameRate; }
	UINT FrameCount() { return frameCount; }
};