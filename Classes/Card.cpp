#include "Card.h"

Card::Card()
{
	m_nPoint = -1;
	m_ePoint = ECardPoint_None;
	m_eHuaSe = ECardHS_None;
	m_bSelect = false;
	CB_Change_Put = nullptr;
}

Card::~Card()
{
	if (_eventDispatcher != nullptr)
	{
		_eventDispatcher->removeEventListenersForTarget(this);
	}
}

Card* Card::create(int nPoint)
{
	Card *pRet = new(std::nothrow) Card();
	if (pRet && pRet->init(nPoint))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Card::init(int nPoint)
{
	m_nPoint = nPoint;

	String* str_point = String::createWithFormat("image/%d.png", m_nPoint);
	if (str_point == nullptr)
	{
		return false;
	}
	initWithFile(str_point->getCString());

	//´¥Ãþ¼àÌý
	EventListenerTouchOneByOne* touchListener = EventListenerTouchOneByOne::create();
	if (touchListener == nullptr || _eventDispatcher == nullptr)
	{
		return false;
	}
	touchListener->onTouchBegan = CC_CALLBACK_2(Card::onTouchBegan, this);
	touchListener->setSwallowTouches(true);//²»ÏòÏÂ´«µÝ´¥Ãþ
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	m_ePoint = (ECardPointEnum)(m_nPoint / 4 + 1);
	if (m_ePoint == ECardPoint_S)
	{
		m_ePoint = m_nPoint % 4 ? ECardPoint_B : ECardPoint_S;
		m_eHuaSe = ECardHS_Wang;
	}
	else
	{
		m_eHuaSe = (ECardHuaSeEnum)(m_nPoint % 4 + 1);
	}

	Size size = this->getContentSize();
	m_rectCard = Rect(-size.width / 2, -size.height / 2, size.width, size.height);//´¥ÃþÇøÓò

	return true;
}

bool Card::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if (pTouch == nullptr || pEvent == nullptr)
	{
		return false;
	}

	if (m_rectCard.containsPoint(convertTouchToNodeSpaceAR(pTouch)))
	{
		setSelectState(!m_bSelect);
		return true;
	}
	return false;
}

void Card::setSelectState(bool bSelect)
{
	m_bSelect = bSelect;
	this->setPositionY(getPositionY() + (bSelect ? 25.f : -25.f));
	//m_CB_ChangePut(this->m_nPoint);
}
