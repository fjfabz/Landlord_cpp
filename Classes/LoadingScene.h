#pragma once
#include "cocos2d.h"
USING_NS_CC;

class LoadingScene : public Layer
{
public:
    static Scene* createScene();
	CREATE_FUNC(LoadingScene);
    virtual bool init();
    
	void LoadResource(Texture2D* texture);//加载资源
	void ReplaceScene();//切换场景
};
