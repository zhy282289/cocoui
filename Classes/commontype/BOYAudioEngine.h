#pragma once


#include <string>


#include "cocos2d.h"
#include "audio/include/AudioEngine.h"


class BOYAudioEngine : public cocos2d::experimental::AudioEngine
{
public:

	static int play2d_backgound(const std::string& filePath, bool loop = false, float volume = 1.0f, const cocos2d::experimental::AudioProfile *profile = nullptr);
	static int play2d_effect(const std::string& filePath, bool loop = false, float volume = 1.0f, const cocos2d::experimental::AudioProfile *profile = nullptr);


	static void setBackgroundEnable(bool enable);
	static bool getBackgroundEnable();

	static void setEffectEnable(bool enable);
	static bool getEffectEnable();

	//static void preload(const std::string& filePath, std::function<void(bool isSuccess)> callback = nullptr);

private:
	static bool hadBackground();
	static float getBackgroundVolume();


private:
	BOYAudioEngine();
	~BOYAudioEngine();

	static bool _backgoundEnable;
	static bool _effectEnable;

	static int _backgroundID;
};

