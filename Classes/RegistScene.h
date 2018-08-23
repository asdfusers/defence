#pragma once
#include "cocos2d.h"
#include "ui\UIEditBox\UIEditBox.h"
#include "ui\CocosGUI.h"
#include "ui\UIEditBox\UIEditBox.h"
#include "network\HttpClient.h"



using namespace cocos2d::ui;

class CRegistScene : public cocos2d::Scene, public cocos2d::ui::EditBoxDelegate
{
public:
	static cocos2d::Scene* scene();
	virtual bool init();
	CREATE_FUNC(CRegistScene);


	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;

	void onHttpRequestCompleted(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response);

	void onBack();
	void doClick(Ref * pSender);

private:
	cocos2d::ui::EditBox* _editName;
	cocos2d::ui::EditBox* _editPassword;
	cocos2d::ui::EditBox* _nickName;


	std::string editName;
	std::string editPassword;
	std::string nickName;
	int win = 0;
	int lose = 0;
	int same = 0;
};

