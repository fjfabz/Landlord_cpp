#include "GameScene.h"

GameScene::GameScene()
{
	cardsNum = CARDS_NUM;
	startItem = nullptr;
	callLandItem = nullptr;
	netWorkItem = nullptr;
	noCallItem = nullptr;
	robLandItem = nullptr;
	noRobItem = nullptr;
	putItem = nullptr;
	noPutItem = nullptr;
	playerSelf = nullptr;
	playerLeft = nullptr;
	playerRight = nullptr;
	vecAllCardBacks.clear();
}

GameScene::~GameScene()
{

}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	//背景
	auto sprBG = Sprite::create("image/bg_desktop.png");
	sprBG->setPosition(visibleSize / 2);
	this->addChild(sprBG);

	playerSelf = Player::create(ERoleSide_Down);
	//playerSelf->setPutEnable(CC_CALLBACK_1(GameScene::setPutEnable, this));
	this->addChild(playerSelf);

	//createButtons();

	Card* card = Card::create(1);
	card->setPosition(visibleSize / 2);
	this->addChild(card);

	Card* card2 = Card::create(1);
	card2->setPosition(Vec2(visibleSize.width / 2 + 20.f, visibleSize.height / 2));
	this->addChild(card2);

	return true;
}

void GameScene::onEnter()
{
	Layer::onEnter();
	//connServer();
}

void GameScene::onExit()
{
	//closeConn();
}

void GameScene::createButtons()
{
	//联网
	netWorkItem = MenuItemImage::create("image/btn_i_know_1.png", "image/btn_i_know_2.png", CC_CALLBACK_1(GameScene::onNetwork, this));
	netWorkItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - netWorkItem->getContentSize().height));
	netWorkItem->setVisible(false);
	//开始游戏
	startItem = MenuItemImage::create("image/btn_start_app.png", "image/btn_start_app2.png", CC_CALLBACK_1(GameScene::onStartGame, this));
	startItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - startItem->getContentSize().height));
	startItem->setVisible(false);
	//叫地主
	callLandItem = MenuItemImage::create("image/btn_jiaodizhu_up.png", "image/btn_jiaodizhu_down.png", CC_CALLBACK_1(GameScene::onCallLand, this));
	callLandItem->setPosition(Vec2(visibleSize.width / 2 - 150, visibleSize.height / 2 - callLandItem->getContentSize().height));
	callLandItem->setVisible(false);
	//不叫
	noCallItem = MenuItemImage::create("image/btn_bujiao.png", "image/btn_bujiao_down.png", CC_CALLBACK_1(GameScene::onNoCall, this));
	noCallItem->setPosition(Vec2(visibleSize.width / 2 + 150, visibleSize.height / 2 - noCallItem->getContentSize().height));
	noCallItem->setVisible(false);
	//抢地主
	robLandItem = MenuItemImage::create("image/btn_qiangdizhu_up.png", "image/btn_qiangdizhu_down.png", CC_CALLBACK_1(GameScene::onRobLand, this));
	robLandItem->setPosition(Vec2(visibleSize.width / 2 - 150, visibleSize.height / 2 - robLandItem->getContentSize().height));
	robLandItem->setVisible(false);
	//不抢
	noRobItem = MenuItemImage::create("image/btn_buqiang.png", "image/btn_buqiang_down.png", CC_CALLBACK_1(GameScene::onNoRob, this));
	noRobItem->setPosition(Vec2(visibleSize.width / 2 + 150, visibleSize.height / 2 - noRobItem->getContentSize().height));
	noRobItem->setVisible(false);
	//出牌
	putItem = MenuItemImage::create("image/btn_chupai.png", "image/btn_chupai_down.png", "image/btn_chupai_hui.png", CC_CALLBACK_1(GameScene::onPut, this));
	putItem->setPosition(Vec2(visibleSize.width / 2 - 150, visibleSize.height / 2 - putItem->getContentSize().height));
	putItem->setVisible(false);
	//不出
	noPutItem = MenuItemImage::create("image/btn_bujiao2.png", "image/btn_bujiao2_down.png", CC_CALLBACK_1(GameScene::onNoPut, this));
	noPutItem->setPosition(Vec2(visibleSize.width / 2 + 150, visibleSize.height / 2 - noPutItem->getContentSize().height));
	noPutItem->setVisible(false);

	auto menu = Menu::create(netWorkItem, startItem, callLandItem, noCallItem, robLandItem, noRobItem, putItem, noPutItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 100);
}

