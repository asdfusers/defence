#pragma once
#include "cocos2d.h"
#define TAG_SPRITE_BATCH_NODE 0
#define TAG_MUSIC   1
#define TAG_EFFECT  2
#define TAG_RESET   3
#define TAG_BACK    4
#define TAG_DIALOG  5

#define SLIDE_LEFT 85
#define SLIDE_RIGHT 130
class COptionDialog : public cocos2d::LayerColor
{
public:	
	virtual bool init();
	CREATE_FUNC(COptionDialog);
	

	cocos2d::EventListenerTouchOneByOne* listener;
	void onEnter();
	void onExit();
	bool onTouchBegan(cocos2d::Touch* touches, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touches, cocos2d::Event* event);
	
	void clickBack();
	void showDialog();
	void hideDialog();
	void buttonEffect();

};

