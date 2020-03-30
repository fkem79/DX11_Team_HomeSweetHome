#pragma once

class Timer
{
private:
	INT64 ticksPerSecond;
	INT64 currentTime;
	INT64 lastTime;
	INT64 lastFPSUpdate;

	UINT frameCount;
	float runningTime;
	float framePerSecond;

	float tickCount;
	float timeElapsed;

	static Timer* instance;

	Timer();
	~Timer();

public:
	static Timer* Get() { return instance; }
	static void Create() { instance = new Timer(); }
	static void Delete() { delete instance; }

	void Update();

	float Delta() { return timeElapsed; }
	float FPS() { return framePerSecond; }
	float Running() { return runningTime; }
};