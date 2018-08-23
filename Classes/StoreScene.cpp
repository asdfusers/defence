#include "StoreScene.h"
#include "SimpleAudioEngine.h"
#include "define.h"
#include "User.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "OK.h"
#include "FailDlg.h"
#include "LobbyScene.h"

USING_NS_CC;

Scene* CStoreScene::createScene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	CStoreScene* lobby = CStoreScene::create();

	scene->addChild(lobby);
	return scene;
}



// on "init" you need to initialize your instance
bool CStoreScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	
	CUser::getInst()->getClient()->emit("getScore", "Score");
	CUser::getInst()->getClient()->on("getScores", CC_CALLBACK_2(CStoreScene::getScores, this));
	CUser::getInst()->getClient()->on("failBuy", CC_CALLBACK_2(CStoreScene::fail, this));
	Size winSize = Director::getInstance()->getWinSize();


	// Menu SpriteSheet
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
	cocos2d::SpriteBatchNode* spriteBatchNodeMenu = cocos2d::SpriteBatchNode::create("Menu.png");
	addChild(spriteBatchNodeMenu);

	// 배경화면
	auto backGround = cocos2d::Sprite::create("chat2.png");
	backGround->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2));
	backGround->setScale(getScale() * 3);
	this->addChild(backGround, 0);


	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Interface.plist");
	SpriteBatchNode* spriteBatchNodeInterface = SpriteBatchNode::create("Interface.png");
	addChild(spriteBatchNodeInterface, ZORDER_SPRITE_BATCH_NODE_INTERFACE, TAG_SPRITE_BATCH_NODE_INTERFACE);


	addWeaponInterface("controller_img_cannon.png", "controller_text_cannon.png", Point((winSize.width * 1) / 3, 100), TAG_INTERFACE_CANNON);
	addWeaponInterface("controller_img_missile.png", "controller_text_missile.png", Point((winSize.width * 2) /3 + 20 , 100), TAG_INTERFACE_MISSILE);

	//구매하기
	cannonBuyButton = cocos2d::ui::Button::create("buy.png", "buy.png");
	cannonBuyButton->setPosition(cocos2d::Vec2(200, 110));
	cannonBuyButton->setScale(cannonBuyButton->getScale() * 2);
	cannonBuyButton->addClickEventListener(CC_CALLBACK_0(CStoreScene::buy1, this));
	cannonBuyButton->setTouchEnabled(true);
	
	this->addChild(cannonBuyButton);

	//구매하기
	 missilebuyButton = cocos2d::ui::Button::create("buy.png", "buy.png");
	missilebuyButton->setPosition(cocos2d::Vec2(420, 110));
	missilebuyButton->setScale(missilebuyButton->getScale() * 2);
	missilebuyButton->addClickEventListener(CC_CALLBACK_0(CStoreScene::buy2, this));
	missilebuyButton->setTouchEnabled(true);
	this->addChild(missilebuyButton);
	CUser::getInst()->updateState();



	backButton = cocos2d::ui::Button::create("back.png");
	backButton->setScale(backButton->getScale() * 2);
	backButton->setPosition(Point(winSize.width /2 , 50));
	backButton->addClickEventListener(CC_CALLBACK_0(CStoreScene::click, this));
	this->addChild(backButton, 1);

	return true;
}

void CStoreScene::addWeaponInterface(const char * imgPath, const char * txtPath, cocos2d::Point pt, int tag)
{
	Size winSize = Director::getInstance()->getWinSize();

	//버튼의 배경 이미지
	Sprite* spriteBK = Sprite::createWithSpriteFrameName("controller_back.png");
	spriteBK->setAnchorPoint(Point(0.5, 0));
	spriteBK->setScale(spriteBK->getScale() * 2);
	spriteBK->setPosition(pt);
	spriteBK->setTag(tag);
	getChildByTag(TAG_SPRITE_BATCH_NODE_INTERFACE)->addChild(spriteBK);

	//버튼 하단 텍스트
	Sprite* spriteText = Sprite::createWithSpriteFrameName(txtPath);
	spriteText->setAnchorPoint(Point(0.5, 0));
	spriteText->setScale(spriteText->getScale() * 2);
	spriteText->setPosition(Point(44, 100));
	spriteBK->addChild(spriteText);
	
	//무기 이미지

	 Sprite* spriteWeapon = Sprite::createWithSpriteFrameName(imgPath);
	spriteWeapon->setPosition(Point(42, 62));
	spriteWeapon->setScale(spriteWeapon->getScale() * 1);
	spriteBK->addChild(spriteWeapon);
	

	button = Button::create();
	addChild(button);



}



void CStoreScene::click()
{
	cocos2d::Director::getInstance()->popScene();
}


void CStoreScene::buy1()
{
	CUser::getInst()->getClient()->emit("buyCannon", "buyCannon");
	ShowDlg(1);
	CUser::getInst()->updateState();
	CUser::getInst()->getClient()->emit("getScore", "Score");

}

void CStoreScene::buy2()
{
	CUser::getInst()->getClient()->emit("buyMissile", "buyMissile");
	ShowDlg(2);
	CUser::getInst()->updateState();
	CUser::getInst()->getClient()->emit("getScore", "Score");
}

void CStoreScene::getScores(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	Score = doc["value"].GetInt();

	std::string text = "SCORE : ";
	text += std::to_string(Score);
	button->setTitleText(text);
	button->setTitleFontSize(35);
	button->setTitleColor(cocos2d::Color3B::BLACK);
	button->setTouchEnabled(false);
	button->setPosition(cocos2d::Vec2(420, 370));
}

void CStoreScene::fail(SIOClient * client, const std::string & data)
{
	bFail = true;
}

void CStoreScene::hideDlg()
{
	cannonBuyButton->setTouchEnabled(true);
	missilebuyButton->setTouchEnabled(true);
	backButton->setTouchEnabled(true);
	bFail = true;
	//setVisible(false);
}

void CStoreScene::ShowDlg(int tag)
{
	
	int price;
	if (tag == 1)
		price = 4000;
	else if (tag == 2)
		price = 8000;


	if (Score >= price)
	{
		COK* optionDlg = COK::create();
		optionDlg->setVisible(true);
		addChild(optionDlg, 10, TAG_OPTION_DLG);
	}
	else
	{
		CFailDlg* failDlg = CFailDlg::create();
		failDlg->setVisible(true);
		addChild(failDlg, 10, TAG_OPTION_DLG);
	}

	cannonBuyButton->setTouchEnabled(false);
	missilebuyButton->setTouchEnabled(false);
	backButton->setTouchEnabled(false);


}
