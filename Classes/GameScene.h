#pragma once
#include "cocos2d.h"
USING_NS_CC;

class GameScene : public Layer
{
public:
    static Scene* createScene();
	CREATE_FUNC(GameScene);
    virtual bool init();
    
    void OnExitGame(Ref* pSender);//退出游戏
};
