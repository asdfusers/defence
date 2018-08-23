#pragma once
#include "cocos2d.h"
#include "ui\UIEditBox\UIEditBox.h"


class LoginScene : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
	virtual bool init();
	static cocos2d::Scene* scene();
	CREATE_FUNC(LoginScene);


	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
		override;
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
		override;
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;

	void onMenu(cocos2d::Object * sender);
	cocos2d::MenuItemSprite* createMenuItemSprite(const char* text, const char* hilight, const char* han, int tag);
private:
	cocos2d::ui::EditBox* _editName;
	cocos2d::ui::EditBox* _editPassword;
	cocos2d::ui::EditBox* _editEmail;

};

