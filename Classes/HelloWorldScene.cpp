#include "HelloWorldScene.h"


#include "public.h"

#include "commonui/RockManager.h"

USING_NS_CC;

#define AudioEngine_play2d(s) experimental::AudioEngine::play2d(s);

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_tileMap = cocos2d::experimental::TMXTiledMap::create("TMXTileMaps/test1.tmx");
	addChild(_tileMap);


	// player
	//auto objectGroup = _tileMap->getObjectGroup("Objects");
	//auto valueMap = objectGroup->getObject("SpawnPoint");

	_sprite = Sprite::create("CloseNormal.png");
	//_tileMap->addChild(_sprite);
	addChild(_sprite);
	_sprite->setPosition(100, 100);


	auto metaLayer = _tileMap->getLayer("Meta");
	metaLayer->setVisible(false);


	//auto lister = EventListenerTouchOneByOne::create();
	//lister->onTouchBegan = [](Touch *touch, Event *e){
	//	return true;
	//};

	//lister->onTouchEnded = [=](Touch *touch, Event *e){
	//	auto location = touch->getLocation();

	//	auto pos = this->convertToNodeSpace(location);
	//	//auto diff = location - _sprite->getPosition();
	//	if (pos.x >= 0 && pos.y >= 0)
	//	{
	//		setPlayerPosition(pos);
	//		setViewPointCenter(pos);
	//	}

	//};

	//_eventDispatcher->addEventListenerWithSceneGraphPriority(lister, this);

	scheduleUpdate();

	experimental::AudioEngine::preload("TileGameResources/pickup.mp3");
	experimental::AudioEngine::preload("TileGameResources/kongbu.mp3");


    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	experimental::AudioEngine::play2d("TileGameResources/kongbu.mp3", true, 0.5);
}

void HelloWorld::setViewPointCenter(Point pos)
{

	auto screenSize = Director::getInstance()->getVisibleSize();
	//int x = MAX(pos.x, screenSize.width/2);
	//int y = MAX(pos.y, screenSize.height/2);

	//x = MIN(x, _tileMap->getMapSize().width * _tileMap->getTileSize().width - screenSize.width/2);
	//y = MIN(y, _tileMap->getMapSize().height * _tileMap->getTileSize().height - screenSize.height / 2);

	auto centerOfView = Point(screenSize.width / 2, screenSize.height / 2);
	auto viewPoint = centerOfView - pos;

	if (viewPoint.x < 0 || viewPoint.y < 0)
	{
		setPosition(viewPoint);
	}
	

	//auto worldPos = convertToWindowSpace(pos) - screenSize / 2;
	//if (abs(worldPos.x) > screenSize.width / 4 || abs(worldPos.y) > screenSize.height / 4)
	//{
	//	this->stopAllActions();
	//	this->runAction(MoveTo::create(0.2, viewPoint));
	//}

}
bool HelloWorld::colliade(cocos2d::Point pos)
{
	auto tilePos = tileCoordForPosition(pos);
	TMXTilePropertyMgr manager(_tileMap);
	if (Tuple_Second(manager.Boolean(tilePos, "Collidable")))
		return true;
	return false;
}


void HelloWorld::setPlayerPosition(Point pos)
{
	auto tilePos = tileCoordForPosition(pos);
	TMXTilePropertyMgr manager(_tileMap);
	//if (Tuple_Second(manager.Boolean(tilePos, "Collidable")))
	//	return;

	if (Tuple_Second(manager.Boolean(tilePos, "Eat")))
	{
		Eat(tilePos);
	}
		
	_sprite->setPosition(pos);
}



template<class V1, class V2, class V3>
V1 BOUND_T(V1 v1, V2 v2, V3 v3) {
	if (v2 < v1) 
		return v1; 
	else if (v3 < v2) 
		return v3; 
	else 
		return v2;
}


