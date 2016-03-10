#include "Player.h"

Player::Player()
{
	m_eStatu = ERoleStatu_None;
	m_eSide = ERoleSide_None;
	m_sprAvatar = nullptr;
	vecFrontCards.clear();
	vecBackCards.clear();
	vecPoints.clear();
	vecPutPoints.clear();
	playID = -1;
}

Player::~Player()
{

}

Player* Player::create(ERoleSideEnum eSide)
{
	Player *pRet = new(std::nothrow) Player();
	if (pRet && pRet->init(eSide))
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

bool Player::init(ERoleSideEnum eSide)
{
	m_eSide = eSide;
	if (m_eSide == ERoleSide_None)
	{
		return false;
	}
	m_Size = Director::getInstance()->getVisibleSize();
	
	//头像
	m_sprAvatar = Sprite::create("image/avatar_nan.png");
	if (m_sprAvatar == nullptr)
	{
		return false;
	}
	Vec2 pos;
	switch (m_eSide)
	{
	case ERoleSide_Down:
		pos = Vec2(m_sprAvatar->getContentSize().width / 2, m_Size.height / 2 - m_sprAvatar->getContentSize().height / 2);
		break;
	case ERoleSide_Left:
		pos = Vec2(m_sprAvatar->getContentSize().width / 2, m_Size.height - m_sprAvatar->getContentSize().height / 2);
		break;
	case ERoleSide_Right:
		pos = m_Size - m_sprAvatar->getContentSize() / 2;
		break;
	default:
		pos = Vec2::ZERO;
		break;
	}
	m_sprAvatar->setPosition(pos);
	this->addChild(m_sprAvatar);

	return true;
}

void Player::addCard(Sprite* sprCardBack)
{
	if (sprCardBack == nullptr)
	{
		return;
	}

	if (m_eSide == ERoleSide_Down)
	{
		sprCardBack->removeFromParent();
		if (vecPoints.size())
		{
			Card* frontCard = Card::create(vecPoints[vecFrontCards.size()]);
			frontCard->setChangePut(CC_CALLBACK_1(Player::changePutCard, this));
			this->addChild(frontCard);
			vecFrontCards.push_back(frontCard);

			sortPlayCards();
		}
	}
	else
	{
		int cardsNum = vecBackCards.size();
		float fPosX = m_eSide == ERoleSide_Left ? 250 : m_Size.width - 250;
		sprCardBack->setPosition(Vec2(fPosX, m_Size.height - 20 * cardsNum - 70));
		sprCardBack->setZOrder(cardsNum);
		vecBackCards.push_back(sprCardBack);
	}
}

void Player::sortPlayCards()
{
	std::sort(vecFrontCards.begin(), vecFrontCards.end(), &Player::sortCards);
	for (int i = 0, cardsNum = vecFrontCards.size(); i < cardsNum; i++)
	{
		Card* cardFront = vecFrontCards[i];
		Vec2 pos;
		pos.x = 25.f * (2 * i - cardsNum + 1) + m_Size.width / 2;
		pos.y = cardFront->getContentSize().height / 2;
		if (cardFront->getSelectState())
		{
			pos.y += 50.f;
		}
		cardFront->setPosition(pos);
		cardFront->setZOrder(i);
	}
}

void Player::setRole(ERoleStatuEnum role)
{
	m_eStatu = role;
	switch (m_eStatu)
	{
	case ERoleStatu_Landlord:
		m_sprAvatar->initWithFile("image/avatar_nan_landlord.png");
		break;
	case ERoleStatu_Farmer:
		m_sprAvatar->initWithFile("image/avatar_nan_farmer.png");
		break;
	default:
		break;
	}
}

void Player::addLandCards(std::vector<int> vecCards)
{
	for (size_t i = 0; i < 3; i++)
	{
		Card* frontCard = Card::create(vecCards[i]);
		frontCard->setChangePut(CC_CALLBACK_1(Player::changePutCard, this));
		this->addChild(frontCard);
		vecFrontCards.push_back(frontCard);
		frontCard->setSelectState(true);//地主牌选中
	}
	sortPlayCards();
}

void Player::changePutCard(int cardPoint)
{
	std::vector<int>::iterator itr = std::find(vecPutPoints.begin(), vecPutPoints.end(), cardPoint);
	if (itr == vecPutPoints.end())
	{
		vecPutPoints.push_back(cardPoint);
	}
	else
	{
		vecPutPoints.erase(itr);
	}
	//判断是否能出牌
	CB_Put_Enable(judgePutBtns());//1.出牌正确 2.自己开牌或比别人大 TODO
}

bool Player::judgePutBtns()
{
	SCardsInfo putCardInfo = getCardsInfo(vecPutPoints);
	if (putCardInfo.eKind != ECardsKind_None)
	{
		if (otherCardsInfo.eKind == ECardsKind_None)
		{
			return true;
		}
		else if (putCardInfo.eKind == otherCardsInfo.eKind && putCardInfo.nLong == otherCardsInfo.nLong)
		{
			return putCardInfo.ePoint > otherCardsInfo.ePoint;
		}
	}

	return false;
}

void Player::setComparePoints(std::vector<int> vecPoints)
{
	otherCardsInfo = getCardsInfo(vecPoints);
}

std::vector<int> Player::getPutPoints()
{
	std::sort(vecPutPoints.begin(), vecPutPoints.end(), &Player::sortCardPoints);
	return vecPutPoints;
}

void Player::removeShowCards()//移除自己的展示牌
{
	for (auto showCard : vecShowCards)
	{
		showCard->removeFromParent();
	}
	vecShowCards.clear();
}

void Player::showOtherCards(std::vector<int> vecCards)//left right调用
{
	if (m_eSide == ERoleSide_Down)
	{
		return;
	}
	removeShowCards();
	//1.添加左/右方展示牌
	int cardsNum = vecCards.size();
	if (cardsNum == 0)
	{
		return;
	}
	float fPosX = m_eSide == ERoleSide_Left ? 350 : m_Size.width - 350;
	for (int i = 0; i < cardsNum; i++)
	{
		String* StrShow = String::createWithFormat("image/%d.png", vecCards[i]);
		Sprite* showCard = Sprite::create(StrShow->getCString());
		showCard->setScale(0.5f);
		showCard->setPosition(Vec2(fPosX, m_Size.height - 20 * i - 70));
		this->addChild(showCard);
		vecShowCards.push_back(showCard);
	}
	//2.移除左/右方展示牌
	for (int i = 0; i < cardsNum; i++)
	{
		vecBackCards.back()->removeFromParent();
		vecBackCards.pop_back();
	}
}

void Player::showSelfCards()//self调用
{
	if (m_eSide != ERoleSide_Down)
	{
		return;
	}
	removeShowCards();
	//1.添加上方牌
	int cardsNum = vecPutPoints.size();
	if (cardsNum == 0)
	{
		return;
	}
	for (int i = 0; i < cardsNum; i++)
	{
		String* StrShow = String::createWithFormat("image/%d.png", vecPutPoints[i]);
		Sprite* showCard = Sprite::create(StrShow->getCString());
		showCard->setScale(0.5f);
		float fPosX = 20.f * (2 * i - cardsNum + 1) + m_Size.width / 2;
		showCard->setPosition(Vec2(fPosX, showCard->getContentSize().height * 1.5f));
		this->addChild(showCard);
		vecShowCards.push_back(showCard);
	}
	//2.移除下方牌
	std::vector<int>::iterator itrPut = vecPutPoints.begin();
	for (std::vector<Card*>::iterator itr = vecFrontCards.begin(); itr != vecFrontCards.begin();)
	{
		if ((*itr)->getPoint() == (*itrPut))
		{
			(*itr)->removeFromParent();
			vecFrontCards.erase(itr);
			itrPut = vecPutPoints.erase(itrPut);
			if (itrPut == vecPutPoints.begin())
			{
				break;
			}
		}
	}
	sortPlayCards();
}

SCardsInfo Player::getCardsInfo(std::vector<int> vecPoints)
{
	int nCardsNum = vecPoints.size();//牌的数目
	SCardsInfo cardsInfo;
	if (nCardsNum <= 0)
	{
		return cardsInfo;
	}

	std::vector<ECardPointEnum> vecPointEnums;//转换成点数
	std::vector<SCardsNum> vecPointEnumsNums;//该点数的数目
	for (auto point : vecPoints)
	{
		ECardPointEnum ePoint = (ECardPointEnum)(point / 4 + 1);
		vecPointEnums.push_back(ePoint);

		SCardsNum pointEnumsNum;
		pointEnumsNum.ePoint = ePoint;

		std::vector<SCardsNum>::iterator itr = std::find(vecPointEnumsNums.begin(), vecPointEnumsNums.end(), pointEnumsNum);
		if (itr != vecPointEnumsNums.end())
		{
			itr->nNum += 1;
		}
		else
		{
			pointEnumsNum.nNum = 1;
			vecPointEnumsNums.push_back(pointEnumsNum);
		}
	}
	//将数目多的放在前面
	std::sort(vecPointEnumsNums.begin(), vecPointEnumsNums.end(), &Player::sortPointEnumNum);

	ECardsKindEnum cardKind = ECardsKind_None;//牌的种类
	int nLength = 1;//牌的长度 比如是2飞机 还是3飞机

	int nCardsKind = vecPointEnumsNums.size();//牌的种类数目
	if (vecPointEnumsNums[0].ePoint - vecPointEnumsNums[nCardsKind - 1].ePoint == (nCardsKind - 1) && vecPointEnumsNums[0].nNum == vecPointEnumsNums[nCardsKind - 1].nNum)//相同数量且连续的
	{
		if (vecPointEnumsNums[0].nNum == 1)
		{
			if (nCardsNum == 1)
			{
				cardKind = ECardsKind_One;
			}
			else if (nCardsNum > 4 && vecPointEnums[0] < ECardPoint_2)
			{
				cardKind = ECardsKind_Follow_One;
			}
		}
		else if (vecPointEnumsNums[0].nNum == 2)
		{
			if (nCardsNum == 2)
			{
				if (vecPointEnumsNums[0].ePoint == ECardPoint_S)
				{
					cardKind = ECardsKind_Bomb;
				}
				else
				{
					cardKind = ECardsKind_Two;
				}
			}
			else if (nCardsNum > 5 && vecPointEnums[0] < ECardPoint_2)
			{
				cardKind = ECardsKind_Follow_Two;
			}
		}
		else if (vecPointEnumsNums[0].nNum == 3)
		{
			if (nCardsKind == 1)
			{
				cardKind = ECardsKind_Three;
			}
			else
			{
				cardKind = ECardsKind_Plane;
			}
		}
		else if (vecPointEnumsNums[0].nNum == 4 && nCardsNum == 4)
		{
			cardKind = ECardsKind_Bomb;
		}
		nLength = nCardsKind;
	}
	else if (vecPointEnumsNums[0].nNum == 3 && vecPointEnumsNums[nCardsKind / 2 - 1].nNum == 3 &&
		vecPointEnumsNums[0].ePoint - vecPointEnumsNums[nCardsKind / 2 - 1].ePoint == (nCardsKind / 2 - 1) &&
		nCardsKind % 2 == 0)//不可能出现3带3 上面已排除
	{
		if (nCardsNum == nCardsKind * 2 && vecPointEnumsNums[nCardsKind / 2].nNum == 1)
		{
			if (nCardsKind == 2)
			{
				cardKind = ECardsKind_Three_One;
			}
			else
			{
				cardKind = ECardsKind_Plane_One;
			}
		}
		else if (nCardsNum == nCardsKind * 5 / 2 && vecPointEnumsNums[nCardsKind / 2].nNum == 2 && vecPointEnumsNums[nCardsKind - 1].nNum == 2)
		{
			if (nCardsKind == 2)
			{
				cardKind = ECardsKind_Three_Two;
			}
			else
			{
				cardKind = ECardsKind_Plane_Two;
			}
		}
		nLength = nCardsKind / 2;
	}
	else if (vecPointEnumsNums[0].nNum == 4)//不可能出现炸弹 上面已排除
	{
		if (nCardsNum == 6)
		{
			cardKind = ECardsKind_Four_One;
		}
		else if (nCardsNum == 8 && nCardsKind == 3 && vecPointEnumsNums[1].nNum == 2)
		{
			cardKind = ECardsKind_Four_Two;
		}
	}

	cardsInfo.eKind = cardKind;
	cardsInfo.ePoint = vecPointEnumsNums[0].ePoint;
	cardsInfo.nLong = nLength;

	return cardsInfo;
}
