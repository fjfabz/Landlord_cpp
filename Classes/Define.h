#pragma once
#include "cocos2d.h"
USING_NS_CC;

#define CARDS_NUM 54	//牌的数目

//玩家
enum ERoleStatuEnum		//身份
{
	ERoleStatu_None,
	ERoleStatu_Farmer,	//农民
	ERoleStatu_Landlord	//地主
};

enum ERoleSideEnum		//方位
{
	ERoleSide_None,
	ERoleSide_Down,		//下面
	ERoleSide_Left,		//左边
	ERoleSide_Right		//右边
};

//单张牌
enum ECardHuaSeEnum		//花色
{
	ECardHS_None,
	ECardHS_FangPian,		//方片
	ECardHS_MeiHua,			//梅花
	ECardHS_HongXin,		//红心
	ECardHS_HeiTao,			//黑桃
	ECardHS_Wang			//王
};

enum ECardPointEnum		//点数
{
	ECardPoint_None,
	ECardPoint_3,		//3点
	ECardPoint_4,		//4点
	ECardPoint_5,		//5点
	ECardPoint_6,		//6点
	ECardPoint_7,		//7点
	ECardPoint_8,		//8点
	ECardPoint_9,		//9点
	ECardPoint_10,		//10点
	ECardPoint_J,		//J点
	ECardPoint_Q,		//Q点
	ECardPoint_K,		//K点
	ECardPoint_A,		//A点
	ECardPoint_2,		//2点
	ECardPoint_S,		//小王
	ECardPoint_B		//大王
};

//出的牌的信息
enum ECardsKindEnum			//玩家牌的类型
{
	ECardsKind_None,		//错误的牌型
	ECardsKind_One,			//个子
	ECardsKind_Two,			//对子
	ECardsKind_Three,		//三不带
	ECardsKind_Three_One,	//三带一
	ECardsKind_Three_Two,	//三代二
	ECardsKind_King_Bomb,	//王炸
	ECardsKind_Bomb,		//炸弹
	ECardsKind_Four_One,	//四带两个
	ECardsKind_Four_Two,	//四带两对
	ECardsKind_Plane,		//飞机不带
	ECardsKind_Plane_One,	//飞机带两个
	ECardsKind_Plane_Two,	//飞机带两对
	ECardsKind_Follow_One,	//顺子
	ECardsKind_Follow_Two	//连对
};

struct SCardsNum							//该种牌的数目
{
	ECardPointEnum ePoint = ECardPoint_None;//点数
	int nNum = 0;							//数目
	bool operator==(const SCardsNum &pointNum) { return ePoint == pointNum.ePoint; }
};

struct SCardsInfo							//出的牌的所有信息
{
	ECardsKindEnum eKind = ECardsKind_None;	//牌的类型
	ECardPointEnum ePoint = ECardPoint_None;//牌的最大特征点的点数
	int nLong = 0;							//牌的大特征点的数目
};
