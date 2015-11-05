#include "BOYAudioEngine.h"


using namespace cocos2d::experimental;


bool BOYAudioEngine::_backgoundEnable = true;
bool BOYAudioEngine::_effectEnable = true;
int BOYAudioEngine::_backgroundID = AudioEngine::INVALID_AUDIO_ID;

bool BOYAudioEngine::hadBackground()
{
	return _backgroundID != AudioEngine::INVALID_AUDIO_ID;
}

BOYAudioEngine::BOYAudioEngine()
{
}


BOYAudioEngine::~BOYAudioEngine()
{
}

int BOYAudioEngine::play2d_backgound(const std::string& filePath, bool loop /*= false*/, float volume /*= 1.0f*/, const cocos2d::experimental::AudioProfile *profile /*= nullptr*/)
{
	if (hadBackground())
	{
		AudioEngine::stop(_backgroundID);
		_backgroundID = AudioEngine::INVALID_AUDIO_ID;
	}

	_backgroundID = AudioEngine::play2d(filePath, loop, getBackgroundVolume(), profile);
	
	return _backgroundID;
}

int BOYAudioEngine::play2d_effect(const std::string& filePath, bool loop /*= false*/, float volume /*= 1.0f*/, const cocos2d::experimental::AudioProfile *profile /*= nullptr*/)
{
	if (_effectEnable)
	{
		return AudioEngine::play2d(filePath, loop, volume, profile);
	}
	
	return AudioEngine::INVALID_AUDIO_ID;
}

void BOYAudioEngine::setBackgroundEnable(bool enable)
{
	_backgoundEnable = enable;
	if (hadBackground())
		AudioEngine::setVolume(_backgroundID, getBackgroundVolume());
}

bool BOYAudioEngine::getBackgroundEnable()
{
	return _backgoundEnable;
}

void BOYAudioEngine::setEffectEnable(bool enable)
{
	_effectEnable = enable;
}

bool BOYAudioEngine::getEffectEnable()
{
	return _effectEnable;
}

float BOYAudioEngine::getBackgroundVolume()
{
	return _backgoundEnable ? 1.0f : 0.0f;
}

//void BOYAudioEngine::preload(const std::string& filePath, std::function<void(bool isSuccess)> callback /*= nullptr*/)
//{
//	AudioEngine::preload(filePath, callback);
//}
//
