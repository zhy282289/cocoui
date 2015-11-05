#include "StartScene.h"


#include "public.h"



StartScene::StartScene()
	:Layer()
{
}


StartScene::~StartScene()
{
}

cocos2d::Scene * StartScene::createScene()
{
	auto scene = Scene::create();
	scene->addChild(StartScene::create());
	return scene;
}

bool StartScene::init()
{
	if (!Layer::init())
		return false;

	auto visibleSize = VisibleRect::getVisibleSize();

	auto bg = Sprite::create("start_bg.png");
	bg->setAnchorPoint(Vec2(0.5f, 0.5f));
	bg->setPosition(visibleSize / 2);
	addChild(bg, -100);

	auto layout = Layout::create();
	layout->setLayoutType(Layout::Type::VERTICAL);
	layout->setContentSize(visibleSize);
	layout->setAnchorPoint(Vec2(0.5f, 1.2f));
	layout->setPosition(visibleSize / 2);
	addChild(layout);

	
	auto *l = LinearLayoutParameter::create();
	l->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	l->setMargin(Margin(0, 10, 0, 20));

	auto startBtn = Button::create("btn-play-normal.png", "btn-play-selected.png");
	layout->addChild(startBtn);
	startBtn->setLayoutParameter(l);
	
	auto settingBtn = Button::create("btn-about-normal.png", "btn-about-selected.png");
	settingBtn->setLayoutParameter(l->clone());
	layout->addChild(settingBtn);


	return true;
}

void StartScene::onEnter()
{
	Layer::onEnter();

	std::string audioBG("start_bg.ogg");
	BOYAudioEngine::preload(audioBG, [=](bool isSuccess){
		BOYAudioEngine::play2d_backgound(audioBG, true);
	});

}
