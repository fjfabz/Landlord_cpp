#pragma once
#include "Define.h"

class Card : public Sprite
{
public:
	Card();
	~Card();
	static Card* create(int nPoint);
	bool init(int nPoint);
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);

	int getPoint() { return m_nPoint; }
	void setChangePut(std::function<void(int)> cb) { m_CB_ChangePut = cb; }
	void setSelectState(bool bSelect);
	bool getSelectState() { return m_bSelect; }
private:
	int m_nPoint;
	ECardPointEnum m_ePoint;
	ECardHuaSeEnum m_eHuaSe;
	Rect m_rectCard;
	std::function<void(int)> m_CB_ChangePut;//¸Ä±äÅÆµÄ×´Ì¬
	bool m_bSelect;
};
