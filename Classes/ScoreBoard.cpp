#include "ScoreBoard.h"
#include "SimpleAudioEngine.h"
#include "LobbyScene.h"
#include "GameScene.h"
#include "User.h"
#include "StoreScene.h"
bool CScoreBoard::init()
{

	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	if (!LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 180), winSize.width, winSize.height))
		return false;

	cocos2d::Sprite* scoreBoard = cocos2d::Sprite::create("ScoreBoard.png");
	scoreBoard->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2));
	scoreBoard->setScale(scoreBoard->getScale() / 1.5);
	this->addChild(scoreBoard);

	auto userName = cocos2d::Label::create(CUser::getInst()->getName(), "", 34);
	userName->setPosition(cocos2d::Vec2(winSize.width / 2 - 130, winSize.height / 2 + 10));
	userName->setColor(cocos2d::Color3B::BLACK);
	userName->setScale(userName->getScale() * 0.7);
	this->addChild(userName);

	auto userScore = cocos2d::Label::create(std::to_string(CUser::getInst()->gameScore), "", 34);
	userScore->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2 + 10));
	userScore->setColor(cocos2d::Color3B::BLACK);
	userScore->setScale(userScore->getScale() * 0.7);
	this->addChild(userScore);

	auto enemyName = cocos2d::Label::create(CUser::getInst()->enemyName, "", 34);
	enemyName->setPosition(cocos2d::Vec2(winSize.width / 2 - 130, winSize.height / 2-10));
	enemyName->setColor(cocos2d::Color3B::BLACK);
	enemyName->setScale(enemyName->getScale() * 0.7);
	this->addChild(enemyName);

	auto enemyScore = cocos2d::Label::create(std::to_string(CUser::getInst()->enemyScore), "", 34);
	enemyScore->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2-10));
	enemyScore->setColor(cocos2d::Color3B::BLACK);
	enemyScore->setScale(enemyScore->getScale() * 0.7);
	this->addChild(enemyScore);

	if (CUser::getInst()->enemyScore > CUser::getInst()->gameScore)
	{
		auto myResult = cocos2d::Label::create("LOSE","", 34);
		myResult->setPosition(cocos2d::Vec2(winSize.width / 2 + 130, winSize.height / 2 + 10));
		myResult->setColor(cocos2d::Color3B::BLACK);
		myResult->setScale(myResult->getScale() * 0.7);

		this->addChild(myResult);


		auto enemyResult = cocos2d::Label::create("WIN", "", 34);
		enemyResult->setPosition(cocos2d::Vec2(winSize.width / 2 + 130, winSize.height / 2-10));
		enemyResult->setColor(cocos2d::Color3B::BLACK);
		enemyResult->setScale(enemyResult->getScale() * 0.7);

		this->addChild(enemyResult); 

	}
	else if (CUser::getInst()->enemyScore == CUser::getInst()->gameScore)
	{
		auto myResult = cocos2d::Label::create("SAME", "", 34);
		myResult->setPosition(cocos2d::Vec2(winSize.width / 2 + 130, winSize.height / 2 + 10));
		myResult->setColor(cocos2d::Color3B::BLACK);
		myResult->setScale(myResult->getScale() * 0.7);

		this->addChild(myResult);

		auto enemyResult = cocos2d::Label::create("SAME", "", 34);
		enemyResult->setPosition(cocos2d::Vec2(winSize.width / 2 + 130, winSize.height / 2-10));
		enemyResult->setColor(cocos2d::Color3B::BLACK);
		enemyResult->setScale(enemyResult->getScale() * 0.7);

		this->addChild(enemyResult);
	}
	else
	{
		auto myResult = cocos2d::Label::create("WIN", "", 34);
		myResult->setPosition(cocos2d::Vec2(winSize.width / 2 + 130, winSize.height / 2 +10));
		myResult->setColor(cocos2d::Color3B::BLACK);
		myResult->setScale(myResult->getScale() * 0.7);

		this->addChild(myResult);

		auto enemyResult = cocos2d::Label::create("LOSE", "", 34);
		enemyResult->setPosition(cocos2d::Vec2(winSize.width / 2 + 130, winSize.height / 2-10));
		enemyResult->setColor(cocos2d::Color3B::BLACK);
		enemyResult->setScale(enemyResult->getScale() * 0.7);

		this->addChild(enemyResult);
	}

	auto ok = cocos2d::ui::Button::create("OK.png", "OK.png");
	ok->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2 - 50));
	ok->setScale(ok->getScale() * 2);
	ok->addClickEventListener(CC_CALLBACK_0(CScoreBoard::OK, this));
	this->addChild(ok);

	
	setTouchEnabled(true);
	return true;

}

void CScoreBoard::OK()
{
	CGameScene* lobby = (CGameScene*)getParent();
	lobby->hideScoreDlg();
	setVisible(false);

}

