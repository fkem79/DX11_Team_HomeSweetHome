#include "Framework.h"

//SoundManager* SoundManager::instance = nullptr;
//
//SoundManager::SoundManager()
//{
//	System_Create(&soundSystem);
//	soundSystem->init(SOUNDCOUNT, FMOD_INIT_NORMAL, nullptr);
//}
//
//SoundManager::~SoundManager()
//{
//	for (auto sound : totalSound)
//		delete sound.second;
//}
//
//void SoundManager::Update()
//{
//	soundSystem->update();
//}
//
//void SoundManager::Add(string key, string fileName, bool bgm)
//{
//	SoundInfo* info = new SoundInfo();
//
//	if (bgm)
//	{
//		soundSystem->createStream(fileName.c_str(), FMOD_LOOP_NORMAL, nullptr, &info->sound);
//	}
//	else
//	{
//		soundSystem->createSound(fileName.c_str(), FMOD_DEFAULT, nullptr, &info->sound);
//	}
//
//	totalSound.insert({ key, info });
//}
//
//void SoundManager::Play(string key, float volume)
//{
//	if (totalSound.count(key) == 0)
//		return;
//
//	soundSystem->playSound(totalSound[key]->sound,
//		nullptr, false, &totalSound[key]->channel);
//	totalSound[key]->channel->setVolume(volume);
//}
//
//void SoundManager::Stop(string key)
//{
//	if (totalSound.count(key) == 0)
//		return;
//
//	totalSound[key]->channel->stop();
//}
//
//void SoundManager::Pause(string key)
//{
//	if (totalSound.count(key) == 0)
//		return;
//
//	totalSound[key]->channel->setPaused(true);
//}
//
//void SoundManager::Resume(string key)
//{
//	if (totalSound.count(key) == 0)
//		return;
//
//	totalSound[key]->channel->setPaused(false);
//}