void GameScene::onNetwork(Ref* pSender)
{
	std::thread recvThread(&GameScene::connThread, this);
	recvThread.detach();
}

void GameScene::onStartGame(Ref* pSender)
{
	startItem->setVisible(false);
	sendServer("StartGame:");
}

void GameScene::onCallLand(Ref* pSender)
{
	callLandItem->setVisible(false);
	noCallItem->setVisible(false);

	sendServer("CallLand:");
}

void GameScene::onNoCall(Ref* pSender)
{
	callLandItem->setVisible(false);
	noCallItem->setVisible(false);

	sendServer("NoCall:");
}

void GameScene::onRobLand(Ref* pSender)
{
	robLandItem->setVisible(false);
	noRobItem->setVisible(false);

	sendServer("RobLand:");
}

void GameScene::onNoRob(Ref* pSender)
{
	robLandItem->setVisible(false);
	noRobItem->setVisible(false);

	sendServer("NoRob:");
}

void GameScene::onPut(Ref* pSender)//出牌 TODO
{
	putItem->setVisible(false);
	noPutItem->setVisible(false);

	playerSelf->showSelfCards();
	sendServer("Put:" + vectorTostring(playerSelf->getPutPoints()));
}

void GameScene::onNoPut(Ref* pSender)
{
	putItem->setVisible(false);
	noPutItem->setVisible(false);

	sendServer("NoPut:");
}

void GameScene::connServer()
{
	//WinSock初始化
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		CCLOG("WSAStartup fail");
		return;
	}
	//确认WinSock DLL支持版本2.2  
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		CCLOG("WinSock DLL Version Invalid");
		return;
	}
	//创建Socket,使用TCP协议
	sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockClient == INVALID_SOCKET)
	{
		closeConn();
		CCLOG("socket fail");
		return;
	}
	//服务器地址
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, "127.0.0.1", &addrClient.sin_addr.S_un.S_addr);
	//连接服务器
	std::thread recvThread(&GameScene::connThread, this);
	recvThread.detach();
}

void GameScene::connThread()
{
	if (SOCKET_ERROR == connect(sockClient, (SOCKADDR*)&addrClient, sizeof(SOCKADDR)))
	{
		netWorkItem->setVisible(true);
		CCLOG("connect fail");
		return;
	}
	netWorkItem->setVisible(false);
	startItem->setVisible(true);

	//连接消息
	std::thread recvThread(&GameScene::recvThread, this);
	recvThread.detach();
}

void GameScene::closeConn()
{
	closesocket(sockClient);
	WSACleanup();
}

