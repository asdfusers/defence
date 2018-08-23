#include "User.h"
#include "json/rapidjson.h"
#include "json/document.h"

CUser * CUser::inst = NULL;
bool CUser::init()
{
	if (!Scene::init())
		return false;
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	_client = SocketIO::connect(*this, "http://localhost:4000");
	_client->on("info", CC_CALLBACK_2(CUser::update, this));
	return true;												
}

CUser * CUser::getInst()
{
	if (inst == NULL)
		inst = new CUser();

	return inst;
}

void CUser::updateState()
{
	_client->emit("updateState", "Update");
}

void CUser::update(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());


	CUser::getInst()->setUserName(doc["userName"].GetString());
	CUser::getInst()->setWin(doc["userWin"].GetInt());
	CUser::getInst()->setLose(doc["userlose"].GetInt());
	CUser::getInst()->setSame(doc["usersame"].GetInt());
	CUser::getInst()->setScore(doc["userScore"].GetInt());
	CUser::getInst()->setMissileCnt(doc["userMissile"].GetInt());
	CUser::getInst()->setCannonCnt(doc["userCannon"].GetInt());

}

void CUser::onConnect(SIOClient * client)
{
}

void CUser::onMessage(SIOClient * client, const std::string & data)
{
}

void CUser::onClose(SIOClient * client)
{
}

void CUser::onError(SIOClient * client, const std::string & data)
{
}

