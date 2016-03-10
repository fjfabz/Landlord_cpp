#pragma once
#include "cocos2d.h"
#include "Define.h"
#include "Card.h"
#include <algorithm>
USING_NS_CC;
using namespace std;

class Player : public Node
{
public:
	Player();
	~Player();
	static Player* create(ERoleSideEnum side);
	bool init(ERoleSideEnum eSide);
	void setPutEnable(std::function<void(bool)> cb) { CB_Put_Enable = cb; }
	CC_SYNTHESIZE(int, playID, PlayID);
	void setPlayPoints(std::vector<int> vecPoints) { vecPoints = vecPoints; }
	void addCard(Sprite* cardBack);
	void changePutCard(int cardPoint);//添加/删除玩家选中的牌
	void setRole(ERoleStatuEnum role);
	ERoleStatuEnum getRole() { return m_eStatu; }
	void addLandCards(std::vector<int> vecCards);
	std::vector<int> getPutPoints();
	void showSelfCards();
	void showOtherCards(std::vector<int> vecCards);
	void removeShowCards();
	void setComparePoints(std::vector<int> vecPoints);
	SCardsInfo getCardsInfo(std::vector<int> vecPoints);
	bool judgePutBtns();
	void sortPlayCards();
	static bool sortCards(Card* card1, Card* card2) { return card1->getPoint() > card2->getPoint(); }
	static bool sortCardPoints(int point1, int point2) { return point1 > point2; }
	static bool sortPointEnumNum(SCardsNum pointEnumNum1, SCardsNum pointEnumNum2) { return pointEnumNum1.nNum > pointEnumNum2.nNum; }
private:
	ERoleSideEnum m_eSide;		//方位
	ERoleStatuEnum m_eStatu;	//身份
	Size m_Size;
	Sprite* m_sprAvatar;		//头像
	std::function<void(bool)> CB_Put_Enable;
	std::vector<Card*> vecFrontCards;//正面牌(下)
	std::vector<Sprite*> vecBackCards;//背面牌(左,右)
	std::vector<Sprite*> vecShowCards;//展示牌(下)
	std::vector<int> vecPoints;//发给玩家牌的点数
	std::vector<int> vecPutPoints;//玩家出牌的点数
	SCardsInfo otherCardsInfo;//比较牌的信息
};
