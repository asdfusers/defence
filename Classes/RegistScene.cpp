#include "RegistScene.h"
#include "LoginScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

//#include "proj.win32\Defines.h"
//#include "LobbyScene.h"
//#include "GameScene.h"
bool CRegistScene::init()
{
	if (!Scene::init())
		return false;
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	// Menu SpriteSheet
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
	cocos2d::SpriteBatchNode* spriteBatchNodeMenu = cocos2d::SpriteBatchNode::create("Menu.png");
	addChild(spriteBatchNodeMenu);

	// 배경화면
	cocos2d::Sprite* spriteBK = cocos2d::Sprite::createWithSpriteFrameName("MENU_BK.png");
	spriteBK->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2));
	spriteBK->setScale(winSize.width / spriteBK->getContentSize().width);
	spriteBatchNodeMenu->addChild(spriteBK, 1);

	//상단 타이틀
	cocos2d::Sprite* spriteTop = cocos2d::Sprite::createWithSpriteFrameName("main_title.png");
	spriteTop->setAnchorPoint(cocos2d::Point(0.5, 1));
	spriteTop->setPosition(cocos2d::Point(winSize.width / 2, winSize.height));
	spriteBatchNodeMenu->addChild(spriteTop, 2);
	
	//회원가입 버튼
	auto regist = cocos2d::ui::Button::create("regist.png", "regist2.png");
	regist->setPosition(cocos2d::Vec2(winSize.width / 2 - 45, winSize.height / 2 - 120));
	regist->setScale(regist->getScale()*1.5f);
	regist->addClickEventListener(CC_CALLBACK_1(CRegistScene::doClick, this));
	this->addChild(regist, 3);

	//뒤로가기 버튼
	auto back = cocos2d::ui::Button::create("back.png", "back.png");
	back->setPosition(cocos2d::Vec2(winSize.width / 2 + 55, winSize.height / 2 - 120));
	back->setScale(back->getScale()*1.4f);
	back->addClickEventListener(CC_CALLBACK_0(CRegistScene::onBack, this));
	this->addChild(back, 4);

	cocos2d::Size editBoxSize = cocos2d::Size(200, 40);
	std::string pNormalSprite = "login.png";
	_editName = cocos2d::ui::EditBox::create(editBoxSize, cocos2d::ui::Scale9Sprite::create(pNormalSprite));
	_editName->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2 + 20));
	_editName->setFontSize(20);
	_editName->setOpacity(100);
	_editName->setFontColor(cocos2d::Color3B::BLACK);
	_editName->setPlaceHolder("            ID   ");
	_editName->setPlaceholderFontColor(cocos2d::Color3B::GRAY);
	_editName->setMaxLength(10);
	_editName->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
	_editName->setDelegate(this);
	addChild(_editName);

	_editPassword = cocos2d::ui::EditBox::create(editBoxSize, "login.png");
	_editPassword->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2 - 30));
	_editPassword->setFontSize(20);
	_editPassword->setOpacity(100);
	_editPassword->setFontColor(cocos2d::Color3B::BLACK);
	_editPassword->setPlaceHolder("     Password  ");
	_editPassword->setMaxLength(10);
	_editPassword->setPlaceholderFontColor(cocos2d::Color3B::GRAY);
	_editPassword->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
	_editPassword->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
	_editPassword->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
	_editPassword->setDelegate(this);
	addChild(_editPassword);

	_nickName = cocos2d::ui::EditBox::create(editBoxSize, "login.png");
	_nickName->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2 - 80));
	_nickName->setFontSize(20);
	_nickName->setOpacity(100);
	_nickName->setFontColor(cocos2d::Color3B::BLACK);
	_nickName->setPlaceHolder("     NickName  ");
	_nickName->setMaxLength(10);
	_nickName->setPlaceholderFontColor(cocos2d::Color3B::GRAY);
	_nickName->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
	_nickName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
	_nickName->setDelegate(this);
	addChild(_nickName);

	return true;
}

cocos2d::Scene * CRegistScene::scene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	CRegistScene* login = CRegistScene::create();

	scene->addChild(login);
	return scene;
}

void CRegistScene::editBoxEditingDidBegin(cocos2d::ui::EditBox * editBox)
{
}

void CRegistScene::editBoxEditingDidEnd(cocos2d::ui::EditBox * editBox)
{
}

void CRegistScene::editBoxTextChanged(cocos2d::ui::EditBox * editBox, const std::string & text)
{
}

void CRegistScene::editBoxReturn(cocos2d::ui::EditBox * editBox)
{
	if (editBox == _editName)
		editName = editBox->getText();
	else if (editBox == _editPassword)
		editPassword = editBox->getText();
	else
		nickName = editBox->getText();
}

void CRegistScene::onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response)
{
	std::string buffer;
	std::vector<char> *pbuffer = response->getResponseData();
	printf("Get data : ");
	for (int i = 0; i < pbuffer->size(); i++)
	{
		buffer += (*pbuffer)[i];
	}
	CCLOG("%s", buffer.c_str());
	if (response->getResponseCode() == 200)
		CCLOG("successful");
	else
		CCLOG("error");
}

void CRegistScene::onBack()
{
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1.5, CLoginScene::scene(), cocos2d::Color3B(0, 0, 0)));
}

void CRegistScene::doClick(Ref * pSender)
{
	std::string dataToSend = "userID=" +editName + "&userPassword=" + editPassword + "&userName=" + nickName;
	cocos2d::network::HttpRequest *request = new cocos2d::network::HttpRequest();
	request->setUrl("http://localhost/cocosRegist.php");
	request->setResponseCallback(CC_CALLBACK_2(CRegistScene::onHttpRequestCompleted, this));
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setRequestData(dataToSend.c_str(), dataToSend.length());
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	
}

