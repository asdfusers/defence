#include "OK.h"
#include "SimpleAudioEngine.h"
#include "LobbyScene.h"
#include "StoreScene.h"
bool COK::init()
{

	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	if (!LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 180), winSize.width, winSize.height))
		return false;

	if (bFail)
	{
		auto button = Button::create();
		std::string text = "Fail. ";
		button->setTitleText(text);
		button->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2));
		button->setTitleFontSize(35);
		button->setTitleColor(cocos2d::Color3B::BLACK);
		button->setTouchEnabled(false);
		addChild(button);

		button->setTouchEnabled(false);
	}
	else {
		auto button = Button::create();
		std::string text = "SUCCESS. ";
		button->setTitleText(text);
		button->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2));
		button->setTitleFontSize(35);
		button->setTitleColor(cocos2d::Color3B::BLACK);
		button->setTouchEnabled(false);
		addChild(button);

		button->setTouchEnabled(false);
	}
	
	//구매하기
	auto buy = cocos2d::ui::Button::create("OK.png", "OK.png");
	buy->setPosition(cocos2d::Vec2(winSize.width/ 2, winSize.height/2 - 50));
	buy->setScale(buy->getScale() * 2);
	buy->addClickEventListener(CC_CALLBACK_0(COK::OK, this));
	this->addChild(buy);
		
	setTouchEnabled(true);
	return true;

}
void COK::OK()
{
	CStoreScene* lobby = (CStoreScene*)getParent();
	setVisible(false);
	lobby->hideDlg();
}
void COK::buttonEffect()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button.wav");
}
