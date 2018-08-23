#include "LobbyScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <vector>
#include "User.h"
#include "RegistScene.h"
#include "GameScene.h"
#include "OptionDialog.h"
#include "StoreScene.h"
#include "SimpleAudioEngine.h"
#include <iostream>

//#include "LoginScene.h"

#define TAG_SURFACE 1000

CLobbyScene * CLobbyScene::inst = NULL;

bool CLobbyScene::init()
{
	if (!Scene::init())
		return false;
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	//
	_client = CUser::getInst()->getClient();
	_client->emit("Lobby", "Looby");

	// Menu SpriteSheet
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
	cocos2d::SpriteBatchNode* spriteBatchNodeMenu = cocos2d::SpriteBatchNode::create("Menu.png");
	addChild(spriteBatchNodeMenu);

	// 배경화면
	spriteSEA = cocos2d::Sprite::create("chat2.png");
	spriteSEA->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2));
	spriteSEA->setScale(getScale()*3);
	this->addChild(spriteSEA,0, TAG_SURFACE);
	
	
	insertChat = cocos2d::ui::EditBox::create(cocos2d::Size(200, 30), "chat.png");
	insertChat->setPosition(cocos2d::Vec2(120, 40));
	insertChat->setFontSize(20);
	insertChat->setFontColor(cocos2d::Color3B::WHITE);
	insertChat->setPlaceHolder("Chat !!!");
	insertChat->setPlaceholderFontColor(cocos2d::Color3B::WHITE);
	insertChat->setMaxLength(20);
	insertChat->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
	insertChat->setDelegate(this);
	this->addChild(insertChat,2);

	auto sendbtn = cocos2d::ui::Button::create("send.png");
	sendbtn->setPosition(cocos2d::Vec2(280, 40));
	sendbtn->setScale(sendbtn->getScale() * 2);
	sendbtn->addClickEventListener(CC_CALLBACK_0(CLobbyScene::textFieldEvent, this));
	this->addChild(sendbtn,2, TAG_SEND_BTN);

	auto chatImage = cocos2d::Sprite::create("chat3.png");
	chatImage->setPosition(cocos2d::Vec2(winSize.width / 3 - 20 , 180));
	chatImage->setContentSize(cocos2d::Size(winSize.width / 2 + 20, 220));
	this->addChild(chatImage, 1);
	//listView
	listView = cocos2d::ui::ListView::create();
	listView->setClippingEnabled(true);
	listView->setPosition(cocos2d::Vec2(22, 70));
	listView->setDirection(cocos2d::ui::ListView::Direction::VERTICAL);
	listView->setBounceEnabled(false);
	listView->setItemsMargin(2.0f);
	listView->setContentSize(cocos2d::Size(winSize.width/2 + 20, 224));
	this->addChild(listView,2);


	auto rankImg = cocos2d::Sprite::create("highscore.png");
	rankImg->setPosition(cocos2d::Vec2(480, 165));
	this->addChild(rankImg, 1);

	//rankgView
	rankView = cocos2d::ui::ListView::create();
	rankView->setPosition(cocos2d::Vec2(410, 40));
	rankView->setDirection(cocos2d::ui::ListView::Direction::VERTICAL);
	rankView->setItemsMargin(2.0f);
	rankView->setScrollBarEnabled(false);
	rankView->setContentSize(cocos2d::Size(winSize.width /3, 220));
	this->addChild(rankView, 2);

	//게임 시작 버튼
	auto start = cocos2d::ui::Button::create("main_start_00.png", "main_start_01.png");
	start->setPosition(cocos2d::Vec2(winSize.width / 2 + 180, winSize.height-40));
	start->setScale(start->getScale()*1.4f);
	start->addClickEventListener(CC_CALLBACK_0(CLobbyScene::onStart, this));
	this->addChild(start, 3, TAG_START_BTN);


	//게임 상점 버튼
	auto store = cocos2d::ui::Button::create("main_store_00.png", "main_store_01.png");
	store->setPosition(cocos2d::Vec2(170 , winSize.height - 40));
	store->setScale(store->getScale()*0.35f);
	store->addClickEventListener(CC_CALLBACK_0(CLobbyScene::onStore, this));
	this->addChild(store, 3, TAG_STORE_BTN);



	_client->on("chat", CC_CALLBACK_2(CLobbyScene::onReceivedEvent, this));
	_client->on("userInfo", CC_CALLBACK_2(CLobbyScene::onLoginReceivedEvent, this));
	_client->on("rank", CC_CALLBACK_2(CLobbyScene::onRank, this));
	_client->on("GameStart", CC_CALLBACK_2(CLobbyScene::onGameStart, this));
	_client->on("MessageRoom", CC_CALLBACK_2(CLobbyScene::onMessageInRoom, this));
	_client->on("MessageRoom", CC_CALLBACK_2(CLobbyScene::onSessionID, this));

	this->scheduleOnce(schedule_selector(CLobbyScene::SessionCheck), 1.0f);

	
	return true;
}

