#pragma once
#include "cocos2d.h"
#include <network\SocketIO.h>
using namespace cocos2d::network;

class CUser : public cocos2d::Scene, public SocketIO::SIODelegate
{
public:
	virtual bool init();

	CREATE_FUNC(CUser);
	static CUser * getInst();

	std::string getID() { return userID; }
	std::string getPassword() { return userPassword; }
	std::string getName() { return userName; }
	std::string getSessionID() { return sessionID; }
	std::string getNodeJSID() { return NodeJSID; }
	SIOClient* getClient() { return _client; }
	int getMissileCnt() { return missileCnt; }
	int getCannonCnt() { return cannonCnt; }
	int getScore() { return _score; }

	void setID(std::string ID) { userID = ID; }
	void setPassword(std::string password) { userPassword = password; }
	void setUserName(std::string _userName) { userName = _userName; }
	void setSessionID(std::string _sessionID) { sessionID = _sessionID; }
	void setNodeJSID(std::string _sessionID) { NodeJSID = _sessionID; }
	void setClient(SIOClient* clients) { _client = clients; }
	void setWin(int i) { _win = i; }
	void setLose(int i) { _lose = i; }
	void setSame(int i) { _same = i; }
	void setScore(int i) { _score = i; }
	void setMissileCnt(int i) { missileCnt = i; }
	void setCannonCnt(int i) { cannonCnt = i; }

	void updateState();
	void update(SIOClient * client, const std::string & data);
	virtual void onConnect(SIOClient* client);
	virtual void onMessage(SIOClient* client, const std::string& data);
	virtual void onClose(SIOClient* client);
	virtual void onError(SIOClient* client, const std::string& data);



	std::string enemyName;
	int gameScore;
	int enemyScore;

private:
	std::string userID;
	std::string userPassword;
	std::string userName;
	std::string sessionID;
	std::string NodeJSID;


	int _win, _lose, _same;
	int _score;
	int missileCnt = 0;
	int cannonCnt = 0;
	SIOClient* _client;

	std::vector<char*> info;

	static CUser* inst;

};

