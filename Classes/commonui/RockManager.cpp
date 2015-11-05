#include "RockManager.h"

USING_NS_CC;



RockManager *g_RockManager = nullptr;
RockManager* RockManager::createWithImage(const std::string &rock, const std::string &point)
{
	if (g_RockManager == nullptr)
	{
		g_RockManager = new RockManager;
		if (g_RockManager && g_RockManager->init(rock, point))
		{
			//g_RockManager->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(g_RockManager);
		}
	}

	return g_RockManager;
}


RockManager* RockManager::getInstance()
{
	return g_RockManager;
}

void RockManager::destroyInstance()
{
	if (g_RockManager)
	{
		delete g_RockManager;
		g_RockManager = nullptr;
	}
}




RockManager::RockManager()
	:Node()
	, _velocity(0.0f, 0.0f)
	, _pressDown(false)
{

}

RockManager::~RockManager()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool RockManager::init(const std::string &rock, const std::string &point)
{
	_rock = Sprite::create(rock);
	setContentSize(_rock->getContentSize());

	Vec2 centerPoint(getContentSize().width / 2, getContentSize().height / 2);
	_rock->setPosition(centerPoint);
	addChild(_rock);

	_point = Sprite::create(point);
	_point->setPosition(centerPoint);
	addChild(_point);
	

	_listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);

	_listener->onTouchBegan = [=](Touch *touch, Event *e){

		auto location = convertToNodeSpace(touch->getLocation());
		auto rect = Rect(0, 0, _rock->getContentSize().width, _rock->getContentSize().height);
		if (rect.containsPoint(location))
		{
			movePosition(location);
			_pressDown = true;
		}
			
		return true;
	};

	_listener->onTouchMoved = [=](Touch *touch, Event *e){
		
		if (_pressDown)
		{
			auto pos = _rock->convertToNodeSpace(touch->getLocation());
			_velocity = pos - centerPoint;
			movePosition(pos);
		}

	};

	_listener->onTouchEnded = [=](Touch *touch, Event *e){

		_velocity.setZero();
		movePosition(Vec2(getContentSize().width/2, getContentSize().height/2));
		_pressDown = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);


	return true;
}



void RockManager::movePosition(Point p)
{
	Vec2 centerPoint(getContentSize().width / 2, getContentSize().height / 2);
	float r = getContentSize().width / 2; // 半径

	if (p.distance(centerPoint) > r)
	{
		// 在控制圈外
		float t_x = p.x - r;
		float t_y = p.y - r;
		float t_z = sqrt(t_x * t_x + t_y * t_y);

		float n = r / t_z; // 比例

		p.set(Vec2(t_x * n + r, t_y * n + r));

	}
	_point->setPosition(p);
}