void GameScene::recvThread()
{
	char buff[100];//最大100
	int pos;
	while (true)
	{
		recv(sockClient, buff, 100, 0);
		std::string strBuff = buff;
		CCLOG("%s", strBuff.c_str());

		pos = 0;
		std::string strFront = strBuff.substr(pos, strBuff.find(":"));
		if (strFront == "SelfPlayID")//数据类型 SelfPlayID:0
		{
			int playID = std::stoi(getStringByPos(strBuff, pos));
			playerSelf->setPlayID(playID);
			mapPlayer[playID] = playerSelf;

			if (playID > 0)
			{
				playerLeft = Player::create(ERoleSide_Left);
				this->addChild(playerLeft);
				playerLeft->setPlayID((playID + 2) % 3);

				mapPlayer[playerLeft->getPlayID()] = playerLeft;
			}
			if (playID > 1)
			{
				playerRight = Player::create(ERoleSide_Right);
				this->addChild(playerRight);
				playerRight->setPlayID((playID + 1) % 3);

				mapPlayer[playerRight->getPlayID()] = playerRight;
			}
		}
		else if (strFront == "AddPlay")//数据类型 AddPlay:
		{
			if (playerRight)
			{
				playerLeft = Player::create(ERoleSide_Left);
				this->addChild(playerLeft);
				playerLeft->setPlayID((playerSelf->getPlayID() + 2) % 3);

				mapPlayer[playerLeft->getPlayID()] = playerLeft;
			}
			else
			{
				playerRight = Player::create(ERoleSide_Right);
				this->addChild(playerRight);
				playerRight->setPlayID((playerSelf->getPlayID() + 1) % 3);

				mapPlayer[playerRight->getPlayID()] = playerRight;
			}
		}
		else if (strFront == "SelfCards")//告诉你的牌 数据类型 SelfCards:1,2,3
		{
			playerSelf->setPlayPoints(stringTovector(getStringByPos(strBuff, pos)));
		}
		else if (strFront == "DealCards")//发牌  数据类型 DealCards:
		{
			createAllCards();
			dealCards();
		}
		else if (strFront == "ChooseCallLand")//叫地主  数据类型 ChooseCallLand:
		{
			showChooseCallButtons();
		}
		else if (strFront == "ChooseRobLand")//抢地主  数据类型 ChooseRobLand:
		{
			showChooseRobButtons();
		}
		else if (strFront == "ReStart")//重新开始 现在不考虑
		{

		}
		else if (strFront == "LandLordID")//地主是谁  数据类型 LandLordID:1
		{
			int landLordID = std::stoi(getStringByPos(strBuff, pos));
			for (int i = 0; i < mapPlayer.size(); i++)
			{
				if (i == landLordID)
				{
					mapPlayer[i]->setRole(ERoleStatu_Landlord);
				}
				else
				{
					mapPlayer[i]->setRole(ERoleStatu_Farmer);
				}
			}
		}
		else if (strFront == "LandCards")//地主牌  数据类型 LandCards:1,2,3
		{
			std::vector<int> vecLandCards = stringTovector(getStringByPos(strBuff, pos));
			//显示地主
			for (int i = 0; i < vecLandCards.size(); i++)
			{
				Card* cardFront = Card::create(vecLandCards[i]);
				cardFront->setPosition(vecAllCardBacks[51 + i]->getPosition());
				this->addChild(cardFront);
			}
			//添加地主
			if (playerSelf->getRole() == ERoleStatu_Landlord)
			{
				playerSelf->addLandCards(vecLandCards);
				for (int i = 0; i < vecLandCards.size(); i++)
				{
					vecAllCardBacks.back()->removeFromParent();
					vecAllCardBacks.pop_back();
				}
			}
			else
			{
				Player* curPlay = playerLeft->getRole() == ERoleStatu_Landlord ? playerLeft : playerRight;
				for (int i = 0; i < vecLandCards.size(); i++)
				{
					curPlay->addCard(vecAllCardBacks[51 + i]);
				}
			}
		}
		else if (strFront == "ChoosePutCards")// 数据类型 ChoosePutCards:11,12,13
		{
			std::string strCards = getStringByPos(strBuff, pos);
			if (strCards == "")
			{
				playerLeft->removeShowCards();
				playerRight->removeShowCards();
			}
			else
			{
				playerSelf->setComparePoints(stringTovector(strCards));
			}
			playerSelf->removeShowCards();
			showChoosePutButtons();
		}
		else if (strFront == "ShowCards")//数据类型 ShowCards:1:1,2,3
		{
			int playID = std::stoi(getStringByPos(strBuff, pos));//第一个:到第二个:之间的内容
			std::string strPlayCards = getStringByPos(strBuff, pos);
			if (strPlayCards == "")//strPlayID 不要
			{

			}
			//strPlayID 要
			mapPlayer[playID]->showOtherCards(stringTovector(strPlayCards));//strPlayID号玩家 -- playerSelf
		}
		else if (strFront == "Win")//数据类型 Win:0::1:1,2,3
		{
			int playID_1 = std::stoi(getStringByPos(strBuff, pos));
			std::string strPlayCards_1 = getStringByPos(strBuff, pos);
			mapPlayer[playID_1]->showOtherCards(stringTovector(strPlayCards_1));

			int playID_2 = std::stoi(getStringByPos(strBuff, pos));
			std::string strPlayCards_2 = getStringByPos(strBuff, pos);
			mapPlayer[playID_2]->showOtherCards(stringTovector(strPlayCards_2));

			//判断地主
			if (strPlayCards_1 == "")
			{
				//地主 mapPlayer[playID_1]
			}
			else if (strPlayCards_2 == "")
			{
				//地主 mapPlayer[playID_2]
			}
			else
			{
				//地主 self
			}

			startItem->setVisible(true);
		}
		else if (strFront == "ARE YOU OK")
		{
			//sendServer("OK:");
		}
		else//这是BUG 得治
		{
			CCLOG("Undefined Message");
		}
	}
}