cocos2d::Point HelloWorld::tileCoordForPosition(Point pos)
{
	
	int x = pos.x / _tileMap->getTileSize().width;
	int y = (abs((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y)) / _tileMap->getTileSize().height;
	return Point(BOUND_T(0,x, _tileMap->getMapSize().width - 1), BOUND_T(0,y, _tileMap->getMapSize().height - 1));
}


void HelloWorld::update(float delt)
{
	auto velocity = RockManager::getInstance()->Velocity();
	if (!velocity.isZero())
	{
		velocity.normalize();
		auto pos = _sprite->getPosition() + velocity * 2;
		rotateHeader(pos);

		std::vector<Point> collideVec(4);
		collideVec.push_back(pos + Point(0, _sprite->getContentSize().width / 2));
		collideVec.push_back(pos + Point(0, -_sprite->getContentSize().width / 2));
		collideVec.push_back(pos + Point(_sprite->getContentSize().width / 2, 0));
		collideVec.push_back(pos + Point(-_sprite->getContentSize().width / 2, 0));
		for (auto p : collideVec)
		{
			if (colliade(p))
				return;
		}

		setPlayerPosition(pos);
		setViewPointCenter(pos);
	}

}

void HelloWorld::Eat(Point p)
{
	AudioEngine_play2d("TileGameResources/pickup.mp3");

	_tileMap->getLayer("Meta")->removeTileAt(p);
	_tileMap->getLayer("Foreground")->removeTileAt(p);
}

void HelloWorld::rotateHeader(cocos2d::Point p)
{
	float o = p.x - _sprite->getPosition().x;
	float a = p.y - _sprite->getPosition().y;
	float at = (float)CC_RADIANS_TO_DEGREES(atanf(o / a));

	if (a < 0)
	{
		if (o < 0)
			at = 180 + fabs(at);
		else
			at = 180 - fabs(at);
	}


	_sprite->setRotation(at);
}



//////////////////////////////////////////////////////////////////////////
TMXTilePropertyMgr::TMXTilePropertyMgr(cocos2d::experimental::TMXTiledMap *tileMap)
	: _tileMap(tileMap)
{

}



std::tuple<bool, bool> TMXTilePropertyMgr::Boolean(cocos2d::Point p, const std::string &property, const std::string layerName /*= "Meta"*/)
{
	ValueMap valueMap = propertys(p, layerName);

	bool ret = false;
	bool value = false;
	if (!valueMap.empty())
	{
		ret = true;
		value = valueMap[property].asBool();
	}

	return std::make_tuple(ret, value);
}

std::tuple<bool, std::string> TMXTilePropertyMgr::String(cocos2d::Point p, const std::string &property, const std::string layerName /*= "Meta"*/)
{
	ValueMap valueMap = propertys(p, layerName);

	bool ret = false;
	std::string value;
	if (!valueMap.empty())
	{
		value = valueMap[property].asString();
	}

	return make_tuple(ret, value);
}


ValueMap TMXTilePropertyMgr::propertys(cocos2d::Point p, const std::string layerName)
{
	auto layer = _tileMap->getLayer(layerName);
	CCASSERT(layer, "layer must not nullptr");

	auto gid = layer->getTileGIDAt(p);
	ValueMap valueMap;
	if (gid)
		valueMap = _tileMap->getPropertiesForGID(gid).asValueMap();
	
	return valueMap;
}


//////////////////////////////////////////////////////////////////////////
MainScene::MainScene()
	:Scene()
{

}

MainScene::~MainScene()
{

}

bool MainScene::init()
{
	if (!Scene::init())
		return false;

	auto backgroundLayer = HelloWorld::create();
	addChild(backgroundLayer);

	auto controlLayer = ControlLayer::create();
	addChild(controlLayer);

	return true;
}



cocos2d::Scene* TestScene::createScene()
{
	auto scene = Scene::create();
	scene->addChild(TestScene::create());

	return scene;
}


#include "ui/UISlider.h"
#include "ui/UICheckBox.h"
#include "ui/UIText.h"
using namespace cocos2d::ui;


bool TestScene::init()
{
	if (!LayerColor::init())
		return false;

	initWithColor(Color4B(250,250,0, 250));

	auto winSize = Director::getInstance()->getWinSize();
	
	auto slider = Slider::create();
	slider->setTag(110);
	slider->loadBarTexture("sliderTrack.png");
	slider->loadProgressBarTexture("slider02.png");
	slider->setScale9Enabled(true);
	slider->setContentSize(Size(500, 30));
	slider->setPosition(winSize / 2);

	//addChild(slider);


	//schedule([=](float){
	//
	//	static int percent = 0;
	//	auto slider = static_cast<Slider*>(getChildByTag(110));
	//	slider->setPercent(++percent);
	//	if (percent > 100)
	//		percent = 0;

	//}, "update");





	auto text = Text::create();
	text->setFontSize(22);
	text->setPosition(winSize / 2);
	
	addChild(text);

	

	auto checkBox = CheckBox::create("rock.png", "point.png");
	//checkBox->setContentSize(Size(50, 50));
	checkBox->setPosition(Vec2(0, 0));
	checkBox->addEventListener([=](Ref *, CheckBox::EventType type){
		switch (type)
		{
		case CheckBox::EventType::SELECTED:
			text->setString("Selected");
			break;
		case CheckBox::EventType::UNSELECTED:
			text->setString("Un Selected");
			break;
		}
	});
	addChild(checkBox);



	return true;
}

//////////////////////////////////////////////////////////////////////////

ControlLayer::ControlLayer()
	:Layer()
{

}

bool ControlLayer::init()
{
	if (!Layer::init())
		return false;

	_rockMgr = RockManager::createWithImage("rock.png", "point.png");
	_rockMgr->setAnchorPoint(Vec2(0.0f, 0.0f));
	_rockMgr->setPosition(0, 0);
	addChild(_rockMgr);

	return true;
}
