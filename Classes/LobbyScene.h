#pragma once
#include "cocos2d.h"
#include "ui\UIEditBox\UIEditBox.h"
#include "ui\CocosGUI.h"
#include <network\SocketIO.h>

using namespace cocos2d::network;
using namespace cocos2d::ui;

class CLobbyScene : public cocos2d::Scene, public SocketIO::SIODelegate, public cocos2d::ui::EditBoxDelegate
{
public:
	virtual bool init();
	static cocos2d::Scene* scene();
	CREATE_FUNC(CLobbyScene);
	
	virtual void onConnect(SIOClient* client);
	virtual void onMessage(SIOClient* client, const std::string& data);
	virtual void onClose(SIOClient* client);
	virtual void onError(SIOClient* client, const std::string& data);

	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;

	void textFieldEvent();
	
	
	void onReceivedEvent(SIOClient* client, const std::string& data);
	void onLoginReceivedEvent(SIOClient* client, const std::string& data);
	void onGameStart(SIOClient * client, const std::string & data);
	void onRank(SIOClient * client, const std::string & data);
	void onMessageInRoom(SIOClient * client, const std::string & data);
	void onSessionID(SIOClient * client, const std::string & data);
	
	void hideOptionDlg();
	void showOptionDlg();
	void buttonEffect();
	void userInfoToken(std::string buffer);
	void SessionCheck(float dt);
	void onScore();
	void onStart();
	void onOption();
	void onStore();

	static CLobbyScene * getInst();

private:
	cocos2d::ui::EditBox* insertChat;
	std::string _chat;
	int index;
	SIOClient* _client;
	TextField* editBox;
	ListView* listView;
	ListView* rankView;
	std::vector<char*> info;
	cocos2d::Sprite *spriteSEA;

	static CLobbyScene* inst;

};

