#pragma once
#include "cocos2d.h"
USING_NS_CC;

class HelloWorld : public Layer
{
public:
    static Scene* createScene();
	CREATE_FUNC(HelloWorld);
    virtual bool init();
    
    void menuCloseCallback(Ref* pSender);
};
