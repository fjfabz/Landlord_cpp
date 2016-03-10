#include "LoadingScene.h"
#include "GameScene.h"

Scene* LoadingScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LoadingScene::create();
    scene->addChild(layer);

    return scene;
}

bool LoadingScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
	
	//背景
	auto sprBG = Sprite::create("image/bg_loading.png");
	sprBG->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(sprBG, 0);

	//标题
	auto label = Label::createWithTTF("LoadingScene", "fonts/Marker Felt.ttf", 24);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - label->getContentSize().height));
	this->addChild(label, 1);
	
	//加载资源
	Director::getInstance()->getTextureCache()->addImageAsync("image/ui.png", CC_CALLBACK_1(LoadingScene::LoadResource, this));
    
    return true;
}

void LoadingScene::LoadResource(Texture2D* texture)
{
	//切换场景
	ReplaceScene();
}

void LoadingScene::ReplaceScene()
{
	auto gameScene = GameScene::createScene();
	TransitionCrossFade* tranScene = TransitionCrossFade::create(0.5f, gameScene);
	Director::getInstance()->replaceScene(tranScene);
}
