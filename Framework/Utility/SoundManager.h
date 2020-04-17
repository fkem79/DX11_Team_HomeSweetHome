#pragma once

#define SOUNDCOUNT 15
using namespace FMOD;

struct SoundInfo
{
	Sound* sound;
	Channel* channel;

	SoundInfo() : sound(nullptr), channel(nullptr)
	{

	}

	~SoundInfo()
	{
		sound->release();
	}
};

class SoundManager
{
private:
	SoundManager();
	~SoundManager();

public:
	static SoundManager* Get() { return instance; }
	static void Create() { instance = new SoundManager(); }
	static void Delete() { delete instance; }

	void Update();

	void Add(string key, string fileName, bool bgm);

	void Play(string key, float volume = 1.0f);
	void Stop(string key);
	void Pause(string key);
	void Resume(string key);

private:
	static SoundManager* instance;

	System* soundSystem;

	map<string, SoundInfo*> totalSound;
};