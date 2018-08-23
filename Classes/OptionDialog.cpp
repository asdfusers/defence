#include "OptionDialog.h"
#include "SimpleAudioEngine.h"
#include "LobbyScene.h"
bool COptionDialog::init()
{

	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	if (!LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 180), winSize.width, winSize.height))
		return false;

	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
	cocos2d::SpriteBatchNode* spriteBatchNode = cocos2d::SpriteBatchNode::create("Menu.png");
	spriteBatchNode->setTag(TAG_SPRITE_BATCH_NODE);
	addChild(spriteBatchNode);

	cocos2d::Sprite* spriteDlg = cocos2d::Sprite::createWithSpriteFrameName("option.png");
	spriteDlg->setPosition(cocos2d::Point(winSize.width * 0.5, winSize.height * 0.45));
	spriteDlg->setTag(TAG_DIALOG);
	spriteBatchNode->addChild(spriteDlg);

	cocos2d::Sprite* spriteMusic = cocos2d::Sprite::createWithSpriteFrameName("option_button_00.png");
	spriteMusic->setPosition(cocos2d::Point(SLIDE_LEFT, 155));
	spriteMusic->setTag(TAG_MUSIC);
	spriteDlg->addChild(spriteMusic);

	cocos2d::Sprite* spriteEffect = cocos2d::Sprite::createWithSpriteFrameName("option_button_00.png");
	spriteEffect->setPosition(cocos2d::Point(SLIDE_LEFT, 95));
	spriteEffect->setTag(TAG_EFFECT);
	spriteDlg->addChild(spriteEffect);

	cocos2d::Sprite* spriteReset = cocos2d::Sprite::createWithSpriteFrameName("option_button_01.png");
	spriteReset->setPosition(cocos2d::Point(100, 38));
	spriteReset->setTag(TAG_RESET);
	spriteDlg->addChild(spriteReset);

	cocos2d::Sprite* spriteBack = cocos2d::Sprite::createWithSpriteFrameName("option_back.png");
	spriteBack->setPosition(cocos2d::Point(147, 40));
	spriteBack->setTag(TAG_BACK);
	spriteDlg->addChild(spriteBack);
	
	setTouchEnabled(true);
	return true;

}



void COptionDialog::onEnter()
{
	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(COptionDialog::onTouchBegan, this);
}

void COptionDialog::onExit()
{
	_eventDispatcher->removeEventListener(listener);

}

bool COptionDialog::onTouchBegan(cocos2d::Touch * touches, cocos2d::Event * event)
{
	if (isVisible())
	{
		//cocos2d::Touch* touch = (cocos2d::Touch*)*touches->begin();
		cocos2d::Point pt = touches->getLocation();

		cocos2d::SpriteBatchNode* batchNode = (cocos2d::SpriteBatchNode*)getChildByTag(TAG_SPRITE_BATCH_NODE);
		cocos2d::Sprite* dialog = (cocos2d::Sprite*)batchNode->getChildByTag(TAG_DIALOG);
		cocos2d::Sprite* music = (cocos2d::Sprite*)dialog->getChildByTag(TAG_MUSIC);
		cocos2d::Sprite* effect = (cocos2d::Sprite*)dialog->getChildByTag(TAG_EFFECT);
		cocos2d::Sprite* reset = (cocos2d::Sprite*)dialog->getChildByTag(TAG_RESET);
		cocos2d::Sprite* back = (cocos2d::Sprite*)dialog->getChildByTag(TAG_BACK);

		cocos2d::Rect rtDlg = dialog->getBoundingBox();
		pt.x -= rtDlg.origin.x;
		pt.y -= rtDlg.origin.y;

		cocos2d::Rect rtMusic = music->getBoundingBox();
		cocos2d::Rect rtEffect = effect->getBoundingBox();
		cocos2d::Rect rtReset = reset->getBoundingBox();
		cocos2d::Rect rtBack = back->getBoundingBox();

		if (rtMusic.containsPoint(pt))
		{
			buttonEffect();
		}
		else if (rtEffect.containsPoint(pt))
		{
			buttonEffect();
		}
		else if (rtReset.containsPoint(pt))
		{
			buttonEffect();
			cocos2d::MessageBox("모든 데이타를 초기화 합니다.", "Reset");
		}
		else if (rtBack.containsPoint(pt))
		{
			CLobbyScene* title = (CLobbyScene*)getParent();
			title->hideOptionDlg();
		}
	}
	return true;
}

void COptionDialog::onTouchEnded(cocos2d::Touch * touches, cocos2d::Event * event)
{
}


void COptionDialog::showDialog()
{
	
}

void COptionDialog::hideDialog()
{
	setVisible(false);
}

void COptionDialog::buttonEffect()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button.wav");
}
