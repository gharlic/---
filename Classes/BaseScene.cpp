/*유니코드 저장을 위한 더미 한글입니다 지우지 마세요*/
#include "BaseScene.h"

USING_NS_CC;

Scene* BaseScene::createScene()
{
	auto scene = Scene::create();
	auto layer = BaseScene::create();

	scene->addChild(layer);

	return scene;
}

bool BaseScene::init()
{
	if (!Layer::init()) return false;

	return true;
}


void BaseScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	//Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}