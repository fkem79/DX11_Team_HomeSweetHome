#include "Framework.h"

ModelClip::ModelClip()
{
}

ModelClip::~ModelClip()
{
}

KeyFrame* ModelClip::GetKeyFrame(string name)
{
	if (keyFrameMap.count(name) == 0)
		return nullptr;

	return keyFrameMap[name];
}
