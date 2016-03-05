#include "GameScene.h"

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
	
	//标题
	auto label = Label::createWithTTF("GameScene", "fonts/Marker Felt.ttf", 24);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - label->getContentSize().height));
	this->addChild(label, 1);
	
	//背景
	auto sprBG = Sprite::create("image/desktop_bg.png");
	sprBG->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(sprBG, 0);
	
	//退出按钮
	auto exitItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(GameScene::OnExitGame, this));
	exitItem->setPosition(Vec2(visibleSize.width - exitItem->getContentSize().width / 2, exitItem->getContentSize().height / 2));
    auto menu = Menu::create(exitItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    return true;
}

void GameScene::OnExitGame(Ref* pSender)
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
