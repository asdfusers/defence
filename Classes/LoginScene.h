#pragma once
#include "cocos2d.h"
#include "ui\UIEditBox\UIEditBox.h"
#include "ui\CocosGUI.h"
#include "ui\UIEditBox\UIEditBox.h"
#include "network\HttpClient.h"




using namespace cocos2d::ui;

class CLoginScene : public cocos2d::Scene, public cocos2d::ui::EditBoxDelegate
{
public:
	virtual bool init();
	static cocos2d::Scene* scene();
	CREATE_FUNC(CLoginScene);


	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
	void onLogin();
	void onRegist();

	void onHttpRequestCompleted(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response);
	cocos2d::MenuItemSprite* createMenuItemSprite(const char* text, const char* hilight, const char* han, int tag);

private:
	cocos2d::ui::EditBox* _editName;
	cocos2d::ui::EditBox* _editPassword;




};