void GameScene::sendServer(std::string strBuff)
{
	const char* buff = strBuff.c_str();
	send(sockClient, buff, strlen(buff) + 1, 0);
}

std::string GameScene::getStringByPos(std::string strBuff, int &pos)
{
	pos = strBuff.find(":", pos) + 1;
	return strBuff.substr(pos, strBuff.find(":", pos) - pos);
}

std::vector<int> GameScene::stringTovector(std::string strCards)//比如12,15,13
{
	std::vector<int> vecCards;
	int nLength = strCards.length();
	int nStart = 0;
	int nEnd = 0;
	while (nStart < nLength)
	{
		nEnd = strCards.find(",", nStart);
		if (nEnd == -1)
		{
			nEnd = nLength;
		}
		std::string nCard = strCards.substr(nStart, nEnd - nStart);
		vecCards.push_back(std::stoi(nCard));
		nStart = nEnd + 1;
	}
	return vecCards;
}

std::string GameScene::vectorTostring(std::vector<int> vecCards)//比如12,15,13
{
	char cardsBuff[100];
	char buff[4];
	for (int i = 0, vecSize = vecCards.size() - 1; i < vecSize; i++)
	{
		sprintf_s(buff, "%d,", vecCards[i]);
		strcat_s(cardsBuff, buff);
	}
	sprintf_s(buff, "%d", vecCards[vecCards.size() - 1]);
	strcat_s(cardsBuff, buff);

	return cardsBuff;
}

//创建牌
void GameScene::createAllCards()
{
	for (int i = 0; i < CARDS_NUM; i++)
	{
		Sprite* cardBack = Sprite::create("image/54.png");
		cardBack->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - cardBack->getContentSize().height / 2));
		cardBack->setScale(0.5f);
		this->addChild(cardBack, 100);
		vecAllCardBacks.push_back(cardBack);
	}
}
//发牌
void GameScene::dealCards()
{
	this->schedule(schedule_selector(GameScene::dealCard), 0.1f, 52, 0);//间隔,重复次数,延时
}

void GameScene::dealCard(float dt)
{
	cardsNum--;
	if (cardsNum > 2)
	{
		Player* curPlayer = nullptr;
		Vec2 player_pos = Vec2::ZERO;
		switch ((cardsNum + playerSelf->getPlayID()) % 3)
		{
		case 0:
			curPlayer = playerRight;
			player_pos = Vec2(visibleSize.width - 250, visibleSize.height - 90);
			break;
		case 1:
			curPlayer = playerLeft;
			player_pos = Vec2(250, visibleSize.height - 90);
			break;
		case 2:
			curPlayer = playerSelf;
			player_pos = Vec2(visibleSize.width / 2, 90);
			break;
		default:
			break;
		}
		Sprite* cardBack = vecAllCardBacks[cardsNum];
		cardBack->runAction(Sequence::createWithTwoActions(
			MoveTo::create(0.1f, player_pos),
			CallFunc::create([this, curPlayer, cardBack]() {
			curPlayer->addCard(cardBack);
		})));
	}
	else if (cardsNum == 2)
	{
		Sprite* cardBack = vecAllCardBacks[cardsNum];
		Vec2 player_pos = Vec2(visibleSize.width / 2 - cardBack->getContentSize().width, visibleSize.height - cardBack->getContentSize().height / 2);
		cardBack->runAction(MoveTo::create(0.1f, player_pos));
	}
	else
	{
		Sprite* cardBack = vecAllCardBacks[cardsNum];
		Vec2 player_pos = Vec2(visibleSize.width / 2 + cardBack->getContentSize().width, visibleSize.height - cardBack->getContentSize().height / 2);
		cardBack->runAction(MoveTo::create(0.1f, player_pos));
	}
}

void GameScene::showChooseCallButtons()
{
	callLandItem->setVisible(true);
	noCallItem->setVisible(true);
}

void GameScene::showChooseRobButtons()
{
	robLandItem->setVisible(true);
	noRobItem->setVisible(true);
}

void GameScene::showChoosePutButtons()
{
	putItem->setVisible(true);
	putItem->setEnabled(false);
	noPutItem->setVisible(true);
}

void GameScene::setPutEnable(bool bEnable)
{
	putItem->setEnabled(bEnable);
}