cocos2d::Scene * CLobbyScene::scene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	CLobbyScene* lobby = CLobbyScene::create();

	scene->addChild(lobby);
	return scene;
}

void CLobbyScene::onConnect(SIOClient * client)
{
	CCLOG("success!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	
}

void CLobbyScene::onMessage(SIOClient * client, const std::string & data)
{
	CCLOG("%s", data);

}

void CLobbyScene::onClose(SIOClient * client)
{
	CCLOG("close!!!!!!!!!!!!!!!!!!!!!");

}

void CLobbyScene::onError(SIOClient * client, const std::string & data)
{
	CCLOG("error!!!!!!!!!!!!!!!!!!!!!");

}

void CLobbyScene::editBoxEditingDidBegin(cocos2d::ui::EditBox * editBox)
{
}

void CLobbyScene::editBoxEditingDidEnd(cocos2d::ui::EditBox * editBox)
{
}

void CLobbyScene::editBoxTextChanged(cocos2d::ui::EditBox * editBox, const std::string & text)
{
}

void CLobbyScene::editBoxReturn(cocos2d::ui::EditBox * editBox)
{
	if (editBox == insertChat)
		_chat = editBox->getText();
}

void CLobbyScene::textFieldEvent()
{
	_client->emit("chat", _chat);
	insertChat->setText("");
}

void CLobbyScene::onReceivedEvent(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	rapidjson::Value &val = doc["value"];
	std::string value = val.GetString();

	auto button = Button::create();
	button->setTitleText(value);
	button->setTitleFontSize(15);
	button->setTitleColor(cocos2d::Color3B::WHITE);
	button->setTouchEnabled(false);
	listView->pushBackCustomItem(button);

	listView->jumpToBottom();
}

void CLobbyScene::onLoginReceivedEvent(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	rapidjson::Value &val = doc["value"];
	std::string value = val.GetString();
	userInfoToken(value);

}

void CLobbyScene::hideOptionDlg()
{
	buttonEffect();
	((COptionDialog*)getChildByTag(TAG_OPTION_DLG))->hideDialog();
	//_eventDispatcher->resumeEventListenersForTarget((cocos2d::ui::Button*)getChildByTag(TAG_OPTION_BTN), true);
}

void CLobbyScene::showOptionDlg()
{
	buttonEffect();
	((COptionDialog*)getChildByTag(TAG_OPTION_DLG))->showDialog();
	//cocos2d::Director::getInstance()->getEventDispatcher()->removeAllEventListeners();

}

void CLobbyScene::buttonEffect()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button.wav");
}

void CLobbyScene::userInfoToken(std::string buffer)
{
	const char * c = buffer.c_str();
	char *token = std::strtok(const_cast<char*>(c), " ");
	while (token != NULL)
	{
		info.push_back(token);
		token = std::strtok(NULL, " ");
	}
	CUser::getInst()->setUserName(info[0]);
	CUser::getInst()->setWin(atoi(info[1]));
	CUser::getInst()->setLose(atoi(info[2]));
	CUser::getInst()->setSame(atoi(info[3]));
	CUser::getInst()->setScore(atoi(info[4]));

	
}



void CLobbyScene::SessionCheck(float dt)
{
	_client->emit("loginID", CUser::getInst()->getID());
	_client->emit("login", CUser::getInst()->getSessionID());
}

void CLobbyScene::onScore()
{
}

void CLobbyScene::onStart()
{	
	_client->emit("gameStart", "GameStart");
}

void CLobbyScene::onGameStart(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	rapidjson::Value &val = doc["value"];
	std::string value = val.GetString();
	CUser::getInst()->getClient()->emit("start", "asdf");
	cocos2d::Director::getInstance()->pushScene(cocos2d::TransitionFade::create(1.5, CGameScene::scene(), cocos2d::Color3B(0, 0, 0)));

}

void CLobbyScene::onOption()
{
}

void CLobbyScene::onStore()
{
cocos2d::Director::getInstance()->pushScene(cocos2d::TransitionFade::create(1.5, CStoreScene::createScene(), cocos2d::Color3B(0, 0, 0)));
}

CLobbyScene * CLobbyScene::getInst()
{
	if (inst == NULL)
		inst = new CLobbyScene();

	return inst;
}

void CLobbyScene::onRank(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	rapidjson::Value &val = doc["value"];
	std::string value = val.GetString();

	auto button = Button::create();
	button->setTitleText(value);
	button->setTitleFontSize(15);
	button->setTitleColor(cocos2d::Color3B::WHITE);
	button->setTouchEnabled(false);
	rankView->pushBackCustomItem(button);


}

void CLobbyScene::onMessageInRoom(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	rapidjson::Value &val = doc["value"];
	std::string value = val.GetString();
	CCLOG("%s", value);

}

void CLobbyScene::onSessionID(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	rapidjson::Value &val = doc["value"];
	std::string value = val.GetString();
	CUser::getInst()->setNodeJSID(value);
}

