#pragma once

#include "cocos2d.h"

class StartScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene *createScene();

	bool init() override;
	void onEnter() override;

	CREATE_FUNC(StartScene);
	StartScene();
	~StartScene();
};

