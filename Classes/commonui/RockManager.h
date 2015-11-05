#pragma once


#include "cocos2d.h"

class RockManager : public cocos2d::Node
{
public:
	
	static RockManager* createWithImage(const std::string &rock, const std::string &point);

	
	cocos2d::Vec2 Velocity(){ return _velocity; }
	void setSwallowTouches(bool needSwallow) { _listener->setSwallowTouches(needSwallow); }

	static RockManager* getInstance();
	static void destroyInstance();
public:

	bool init(const std::string &rock, const std::string &point);
	void movePosition(cocos2d::Point p);

	RockManager();
	~RockManager();

private:
	cocos2d::Node *_rock;
	cocos2d::Node *_point;
	cocos2d::EventListenerTouchOneByOne *_listener;
	bool _pressDown;
	cocos2d::Vec2 _velocity;
};



