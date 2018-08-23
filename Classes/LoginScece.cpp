#include "LoginScene.h"
#include "proj.win32\Defines.h"
#include "LobbyScene.h"
#include "GameScene.h"
bool LoginScene::init()
{
	if (!Layer::init())
		return false;
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	// Menu SpriteSheet
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
	cocos2d::SpriteBatchNode* spriteBatchNodeMenu = cocos2d::SpriteBatchNode::create("Menu.png");
	spriteBatchNodeMenu->setTag(TAG_SPRITE_BATCH_NODE_MENU);
	addChild(spriteBatchNodeMenu);

	// 배경화면
	cocos2d::Sprite* spriteBK = cocos2d::Sprite::createWithSpriteFrameName("MENU_BK.png");
	spriteBK->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2));
	spriteBK->setScale(winSize.width / spriteBK->getContentSize().width);
	spriteBatchNodeMenu->addChild(spriteBK);

	//상단 타이틀
	cocos2d::Sprite* spriteTop = cocos2d::Sprite::createWithSpriteFrameName("main_title.png");
	spriteTop->setAnchorPoint(cocos2d::Point(0.5, 1));
	spriteTop->setPosition(cocos2d::Point(winSize.width / 2, winSize.height));
	spriteTop->setScale(winSize.width / spriteTop->getContentSize().width);
	spriteBatchNodeMenu->addChild(spriteTop, ZORDER_TITLE);

	//하단 바
	cocos2d::Sprite* spriteBottom = cocos2d::Sprite::createWithSpriteFrameName("main_down.png");
	spriteBottom->setAnchorPoint(cocos2d::Point(0.5, 0));
	spriteBottom->setPosition(cocos2d::Point(winSize.width / 2, 0));
	spriteBottom->setScale(winSize.width / spriteBottom->getContentSize().width);
	spriteBatchNodeMenu->addChild(spriteBottom, ZORDER_TITLE);


	cocos2d::Size editBoxSize = cocos2d::Size(200, 40);
	std::string pNormalSprite = "login.png";
	_editName = cocos2d::ui::EditBox::create(editBoxSize, cocos2d::ui::Scale9Sprite::create(pNormalSprite));
	_editName->setPosition(cocos2d::Vec2(170, 270));
	_editName->setFontSize(20);
	_editName->setOpacity(100);
	_editName->setFontColor(cocos2d::Color3B::GRAY);
	_editName->setPlaceHolder("            ID   ");
	_editName->setPlaceholderFontColor(cocos2d::Color3B::GRAY);
	_editName->setMaxLength(8);
	_editName->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
	_editName->setDelegate(this);
	addChild(_editName);

	_editPassword = cocos2d::ui::EditBox::create(editBoxSize, "login.png");
	_editPassword->setPosition(cocos2d::Vec2(170, 230));
	_editPassword->setFontSize(20);
	_editPassword->setOpacity(100);
	_editPassword->setFontColor(cocos2d::Color3B::GRAY);
	_editPassword->setPlaceHolder("     Password  ");
	_editPassword->setMaxLength(6);
	_editPassword->setPlaceholderFontColor(cocos2d::Color3B::GRAY);
	_editPassword->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
	_editPassword->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
	_editPassword->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
	_editPassword->setDelegate(this);
	addChild(_editPassword);

	cocos2d::MenuItemImage*  loginMenu = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(LoginScene::onMenu, this));

	//각 MenuItem들에 Tag값 부여
	loginMenu->setTag(TAG_MENU_LOGIN);
	loginMenu->setPosition(cocos2d::Vec2(290, 250));

	loginMenu->setScale(loginMenu->getScale() * 2);

	//MenuItem들로부터 Menu 생성
	cocos2d::Menu* menu = cocos2d::Menu::create(loginMenu, NULL);
	menu->setPosition(cocos2d::Vec2::ZERO);
	addChild(menu, ZORDER_MENU);

	return true;
}

cocos2d::Scene * LoginScene::scene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	LoginScene* login = LoginScene::create();

	scene->addChild(login);
	return scene;
}

void LoginScene::editBoxEditingDidBegin(cocos2d::ui::EditBox * editBox)
{
}

void LoginScene::editBoxEditingDidEnd(cocos2d::ui::EditBox * editBox)
{
}

void LoginScene::editBoxTextChanged(cocos2d::ui::EditBox * editBox, const std::string & text)
{
}

void LoginScene::editBoxReturn(cocos2d::ui::EditBox * editBox)
{
}

void LoginScene::onMenu(cocos2d::Object * sender)
{
	switch (((Node*)sender)->getTag())
	{
	case TAG_MENU_LOGIN:
		cocos2d::Director::getInstance()->replaceScene(GameScene::createScene());
		break;

	}
}

cocos2d::MenuItemSprite * LoginScene::createMenuItemSprite(const char * text, const char * hilight, const char * han, int tag)
{
	return nullptr;
}
