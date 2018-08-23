#include "LoginScene.h"
#include "RegistScene.h"
#include "LobbyScene.h"
#include "User.h"
#include "SimpleAudioEngine.h"
//#include "proj.win32\Defines.h"
//#include "LobbyScene.h"
//#include "GameScene.h"
bool CLoginScene::init()
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
	
	//로그인 버튼
	auto login = cocos2d::ui::Button::create("loginBtn.png", "loginBtn2.png");
	login->setPosition(cocos2d::Vec2(winSize.width / 2-45, winSize.height / 2 - 70));
	login->setScale(login->getScale()*1.5f);
	login->addClickEventListener(CC_CALLBACK_0(CLoginScene::onLogin, this));
	this->addChild(login,3);

	//회원가입 버튼
	auto regist = cocos2d::ui::Button::create("regist.png", "regist2.png");
	regist->setPosition(cocos2d::Vec2(winSize.width / 2+45, winSize.height / 2 - 70));
	regist->setScale(regist->getScale()*1.5f);
	regist->addClickEventListener(CC_CALLBACK_0(CLoginScene::onRegist, this));
	this->addChild(regist, 3);

	cocos2d::Size editBoxSize = cocos2d::Size(200, 40);
	std::string pNormalSprite = "login.png";
	_editName = cocos2d::ui::EditBox::create(editBoxSize, cocos2d::ui::Scale9Sprite::create(pNormalSprite));
	_editName->setPosition(cocos2d::Vec2(winSize.width/2, winSize.height/2 + 20));
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
	_editPassword->setPosition(cocos2d::Vec2(winSize.width / 2 , winSize.height / 2-30));
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




	CocosDenshion::SimpleAudioEngine* audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	if (false == audioEngine->isBackgroundMusicPlaying())
	{
		audioEngine->playBackgroundMusic("bgm.mp3", true);
	}

		return true;
}

cocos2d::Scene * CLoginScene::scene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	CLoginScene* login = CLoginScene::create();

	scene->addChild(login);
	return scene;
}

void CLoginScene::editBoxEditingDidBegin(cocos2d::ui::EditBox * editBox)
{
}

void CLoginScene::editBoxEditingDidEnd(cocos2d::ui::EditBox * editBox)
{
}

void CLoginScene::editBoxTextChanged(cocos2d::ui::EditBox * editBox, const std::string & text)
{
}

void CLoginScene::editBoxReturn(cocos2d::ui::EditBox * editBox)
{
	if (editBox == _editName)
		CUser::getInst()->setID(editBox->getText());
	else if (editBox == _editPassword)
		CUser::getInst()->setPassword(editBox->getText());
}

void CLoginScene::onLogin()
{
	std::string dataToSend = "userID=" + CUser::getInst()->getID() + "&userPassword=" + CUser::getInst()->getPassword();
	cocos2d::network::HttpRequest *request = new cocos2d::network::HttpRequest();
	request->setUrl("http://localhost/cocosLogin.php");
	request->setResponseCallback(CC_CALLBACK_2(CLoginScene::onHttpRequestCompleted, this));
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setRequestData(dataToSend.c_str(), dataToSend.length());
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	
	

}

void CLoginScene::onRegist()
{
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1.5, CRegistScene::scene(), cocos2d::Color3B(0,0,0)));
}

void CLoginScene::onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response)
{
	std::string buffer;
	std::vector<char> *pbuffer = response->getResponseData();
	printf("Get data : ");
	for (int i = 0; i < pbuffer->size(); i++)
	{
		buffer += (*pbuffer)[i];
	}
	CCLOG("%s", buffer.c_str());
	if (buffer != "\0")
	{
		CUser::getInst()->setSessionID(buffer);
		cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1.5, CLobbyScene::scene(), cocos2d::Color3B(0, 0, 0)));
	}
}



cocos2d::MenuItemSprite * CLoginScene::createMenuItemSprite(const char * text, const char * hilight, const char * han, int tag)
{
	return nullptr;
}
