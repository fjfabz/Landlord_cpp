#pragma once
#include <stdio.h>
#include <thread>
#include <WS2tcpip.h>
#include "cocos2d.h"
#include "Player.h"
#include "Card.h"
#pragma comment(lib,"WS2_32.lib")
using namespace std;
USING_NS_CC;

#define SERVER_PORT 8080
class GameScene : public Layer
{
public:
	GameScene();
	~GameScene();
	static Scene* createScene();
	CREATE_FUNC(GameScene);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void createButtons();
	void onStartGame(Ref* pSender);
	void onCallLand(Ref* pSender);
	void onNoCall(Ref* pSender);
	void onRobLand(Ref* pSender);
	void onNoRob(Ref* pSender);
	void onPut(Ref* pSender);
	void onNoPut(Ref* pSender);
	void onNetwork(Ref* pSender);
	void createAllCards();
	void dealCards();
	void dealCard(float dt);
	void showChooseCallButtons();
	void showChooseRobButtons();
	void showChoosePutButtons();
	void setPutEnable(bool bEnable);
	//服务器
	void connServer();
	void connThread();
	void closeConn();
	void recvThread();
	void sendServer(std::string strBuff);
	std::string vectorTostring(std::vector<int> vecCards);
	std::vector<int> stringTovector(std::string strCards);
	std::string getStringByPos(std::string strBuff, int &pos);
	SOCKET sockClient;
	SOCKADDR_IN addrClient;
private:
	Size visibleSize;
	int cardsNum;
	MenuItemImage* netWorkItem;
	MenuItemImage* startItem;
	MenuItemImage* callLandItem;
	MenuItemImage* noCallItem;
	MenuItemImage* robLandItem;
	MenuItemImage* noRobItem;
	MenuItemImage* putItem;
	MenuItemImage* noPutItem;
	Player* playerSelf;
	Player* playerLeft;
	Player* playerRight;
	std::map<int, Player*> mapPlayer;
	std::vector<Sprite*> vecAllCardBacks;
};
