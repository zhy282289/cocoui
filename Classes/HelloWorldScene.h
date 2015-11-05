#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void onEnter() override;
	void update(float delt) override;

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void setViewPointCenter(cocos2d::Point pos);
	void setPlayerPosition(cocos2d::Point pos);

	cocos2d::Point tileCoordForPosition(cocos2d::Point pos);
	bool colliade(cocos2d::Point pos);

	void rotateHeader(cocos2d::Point p);

	void Eat(cocos2d::Point p);
private:
	cocos2d::experimental::TMXTiledMap *_tileMap;
	cocos2d::TMXLayer *_layer;
	cocos2d::Sprite *_sprite;
};


#define Tuple_First(t) std::get<0>(t)
#define Tuple_Second(t) std::get<1>(t)

class TMXTilePropertyMgr
{
public:
	TMXTilePropertyMgr(cocos2d::experimental::TMXTiledMap *tileMap);

	std::tuple<bool, bool> Boolean(cocos2d::Point p, const std::string &property, const std::string layerName = "Meta");
	std::tuple<bool, std::string> String(cocos2d::Point p, const std::string &property, const std::string layerName = "Meta");

	cocos2d::ValueMap propertys(cocos2d::Point p, const std::string layerName = "Meta");
private:
	

	cocos2d::experimental::TMXTiledMap *_tileMap;
};






class MainScene : public cocos2d::Scene
{
public:
	
	bool init() override;

	CREATE_FUNC(MainScene);
	MainScene();
	~MainScene();
};



class TestScene : public cocos2d::LayerColor
{
public:
	CREATE_FUNC(TestScene);

	static cocos2d::Scene* createScene();

	bool init() override;


};


class RockManager;
class ControlLayer : public cocos2d::Layer
{
public:

	bool init() override;

	CREATE_FUNC(ControlLayer);

	ControlLayer();

private:
	RockManager *_rockMgr;
};

#endif // __HELLOWORLD_SCENE_H__
