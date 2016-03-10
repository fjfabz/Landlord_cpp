#pragma once
#include "cocos2d.h"
#include "Define.h"
#include "Card.h"
USING_NS_CC;

class PutCard : public Card
{
public:
	PutCard();
	~PutCard();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);

	void setChangePut(std::function<void(int)> cb) { CB_Change_Put = cb; }
	void setSelectState(bool bSelect);
	bool getSelectState() { return m_bSelect; }
private:
	Rect m_RectCard;//�ƵĴ�С��Χ
	bool m_bSelect;//�Ƿ�ѡ��
	std::function<void(int)> CB_Change_Put;	
};
