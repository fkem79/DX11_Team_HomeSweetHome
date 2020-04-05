#include "Framework.h"

Timer* Timer::instance = nullptr;

Timer::Timer()
	: currentTime(0), lastFPSUpdate(0), 
	frameCount(0), runningTime(0), framePerSecond(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerSecond);
	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
	tickCount = 1.0f / ticksPerSecond;
}

Timer::~Timer()
{
}

void Timer::Update()
{	
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	timeElapsed = (float)(currentTime - lastTime) * tickCount;
	runningTime += timeElapsed;

	frameCount++;
	if (currentTime - lastFPSUpdate >= ticksPerSecond)
	{
		float tempCurTime = (float)currentTime * tickCount;
		float tempLastTime = (float)lastFPSUpdate * tickCount;
		framePerSecond = (float)frameCount / (tempCurTime - tempLastTime);

		lastFPSUpdate = currentTime;
		frameCount = 0;
	}

	lastTime = currentTime;
}
