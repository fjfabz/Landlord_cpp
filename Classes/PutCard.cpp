#include "PutCard.h"

PutCard::PutCard() :m_bSelect(false)
{
	m_bCanPut = true;
}

PutCard::~PutCard()
{

}

void PutCard::onEnter()
{
	Sprite::onEnter();

	Size size = this->getContentSize();
	m_RectCard = Rect(-size.width / 2, -size.height / 2, size.width, size.height);

	EventListenerTouchOneByOne* touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PutCard::onTouchBegan, this);
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void PutCard::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	Sprite::onExit();
}

bool PutCard::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPos = convertTouchToNodeSpaceAR(pTouch);
	if (m_RectCard.containsPoint(touchPos))
	{
		setSelectState(!m_bSelect);
		return true;
	}
	return false;
}

void PutCard::setSelectState(bool bSelect)
{
	m_bSelect = bSelect;

	this->setPositionY(getPositionY() + (m_bSelect ? 25.f : -25.f));

	CB_Change_Put(m_nPoint);
}
