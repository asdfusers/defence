#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <vector>
#include "User.h"
#include "RegistScene.h"
#include <iostream>
#include "define.h"
#include "LobbyScene.h"
#include "SimpleAudioEngine.h"
#include "ScoreBoard.h"
//#include "LoginScene.h"

#define TAG_SURFACE 1000
#define TAG_DOKDO 1001
bool CGameScene::init()
{
	if (!Scene::init())
		return false;
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	//_client->on("chat", CC_CALLBACK_2(CLobbyScene::onReceivedEvent, this));
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
	cocos2d::SpriteBatchNode* spriteBatchNodeMenu = cocos2d::SpriteBatchNode::create("Menu.png");
	addChild(spriteBatchNodeMenu);
	

	// 배경화면
	auto spriteSEA = cocos2d::Sprite::create("SEA.png");
	spriteSEA->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2));
	spriteSEA->setScale(winSize.width / spriteSEA->getContentSize().width);
	this->addChild(spriteSEA, 0, TAG_SURFACE);

	//플레리어들 위치
	cocos2d::Sprite* spriteDOKDO1 = cocos2d::Sprite::create("DOKDO.png");
	spriteDOKDO1->setPosition(cocos2d::Vec2(winSize.width * 0.5, winSize.height * 0.1));
	this->addChild(spriteDOKDO1, TAG_DOKDO);

	//플레리어들 위치
	cocos2d::Sprite* spriteDOKDO2 = cocos2d::Sprite::create("DOKDO.png");
	spriteDOKDO2->setPosition(cocos2d::Vec2(winSize.width * 0.5, winSize.height * 0.9));
	this->addChild(spriteDOKDO2, TAG_DOKDO);


	mPtDokdo.setPoint(spriteDOKDO1->getPosition().x, spriteDOKDO1->getPosition().y);
	E_mPtDokdo.setPoint(spriteDOKDO2->getPosition().x, spriteDOKDO2->getPosition().y);
	

	//Surface SpriteBatchNode
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Surface.plist");
	cocos2d::SpriteBatchNode* spriteBatchNodeSurface = cocos2d::SpriteBatchNode::create("Surface.png");
	addChild(spriteBatchNodeSurface, ZORDER_SPRITE_BATCH_NODE_SURFACE, TAG_SPRITE_BATCH_NODE_SURFACE);

	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Air.plist");
	cocos2d::SpriteBatchNode* spriteBatchNodeAir = cocos2d::SpriteBatchNode::create("Air.png");
	addChild(spriteBatchNodeAir, ZORDER_SPRITE_BATCH_NODE_AIR, TAG_SPRITE_BATCH_NODE_AIR);

	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Interface.plist");
	cocos2d::SpriteBatchNode* spriteBatchNodeInterface = cocos2d::SpriteBatchNode::create("Interface.png");
	addChild(spriteBatchNodeInterface, ZORDER_SPRITE_BATCH_NODE_INTERFACE, TAG_SPRITE_BATCH_NODE_INTERFACE);

	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Explosion.plist");
	cocos2d::BlendFunc bfExplosion = { GL_ONE, GL_ONE };

	//지상 폭발용
	cocos2d::SpriteBatchNode* spriteBatchNodeExplosionS = cocos2d::SpriteBatchNode::create("Explosion.png");
	spriteBatchNodeExplosionS->setBlendFunc(bfExplosion);
	addChild(spriteBatchNodeExplosionS, ZORDER_SPRITE_BATCH_NODE_EXPLOSION_S, TAG_SPRITE_BATCH_NODE_EXPLOSION_S);

	//공중 폭발용
	cocos2d::SpriteBatchNode* spriteBatchNodeExplosionA = cocos2d::SpriteBatchNode::create("Explosion.png");
	spriteBatchNodeExplosionA->setBlendFunc(bfExplosion);
	addChild(spriteBatchNodeExplosionA, TAG_SPRITE_BATCH_NODE_EXPLOSION_A, TAG_SPRITE_BATCH_NODE_EXPLOSION_A);

	mVulcanCoolTime = mCannonCoolTime = mMissileCoolTime = 0;
	mIsShoot = false;

	CUser::getInst()->getClient()->on("EnemyShoot", CC_CALLBACK_2(CGameScene::E_shootFromDokdo, this));
	CUser::getInst()->getClient()->on("CreateShip", CC_CALLBACK_2(CGameScene::onCreateShip, this));
	CUser::getInst()->getClient()->on("moveShip", CC_CALLBACK_2(CGameScene::onMoveShip, this));
	CUser::getInst()->getClient()->on("killUnit", CC_CALLBACK_2(CGameScene::onUnitKill, this));
	CUser::getInst()->getClient()->on("positions", CC_CALLBACK_2(CGameScene::onPlayerPosition, this));
	CUser::getInst()->getClient()->on("CreateCloud", CC_CALLBACK_2(CGameScene::onCreateCloud, this));
	CUser::getInst()->getClient()->on("GameEnd", CC_CALLBACK_2(CGameScene::onGameEnd, this));
	CUser::getInst()->getClient()->on("enemyResult", CC_CALLBACK_2(CGameScene::onEnemyResult, this));
	CUser::getInst()->getClient()->on("userResult", CC_CALLBACK_2(CGameScene::onMyResult, this));
	CUser::getInst()->getClient()->on("showResult", CC_CALLBACK_2(CGameScene::showResult, this));
	CUser::getInst()->getClient()->on("playerInfo", CC_CALLBACK_2(CGameScene::playerInfo, this));
	CUser::getInst()->getClient()->emit("starts", "gameStart");

	schedule(schedule_selector(CGameScene::checkCollision));
	schedule(schedule_selector(CGameScene::checkEnemyCollision));
	schedule(schedule_selector(CGameScene::shoot));
	schedule(schedule_selector(CGameScene::moveShip));
	schedule(schedule_selector(CGameScene::updateWeaponInterface));


	addWeaponInterface("controller_img_cannon.png", "controller_text_cannon.png", cocos2d::Point(winSize.width * 0.7, 20), TAG_INTERFACE_CANNON);
	addWeaponInterface("controller_img_vulcan.png", "controller_text_vulcan.png", cocos2d::Point(winSize.width * 0.8, 40), TAG_INTERFACE_VULCAN);
	addWeaponInterface("controller_img_missile.png", "controller_text_missile.png", cocos2d::Point(winSize.width * 0.9, 60), TAG_INTERFACE_MISSILE);

	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	return true;
}

cocos2d::Scene * CGameScene::scene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	CGameScene* lobby = CGameScene::create();

	scene->addChild(lobby);
	return scene;
}

void CGameScene::onConnect(SIOClient * client)
{
	CCLOG("success!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

}

void CGameScene::onMessage(SIOClient * client, const std::string & data)
{
	CCLOG("%s", data);

}

void CGameScene::onClose(SIOClient * client)
{
	CCLOG("close!!!!!!!!!!!!!!!!!!!!!");

}

void CGameScene::onError(SIOClient * client, const std::string & data)
{
	CCLOG("error!!!!!!!!!!!!!!!!!!!!!");

}



void CGameScene::onCreateShip(SIOClient * client, const std::string & data)
{

	
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	auto ShipNumber = doc["ShipNumber"].GetInt();
	auto startXPos = doc["startXPos"].GetInt();
	auto startYPos = doc["startYPos"].GetInt();
	auto targetXPos = doc["targetXPos"].GetInt();
	auto targetYPos = doc["targetYPos"].GetInt();
	auto monsterNumber = doc["monsterNumber"].GetInt();
	auto speed = doc["speed"].GetInt();

	float length = sqrtf(powf(targetXPos - startXPos, 2) + powf(targetYPos - startYPos, 2)); // 길이
	cocos2d::Vec2 normalVec = cocos2d::Vec2((targetXPos - startXPos) / length, (targetYPos - startYPos) / length); // 방향벡터
	 
	
	switch (ShipNumber)
	{
	case CRUISER:
	{
		CUnit* cruiser = new CUnit();
		cruiser->body = cocos2d::Sprite::create("cruiser.png");
		cruiser->body->setTag(TAG_UNIT_E);
		cruiser->isEnermy = true;
		cruiser->body->setRotation(270);
		cruiser->eUnitType = AIRCRAFT;
		cruiser->setEnergy(HP_CRUISER);
		cruiser->UnitNumber = monsterNumber;
		cruiser->normalVec = normalVec;
		cruiser->speed = speed;
		cruiser->startPos.x = startXPos;
		cruiser->startPos.y = startYPos;
		cruiser->targetPos.x = targetXPos;
		cruiser->targetPos.y = targetYPos;
		setFirstPosition(cruiser->body, startXPos, startYPos);

		this->addChild(cruiser->body, ZORDER_SHIP);

		addUnit(cruiser);
	}
	break;
	case DESTROYER:
	{
		CUnit* destroyer = new CUnit();
		destroyer->body = cocos2d::Sprite::create("destroyer.png");
		destroyer->body->setTag(TAG_UNIT_E);
		destroyer->isEnermy = true;
		destroyer->body->setRotation(270);
		destroyer->eUnitType = AIRCRAFT;
		destroyer->setEnergy(HP_DESTROYER);
		destroyer->UnitNumber = monsterNumber;
		destroyer->normalVec = normalVec;
		destroyer->speed = speed;
		destroyer->startPos.x = startXPos;
		destroyer->startPos.y = startYPos;
		destroyer->targetPos.x = targetXPos;
		destroyer->targetPos.y = targetYPos;


		setFirstPosition(destroyer->body, startXPos, startYPos);

		this->addChild(destroyer->body, ZORDER_SHIP);

		addUnit(destroyer);
	}
	break;
	case FIGHTER:
	{
		CUnit* fighter = new CUnit();
		fighter->body = cocos2d::Sprite::createWithSpriteFrameName("fighter.png");
		fighter->body->setTag(TAG_UNIT_E);
		fighter->isEnermy = true;
		fighter->body->setRotation(270);
		fighter->eUnitType = AIRCRAFT;
		fighter->setEnergy(HP_FIGHTER);
		fighter->normalVec = normalVec;
		fighter->speed = speed;
		fighter->startPos.x = startXPos;
		fighter->startPos.y = startYPos;
		fighter->targetPos.x = targetXPos;
		fighter->targetPos.y = targetYPos;
		// 화염은 bullet.png를 이용한다.
		cocos2d::Sprite* spriteFire = cocos2d::Sprite::createWithSpriteFrameName("bullet.png");
		// 투명도를 주어 반투명하게 만든다.
		spriteFire->setOpacity(180);
		// 둥근 화염의 중심이 AnchorPoint가 되어야 Action을 주었을 때 자연스럽다.
		spriteFire->setAnchorPoint(cocos2d::Point(0.5, 0.1));
		spriteFire->setPosition(cocos2d::Point(17, 40));

		// 화염이 0.15초 동안 15% 작아진다.
		cocos2d::ScaleTo* scaleToSmall = cocos2d::ScaleTo::create(0.15, 0.85);
		// 화염이 0.15초 동안 15% 커진다.
		cocos2d::ScaleTo* scaleToBig = cocos2d::ScaleTo::create(0.15, 1.15);
		// 커지고 작아지는 동작은 순서대로 진행된다.
		cocos2d::Sequence* sequence = cocos2d::Sequence::create(scaleToSmall, scaleToBig, NULL);
		// 계속 반복한다.
		cocos2d::RepeatForever* repeatForever = cocos2d::RepeatForever::create(sequence);

		spriteFire->runAction(repeatForever);
		fighter->body->addChild(spriteFire);

		// 비행운을 날개의 왼쪽과 오른쪽 끝에 위치시켜 준다.
		cocos2d::Sprite* spriteContrailL = cocos2d::Sprite::createWithSpriteFrameName("tail.png");
		spriteContrailL->setAnchorPoint(cocos2d::Point(0.5, 0));
		spriteContrailL->setPosition(cocos2d::Point(0, 30));
		fighter->body->addChild(spriteContrailL);

		cocos2d::Sprite* spriteContrailR = cocos2d::Sprite::createWithSpriteFrameName("tail.png");
		spriteContrailR->setAnchorPoint(cocos2d::Point(0.5, 0));
		spriteContrailR->setPosition(cocos2d::Point(33, 30));
		fighter->body->addChild(spriteContrailR);
		fighter->UnitNumber = monsterNumber;

		setFirstPosition(fighter->body, startXPos, startYPos);

		getChildByTag(TAG_SPRITE_BATCH_NODE_AIR)->addChild(fighter->body, ZORDER_FIGHTER);

		
		addUnit(fighter);
	}
		break;
	case STRATEGYBOMBER:
	{
		CUnit* b52 = new CUnit();
		b52->body = cocos2d::Sprite::createWithSpriteFrameName("b52h.png");
		b52->body->setTag(TAG_UNIT_E);
		b52->isEnermy = true;
		b52->body->setRotation(270);
		b52->eUnitType = AIRCRAFT;
		b52->setEnergy(HP_STRATEGYBOMBER);
		b52->UnitNumber = monsterNumber;
		b52->normalVec = normalVec;
		b52->body->setScale(b52->body->getScale() * 0.5);
		b52->speed = speed;
		b52->startPos.x = startXPos;
		b52->startPos.y = startYPos;
		b52->targetPos.x = targetXPos;
		b52->targetPos.y = targetYPos;
		cocos2d::Sprite* spriteContrailL = cocos2d::Sprite::createWithSpriteFrameName("tail.png");
		spriteContrailL->setAnchorPoint(cocos2d::Point(0.5, 0));
		spriteContrailL->setPosition(cocos2d::Point(1, 80));
		b52->body->addChild(spriteContrailL);

		cocos2d::Sprite* spriteContrailR = cocos2d::Sprite::createWithSpriteFrameName("tail.png");
		spriteContrailR->setAnchorPoint(cocos2d::Point(0.5, 0));
		spriteContrailR->setPosition(cocos2d::Point(130, 80));
		b52->body->addChild(spriteContrailR);

		setFirstPosition(b52->body, startXPos, startYPos);

		getChildByTag(TAG_SPRITE_BATCH_NODE_AIR)->addChild(b52->body, ZORDER_STRATEGYBOMBER);

		addUnit(b52);
	}
		break;
	case Bomber:
	{
		CUnit* bomber = new CUnit();
		bomber->body = cocos2d::Sprite::createWithSpriteFrameName("bomber.png");
		bomber->body->setTag(TAG_UNIT_E);
		bomber->isEnermy = true;
		bomber->eUnitType = AIRCRAFT;
		bomber->setEnergy(HP_BOMBER);
		bomber->body->setRotation(270);
		bomber->UnitNumber = monsterNumber;
		cocos2d::Sprite* spriteContrailL = cocos2d::Sprite::createWithSpriteFrameName("tail.png");
		spriteContrailL->setAnchorPoint(cocos2d::Point(0.5, 0));
		spriteContrailL->setPosition(cocos2d::Point(0, 20));
		bomber->body->addChild(spriteContrailL);
		bomber->normalVec = normalVec;
		bomber->speed = speed;
		bomber->startPos.x = startXPos;
		bomber->startPos.y = startYPos;
		bomber->targetPos.x = targetXPos;
		bomber->targetPos.y = targetYPos;
		cocos2d::Sprite* spriteContrailR = cocos2d::Sprite::createWithSpriteFrameName("tail.png");
		spriteContrailR->setAnchorPoint(cocos2d::Point(0.5, 0));
		spriteContrailR->setPosition(cocos2d::Point(45, 20));
		bomber->body->addChild(spriteContrailR);
		setFirstPosition(bomber->body, startXPos, startYPos);
		getChildByTag(TAG_SPRITE_BATCH_NODE_AIR)->addChild(bomber->body, ZORDER_BOMBER);
		//bezierToDokdo(bomber->body, BOMBER_MOVE_TIME, targetXPos, targetYPos);
		addUnit(bomber);
	}
	break;
	}

}

void CGameScene::setFirstPosition(cocos2d::Sprite * sprite, int x , int y)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	

	float xPos = x;
	float yPos = y;

	sprite->setPosition(cocos2d::Point(xPos, yPos));
	sprite->setAnchorPoint(cocos2d::Point(0.5, 0));
}

void CGameScene::onMoveShip(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	auto currentXpos = doc["targetXPos"].GetFloat();
	auto currentYpos = doc["targetYPos"].GetFloat();
	auto  UnitNumber = doc["monsterNumber"].GetInt();
	
	arrUnitE.find(UnitNumber)->second->body->setPosition(cocos2d::Point(currentXpos, currentYpos));


}

void CGameScene::onUnitKill(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	int value= doc["number"].GetInt();

	for (std::unordered_map<int, CUnit*>::iterator iterUnit = arrUnitE.begin();
		iterUnit != arrUnitE.end();
		iterUnit++)
	{
		CUnit* unit = (CUnit*)iterUnit->second;
		if (unit->UnitNumber == value)
		{
			explosionEffect1();
			cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
			float wRatio = unit->body->getBoundingBox().size.height * 2;
			wRatio /= winSize.width;
			cocos2d::Point pt = unit->body->getPosition();
			//pt.y += unit->body->getBoundingBox().size.height / 2;
			pt.x -= unit->body->getBoundingBox().size.width / 2;
			switch (unit->eUnitType)
			{
			case SUBMARINE:
				break;
			case SHIP:
			case HELICOPTER:
				addExplosion(pt, wRatio, 0.05, false);
				break;
			case AIRCRAFT:
				addExplosion(pt, wRatio, 0.05, true);
				break;
			}

			unit->release();
			delete unit;
			iterUnit = arrUnitE.erase(iterUnit);
	
			if (iterUnit == arrUnitE.end())
				break;
		}
	}
	
}

void CGameScene::onPlayerPosition(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	position = doc["position"].GetInt();


	if (position == 1)
	{
		auto pLabel1 = cocos2d::Label::createWithBMFont("konqa32.fnt", CUser::getInst()->getName());
		pLabel1->setScale(2);
		pLabel1->setColor(cocos2d::Color3B(0, 0, 0));
		pLabel1->setPosition(cocos2d::Vec2(200, 40));
		this->addChild(pLabel1, 3);
	}
	else if (position == 2)
	{
		auto pLabel1 = cocos2d::Label::createWithBMFont("konqa32.fnt", CUser::getInst()->getName());
		pLabel1->setScale(2);
		pLabel1->setColor(cocos2d::Color3B(0, 0, 0));
		pLabel1->setPosition(cocos2d::Vec2(200, 380));
		this->addChild(pLabel1, 3);


	}
}




void CGameScene::shootFromDokdo()
{ 
	if (true == mIsShoot)
	{
		if (mVulcanCoolTime >= DOKDO_VULCAN_COOLTIME)
		{
			mVulcanCoolTime = 0;
			fireBullet(DOKDO_VULCAN_DAMAGE , VULCAN, true);
		}
		if (mCannonCoolTime >= DOKDO_CANNON_COOLTIME)
		{
			mCannonCoolTime = 0;

			if (CUser::getInst()->getCannonCnt() > 0)
			{
				fireBullet(DOKDO_CANNON_DAMAGE, CANNON, true);
				int cnt = CUser::getInst()->getCannonCnt();
				cnt--;
				CUser::getInst()->setCannonCnt(cnt);
			}
		}
		if (mMissileCoolTime >= DOKDO_MISSILE_COOLTIME)
		{
			mMissileCoolTime = 0;
			if (CUser::getInst()->getMissileCnt() > 0)
			{
				fireMissile(DOKDO_MISSILE_DAMAGE, false);
				int cnt = CUser::getInst()->getMissileCnt();

				cnt--;
				CUser::getInst()->setMissileCnt(cnt);

			}
		}
	}
}

void CGameScene::E_shootFromDokdo(SIOClient* client, const std::string& data)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	//enermyGunEffect();
	if (position == 1)
	{
		float speed;
		cocos2d::Vec2 startPos, targetPos, vec;
		bool bEnemy;
		int damage;

		std::string weaponType;
		rapidjson::Document doc;
		doc.Parse<0>(data.c_str());
		
		rapidjson::Value &val = doc["weaponType"];
		std::string value = val.GetString();
		weaponType = value;

		if (weaponType == "VULCAN")
		{
			CWeapon* bullet = new CWeapon();
			bullet->body = cocos2d::Sprite::createWithSpriteFrameName("bullet.png");
			bullet->speed = doc["speed"].GetFloat();
			bullet->startPos.x = E_mPtDokdo.x;
			bullet->startPos.y = E_mPtDokdo.y;
			bullet->targetPos.x = doc["targetXPos"].GetFloat();
			bullet->targetPos.y = doc["targetYPos"].GetFloat();
			bullet->vec.x = doc["normalXPos"].GetFloat();
			bullet->vec.y = doc["normalXPos"].GetFloat();
			bullet->isEnermy = true;
			bullet->damage = 1;
			bullet->body->setPosition(cocos2d::Vec2(E_mPtDokdo.x, E_mPtDokdo.y));
			bullet->eWeaponType = VULCAN;
			float length = sqrtf(powf(bullet->targetPos.x - bullet->startPos.x, 2) + powf(bullet->targetPos.y - bullet->startPos.y, 2)); // 길이
			bullet->vec = cocos2d::Vec2((bullet->targetPos.x - bullet->startPos.x) / length, (bullet->targetPos.y - bullet->startPos.y) / length); // 방향벡터
			bullet->body->setPosition(cocos2d::Vec2(E_mPtDokdo.x, E_mPtDokdo.y)); // 위치

			float scaleH = 0.02;
			bullet->body->setScale(winSize.height*scaleH / bullet->body->getContentSize().height);

			// 각 계산
			float angle = (float)atanf((E_mPtDokdo.x - bullet->targetPos.x) / (E_mPtDokdo.y - bullet->targetPos.y)) * (180.f / M_PI);
			if (mPtShoot.y > mPtDokdo.y)
				bullet->body->setRotation(angle);
			else
				bullet->body->setRotation(angle - 180);

			addChild(bullet->body);
			addWeapon(bullet);
		}
		else if (weaponType == "CANNON")
		{
			CWeapon* cannon = new CWeapon();
			cannon->body = cocos2d::Sprite::createWithSpriteFrameName("bullet.png");
			cannon->speed = doc["speed"].GetFloat();
			cannon->startPos.x = E_mPtDokdo.x;
			cannon->startPos.y = E_mPtDokdo.y;
			cannon->targetPos.x = doc["targetXPos"].GetFloat();
			cannon->targetPos.y = doc["targetYPos"].GetFloat();
			cannon->vec.x = doc["normalXPos"].GetFloat();
			cannon->vec.y = doc["normalXPos"].GetFloat();
			cannon->isEnermy = true;
			cannon->damage = DOKDO_CANNON_DAMAGE;
			cannon->body->setPosition(cocos2d::Vec2(E_mPtDokdo.x, E_mPtDokdo.y));
			cannon->eWeaponType = CANNON;

			float length = sqrtf(powf(cannon->targetPos.x - cannon->startPos.x, 2) + powf(cannon->targetPos.y - cannon->startPos.y, 2)); // 길이
			cannon->vec = cocos2d::Vec2((cannon->targetPos.x - cannon->startPos.x) / length, (cannon->targetPos.y - cannon->startPos.y) / length); // 방향벡터
			cannon->body->setPosition(cocos2d::Vec2(E_mPtDokdo.x, E_mPtDokdo.y)); // 위치

			float scaleH = 0.02;
			cannon->body->setScale(winSize.height*scaleH / cannon->body->getContentSize().height);

			// 각 계산
			float angle = (float)atanf((E_mPtDokdo.x - cannon->targetPos.x) / (E_mPtDokdo.y - cannon->targetPos.y)) * (180.f / M_PI);
			if (mPtShoot.y > mPtDokdo.y)
				cannon->body->setRotation(angle);
			else
				cannon->body->setRotation(angle - 180);

			addChild(cannon->body);
			addWeapon(cannon);
		}
		else if (weaponType == "MISSILE")
		{
			CWeapon* missile = new CWeapon();
			missile->body = cocos2d::Sprite::createWithSpriteFrameName("MISSILE.png");
			missile->speed = doc["speed"].GetFloat();
			missile->startPos.x = E_mPtDokdo.x;
			missile->startPos.y = E_mPtDokdo.y;
			missile->targetPos.x = doc["targetXPos"].GetFloat();
			missile->targetPos.y = doc["targetYPos"].GetFloat();
			missile->vec.x = doc["normalXPos"].GetFloat();
			missile->vec.y = doc["normalXPos"].GetFloat();
			missile->isEnermy = true;
			missile->damage = DOKDO_MISSILE_DAMAGE;
			missile->body->setPosition(cocos2d::Vec2(E_mPtDokdo.x, E_mPtDokdo.y));
			missile->eWeaponType = MISSILE;

			float length = sqrtf(powf(missile->targetPos.x - missile->startPos.x, 2) + powf(missile->targetPos.y - missile->startPos.y, 2)); // 길이
			missile->vec = cocos2d::Vec2((missile->targetPos.x - missile->startPos.x) / length, (missile->targetPos.y - missile->startPos.y) / length); // 방향벡터
			missile->body->setPosition(cocos2d::Vec2(E_mPtDokdo.x, E_mPtDokdo.y)); // 위치

	
			// 각 계산
			float angle = (float)atanf((E_mPtDokdo.x - missile->targetPos.x) / (E_mPtDokdo.y - missile->targetPos.y)) * (180.f / M_PI);
			if (mPtShoot.y > mPtDokdo.y)
				missile->body->setRotation(angle);
			else
				missile->body->setRotation(angle - 180);

			cocos2d::Sprite* tail = cocos2d::Sprite::createWithSpriteFrameName("bullet.png");
			tail->setOpacity(180);
			tail->setAnchorPoint(cocos2d::Point(0.5, 0.1));
			tail->setRotation(180);
			tail->setPosition(cocos2d::Point(3, 0));
			missile->body->addChild(tail);


			addChild(missile->body);
			addWeapon(missile);
		}
		
	}
	else if (position == 2)
	{

		float speed;
		cocos2d::Vec2 startPos, targetPos, vec;
		bool bEnemy;
		int damage;

		std::string weaponType;
		rapidjson::Document doc;
		doc.Parse<0>(data.c_str());
		rapidjson::Value &val = doc["weaponType"];
		std::string value = val.GetString();
		weaponType = value;


		if (weaponType == "VULCAN")
		{
			CWeapon* bullet = new CWeapon();
			bullet->body = cocos2d::Sprite::createWithSpriteFrameName("bullet.png");
			bullet->speed = doc["speed"].GetFloat();
			bullet->startPos.x = doc["startXPos"].GetFloat();
			bullet->startPos.y = doc["startYPos"].GetFloat();
			bullet->targetPos.x = doc["targetXPos"].GetFloat();
			bullet->targetPos.y = doc["targetYPos"].GetFloat();
			bullet->vec.x = doc["normalXPos"].GetFloat();
			bullet->vec.y = doc["normalXPos"].GetFloat();
			bullet->isEnermy = true;
			bullet->damage = 1;
			bullet->body->setPosition(cocos2d::Vec2(bullet->startPos.x, bullet->startPos.y));
			bullet->eWeaponType = VULCAN;

			float length = sqrtf(powf(bullet->targetPos.x - bullet->startPos.x, 2) + powf(bullet->targetPos.y - bullet->startPos.y, 2)); // 길이
			bullet->vec = cocos2d::Vec2((bullet->targetPos.x - bullet->startPos.x) / length, (bullet->targetPos.y - bullet->startPos.y) / length); // 방향벡터
			bullet->body->setPosition(cocos2d::Vec2(mPtDokdo.x, mPtDokdo.y)); // 위치

			float scaleH = 0.02;
			bullet->body->setScale(winSize.height*scaleH / bullet->body->getContentSize().height);

			// 각 계산
			float angle = (float)atanf((mPtDokdo.x - bullet->targetPos.x) / (mPtDokdo.y - bullet->targetPos.y)) * (180.f / M_PI);
			if (bullet->targetPos.y > mPtDokdo.y)
				bullet->body->setRotation(angle);
			else
				bullet->body->setRotation(angle - 180);

			addChild(bullet->body);
			addWeapon(bullet);
		}
		else if (weaponType == "CANNON")
		{
			CWeapon* cannon = new CWeapon();
			cannon->body = cocos2d::Sprite::createWithSpriteFrameName("bullet.png");
			cannon->speed = doc["speed"].GetFloat();
			cannon->startPos.x = doc["startXPos"].GetFloat();
			cannon->startPos.y = doc["startYPos"].GetFloat();
			cannon->targetPos.x = doc["targetXPos"].GetFloat();
			cannon->targetPos.y = doc["targetYPos"].GetFloat();
			cannon->vec.x = doc["normalXPos"].GetFloat();
			cannon->vec.y = doc["normalXPos"].GetFloat();
			cannon->isEnermy = true;
			cannon->damage = DOKDO_CANNON_DAMAGE;
			cannon->body->setPosition(cocos2d::Vec2(cannon->startPos.x, cannon->startPos.y));
			cannon->eWeaponType = CANNON;

			float length = sqrtf(powf(cannon->targetPos.x - cannon->startPos.x, 2) + powf(cannon->targetPos.y - cannon->startPos.y, 2)); // 길이
			cannon->vec = cocos2d::Vec2((cannon->targetPos.x - cannon->startPos.x) / length, (cannon->targetPos.y - cannon->startPos.y) / length); // 방향벡터
			cannon->body->setPosition(cocos2d::Vec2(mPtDokdo.x, mPtDokdo.y)); // 위치

			float scaleH = 0.02;
			cannon->body->setScale(winSize.height*scaleH / cannon->body->getContentSize().height);

			// 각 계산
			float angle = (float)atanf((mPtDokdo.x - cannon->targetPos.x) / (mPtDokdo.y - cannon->targetPos.y)) * (180.f / M_PI);
			if (cannon->targetPos.y > mPtDokdo.y)
				cannon->body->setRotation(angle);
			else
				cannon->body->setRotation(angle - 180);

			addChild(cannon->body);
			addWeapon(cannon);
		}
		else if (weaponType == "MISSILE")
		{
			CWeapon* missile = new CWeapon();
			missile->body = cocos2d::Sprite::createWithSpriteFrameName("MISSILE.png");
			missile->speed = doc["speed"].GetFloat();
			missile->startPos.x = doc["startXPos"].GetFloat();
			missile->startPos.y = doc["startYPos"].GetFloat();
			missile->targetPos.x = doc["targetXPos"].GetFloat();
			missile->targetPos.y = doc["targetYPos"].GetFloat();
			missile->vec.x = doc["normalXPos"].GetFloat();
			missile->vec.y = doc["normalXPos"].GetFloat();
			missile->isEnermy = true;
			missile->damage = DOKDO_MISSILE_DAMAGE;
			missile->body->setPosition(cocos2d::Vec2(missile->startPos.x, missile->startPos.y));
			missile->eWeaponType = MISSILE;

			float length = sqrtf(powf(missile->targetPos.x - missile->startPos.x, 2) + powf(missile->targetPos.y - missile->startPos.y, 2)); // 길이
			missile->vec = cocos2d::Vec2((missile->targetPos.x - missile->startPos.x) / length, (missile->targetPos.y - missile->startPos.y) / length); // 방향벡터
			missile->body->setPosition(cocos2d::Vec2(mPtDokdo.x, mPtDokdo.y)); // 위치

			// 각 계산
			float angle = (float)atanf((mPtDokdo.x - missile->targetPos.x) / (mPtDokdo.y - missile->targetPos.y)) * (180.f / M_PI);
			if (missile->targetPos.y > mPtDokdo.y)
				missile->body->setRotation(angle);
			else
				missile->body->setRotation(angle - 180);

			cocos2d::Sprite* tail = cocos2d::Sprite::createWithSpriteFrameName("bullet.png");
			tail->setOpacity(180);
			tail->setAnchorPoint(cocos2d::Point(0.5, 0.1));
			tail->setRotation(180);
			tail->setPosition(cocos2d::Point(3, 0));
			missile->body->addChild(tail);

			addChild(missile->body);
			addWeapon(missile);
		}
	}

}

void CGameScene::onCreateCloud(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	auto cloundID = doc["cloudNumber"].GetInt();
	auto startXPos = doc["startXPos"].GetInt();
	auto startYPos = doc["startYPos"].GetInt();
	auto targetXPos = doc["targetXPos"].GetInt();
	auto targetYPos = doc["targetYPos"].GetInt();

	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	//SpriteBatchNode* spriteBatchNodeSurface = (SpriteBatchNode*)getChildByTag(TAG_SPRITE_BATCH_NODE_SURFACE);

	//3가지 이미지중 하나를 고른다.
	int cloudNum = cloundID;
	char szFileName[16] = { 0, };
	sprintf(szFileName, "cloud_%02d.png", cloudNum);
	cocos2d::Sprite* spriteCloud = cocos2d::Sprite::create(szFileName);
	spriteCloud->setPosition(cocos2d::Vec2(startXPos, startYPos));
	//구름의 크기를 랜덤하게 적용한다.
	float frand = ((rand() % 4) + 6) / 10.f;
	float scale = (winSize.width * frand) / spriteCloud->getContentSize().width;
	spriteCloud->setScale(scale);
	spriteCloud->setAnchorPoint(cocos2d::Point(1, 0.5));

	spriteCloud->setPosition(cocos2d::Vec2(startXPos, startYPos));

	//Multiply Blending 적용
	cocos2d::BlendFunc bf = { GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA };
	spriteCloud->setBlendFunc(bf);

	//구름의 이동속도를 랜덤하게 선택하여 MoveBy Action
	float moveTime = 8 + (rand() % 3);
	cocos2d::MoveBy* moveBy = cocos2d::MoveBy::create(moveTime, cocos2d::Point(targetXPos+300, targetYPos));

	//구름의 이동이 동료되면 삭제되도록 
	cocos2d::CallFuncN* callfunc = cocos2d::CallFuncN::create(CC_CALLBACK_1(CGameScene::selfRemover, this));

	//이동과 callback을 순차적으로 일어나도록 Sequence로 묶어준다.
	cocos2d::Sequence* sequence = cocos2d::Sequence::create(moveBy, callfunc, NULL);

	//Action 실행
	spriteCloud->runAction(sequence);

	//화면에 추가
	addChild(spriteCloud, ZORDER_CLOUD, TAG_SPRITE_BATCH_NODE_SURFACE);
}

void CGameScene::onGameEnd(SIOClient * client, const std::string & data)
{
	CUser::getInst()->getClient()->emit("gameEnd", "gameEND");
}

void CGameScene::onEnemyResult(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	CUser::getInst()->enemyName = doc["enemyName"].GetString();
	CUser::getInst()->enemyScore= doc["enemyScore"].GetInt();
}

void CGameScene::onMyResult(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	CUser::getInst()->gameScore = doc["userScore"].GetInt();


}

void CGameScene::showResult(SIOClient * client, const std::string & data)
{
	std::string result = std::to_string(CUser::getInst()->gameScore)+ " " + std::to_string(CUser::getInst()->enemyScore);
	CUser::getInst()->getClient()->emit("result", result);
	showScoreDlg();
}

void CGameScene::playerInfo(SIOClient * client, const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	CUser::getInst()->setMissileCnt( doc["missileCnt"].GetInt());
	CUser::getInst()->setCannonCnt(doc["cannonCnt"].GetInt());
}

void CGameScene::hideScoreDlg()
{
	cocos2d::Director::getInstance()->popScene();

}

void CGameScene::showScoreDlg()
{
	CScoreBoard* scoreBoard = CScoreBoard::create();

	scoreBoard->setVisible(true);
	addChild(scoreBoard, 10, TAG_OPTION_DLG);
	if (CUser::getInst()->getMissileCnt() < 0)
	{
		CUser::getInst()->setMissileCnt(0);
	}
	else if (CUser::getInst()->getCannonCnt() < 0)
	{
		CUser::getInst()->setCannonCnt(0);
	}
	std::string data = std::to_string(CUser::getInst()->getMissileCnt()) + " " + std::to_string(CUser::getInst()->getCannonCnt());
	CUser::getInst()->getClient()->emit("gameOver", data);
}

void CGameScene::addShootFire(cocos2d::Point pt, float hRatio, float delayPerUnit, bool surface, float angle)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	cocos2d::Sprite* spriteShootFire = cocos2d::Sprite::createWithSpriteFrameName("dok_01.png");
	spriteShootFire->setPosition(pt);
	spriteShootFire->setScale((winSize.height * hRatio) / spriteShootFire->getContentSize().height);
	spriteShootFire->setRotation(angle);
	spriteShootFire->setAnchorPoint(cocos2d::Point(0.5, 0.2));

	if (true == surface)
		getChildByTag(TAG_SPRITE_BATCH_NODE_EXPLOSION_S)->addChild(spriteShootFire);
	else
		getChildByTag(TAG_SPRITE_BATCH_NODE_EXPLOSION_A)->addChild(spriteShootFire);

	cocos2d::SpriteFrameCache* frameCache = cocos2d::SpriteFrameCache::getInstance();
	cocos2d::Animation* animation = cocos2d::Animation::create();
	char szFile[64] = { 0, };
	for (int i = 1; i <= 6; i++)
	{
		sprintf(szFile, "dok_%02d.png", i);
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation->setDelayPerUnit(delayPerUnit);

	cocos2d::Animate* animate = cocos2d::Animate::create(animation);
	cocos2d::CallFuncN* callfunc = cocos2d::CallFuncN::create(CC_CALLBACK_1(CGameScene::selfRemover, this));
	cocos2d::Sequence* sequence = cocos2d::Sequence::create(animate, callfunc, NULL);
	spriteShootFire->runAction(sequence);

}

void CGameScene::fireBullet(float damage, WEAPON_TYPE type, bool surface)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	gunEffect1();
	if (mIsShoot == true)
	{
		if (position == 1)
		{

			CWeapon* bullet = new CWeapon();
			bullet->body = cocos2d::Sprite::createWithSpriteFrameName("bullet.png");
			bullet->speed = 300.f;
			bullet->damage = damage;
			bullet->body->setPosition(cocos2d::Vec2(mPtDokdo.x, mPtDokdo.y));
			bullet->isEnermy = false;
			bullet->targetPos = mPtShoot;
			bullet->startPos = mPtDokdo;
			bullet->eWeaponType = type;
			float length = sqrtf(powf(bullet->targetPos.x - bullet->startPos.x, 2) + powf(bullet->targetPos.y - bullet->startPos.y, 2)); // 길이
			bullet->vec = cocos2d::Vec2((bullet->targetPos.x - bullet->startPos.x) / length, (bullet->targetPos.y - bullet->startPos.y) / length); // 방향벡터
			bullet->body->setPosition(cocos2d::Vec2(mPtDokdo.x, mPtDokdo.y)); // 위치

			float scaleH = 0.02;
			bullet->body->setScale(winSize.height*scaleH / bullet->body->getContentSize().height);

			// 각 계산
			float angle = (float)atanf((mPtDokdo.x - mPtShoot.x) / (mPtDokdo.y - mPtShoot.y)) * (180.f / M_PI);
			if (mPtShoot.y > mPtDokdo.y)
				bullet->body->setRotation(angle);
			else
				bullet->body->setRotation(angle - 180);

			addChild(bullet->body);
			addWeapon(bullet);

			std::string weaponType = "";
			if (type == VULCAN)
				weaponType = "VULCAN";
			else if (type == CANNON)
				weaponType = "CANNON";

			std::string bulletInfo = std::to_string(bullet->speed) + " " + std::to_string(bullet->startPos.x) + " " + std::to_string(bullet->startPos.y) + " " + std::to_string(bullet->targetPos.x) + " " + std::to_string(bullet->targetPos.y) + " " + std::to_string(bullet->vec.x) + " " + std::to_string(bullet->vec.y)+ " " + weaponType;

			addShootFire(mPtDokdo, (type == VULCAN ? 0.08 : 0.12), (type == VULCAN ? 0.06 : 0.08), surface, angle);

			CUser::getInst()->getClient()->emit("bulletInfo", bulletInfo);

		}
		else if (position == 2)
		{

			CWeapon* bullet = new CWeapon();
			bullet->body = cocos2d::Sprite::createWithSpriteFrameName("bullet.png");
			bullet->speed = 300.f;
			bullet->body->setPosition(cocos2d::Vec2(E_mPtDokdo.x, E_mPtDokdo.y));
			bullet->isEnermy = false;
			bullet->damage = damage;
			bullet->targetPos = mPtShoot;
			bullet->startPos = E_mPtDokdo;
			bullet->eWeaponType = type;
			float length = sqrtf(powf(bullet->targetPos.x - bullet->startPos.x, 2) + powf(bullet->targetPos.y - bullet->startPos.y, 2)); // 길이
			bullet->vec = cocos2d::Vec2((bullet->targetPos.x - bullet->startPos.x) / length, (bullet->targetPos.y - bullet->startPos.y) / length); // 방향벡터

			float scaleH = 0.02;
			bullet->body->setScale(winSize.height*scaleH / bullet->body->getContentSize().height);

			// 각 계산
			float angle = (float)atanf((E_mPtDokdo.x - mPtShoot.x) / (E_mPtDokdo.y - mPtShoot.y)) * (180.f / M_PI);
			if (mPtShoot.y > E_mPtDokdo.y)
				bullet->body->setRotation(angle);
			else
				bullet->body->setRotation(angle - 180);

			addChild(bullet->body);
			addWeapon(bullet);
			std::string weaponType = "";
			if (type == VULCAN)
				weaponType = "VULCAN";
			else if (type == CANNON)
				weaponType = "CANNON";
		


			std::string bulletInfo = std::to_string(bullet->speed) + " " + std::to_string(bullet->startPos.x) + " " + std::to_string(bullet->startPos.y) + " " + std::to_string(bullet->targetPos.x) + " " + std::to_string(bullet->targetPos.y) + " " + std::to_string(bullet->vec.x) + " " + std::to_string(bullet->vec.y) + " " + weaponType;
			addShootFire(E_mPtDokdo, (type == VULCAN ? 0.08 : 0.12), (type == VULCAN ? 0.06 : 0.08), surface, angle);

			CUser::getInst()->getClient()->emit("bulletInfo", bulletInfo);

		}
	}

}

void CGameScene::addWeaponInterface(const char * imgPath, const char * txtPath, cocos2d::Point pt, int tag)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();

	//버튼의 배경 이미지
	cocos2d::Sprite* spriteBK = cocos2d::Sprite::createWithSpriteFrameName("controller_back.png");
	spriteBK->setAnchorPoint(cocos2d::Point(0.5, 0));
	spriteBK->setPosition(pt);
	spriteBK->setTag(tag);
	getChildByTag(TAG_SPRITE_BATCH_NODE_INTERFACE)->addChild(spriteBK);

	//버튼 하단 테스트
	cocos2d::Sprite* spriteText = cocos2d::Sprite::createWithSpriteFrameName(txtPath);
	spriteText->setAnchorPoint(cocos2d::Point(0.5, 0));
	spriteText->setPosition(cocos2d::Point(47, 0));
	spriteBK->addChild(spriteText);

	//쿨타임 표현 녹색 원
	cocos2d::Sprite* spriteActivity = cocos2d::Sprite::createWithSpriteFrameName("controller_active.png");
	spriteActivity->setPosition(cocos2d::Point(42, 62));
	spriteActivity->setOpacity(120);
	spriteActivity->setTag(TAG_INTERFACE_ACTIVATE);
	spriteBK->addChild(spriteActivity);

	//무기 이미지
	cocos2d::Sprite* spriteWeapon = cocos2d::Sprite::createWithSpriteFrameName(imgPath);
	spriteWeapon->setPosition(cocos2d::Point(42, 62));
	spriteBK->addChild(spriteWeapon);

}


void CGameScene::addExplosion(cocos2d::Point pt, float wRatio, float delayPerUnit, bool surface)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	cocos2d::Sprite* spriteExplosion = cocos2d::Sprite::createWithSpriteFrameName("explosion_effect_00.png");
	spriteExplosion->setPosition(pt);

	//wRatio로부터 winSize.width에 대한 Scale계산
	spriteExplosion->setScale((winSize.width * wRatio) / spriteExplosion->getContentSize().width);
	if (surface)
		getChildByTag(TAG_SPRITE_BATCH_NODE_EXPLOSION_S)->addChild(spriteExplosion);
	else
		getChildByTag(TAG_SPRITE_BATCH_NODE_EXPLOSION_A)->addChild(spriteExplosion);

	cocos2d::SpriteFrameCache* frameCache = cocos2d::SpriteFrameCache::getInstance();
	cocos2d::Animation* animation = cocos2d::Animation::create();
	char szFile[64] = { 0, };
	for (int i = 0; i < 36; i++)
	{
		sprintf(szFile, "explosion_effect_%02d.png", i);
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation->setDelayPerUnit(delayPerUnit);

	cocos2d::Animate* animate = cocos2d::Animate::create(animation);
	cocos2d::CallFuncN* callfunc = cocos2d::CallFuncN::create(CC_CALLBACK_1(CGameScene::selfRemover, this));
	cocos2d::Sequence* sequence = cocos2d::Sequence::create(animate, callfunc, NULL);
	spriteExplosion->runAction(sequence);
}

void CGameScene::fireMissile(float damage, bool enemy)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	gunEffect1();
	if (mIsShoot == true)
	{
		if (position == 1)
		{

			CWeapon* missile = new CWeapon();
			missile->body = cocos2d::Sprite::createWithSpriteFrameName("MISSILE.png");
			missile->speed = 300.f;
			missile->damage = DOKDO_MISSILE_DAMAGE;
			missile->body->setPosition(cocos2d::Vec2(mPtDokdo.x, mPtDokdo.y));
			missile->isEnermy = false;
			missile->targetPos = mPtShoot;
			missile->startPos = mPtDokdo;
			missile->eWeaponType = MISSILE;
			float length = sqrtf(powf(missile->targetPos.x - missile->startPos.x, 2) + powf(missile->targetPos.y - missile->startPos.y, 2)); // 길이
			missile->vec = cocos2d::Vec2((missile->targetPos.x - missile->startPos.x) / length, (missile->targetPos.y - missile->startPos.y) / length); // 방향벡터
			missile->body->setPosition(cocos2d::Vec2(mPtDokdo.x, mPtDokdo.y)); // 위치

	
			// 각 계산
			float angle = (float)atanf((mPtDokdo.x - mPtShoot.x) / (mPtDokdo.y - mPtShoot.y)) * (180.f / M_PI);
			if (mPtShoot.y > mPtDokdo.y)
				missile->body->setRotation(angle);
			else
				missile->body->setRotation(angle - 180);

			addChild(missile->body);
			addWeapon(missile);

			cocos2d::Sprite* tail = cocos2d::Sprite::createWithSpriteFrameName("bullet.png");
			tail->setOpacity(180);
			tail->setAnchorPoint(cocos2d::Point(0.5, 0.1));
			tail->setRotation(180);
			tail->setPosition(cocos2d::Point(3, 0));
			missile->body->addChild(tail);


			std::string weaponType = "";
			weaponType = "MISSILE";


			std::string bulletInfo = std::to_string(missile->speed) + " " + std::to_string(missile->startPos.x) + " " + std::to_string(missile->startPos.y) + " " + std::to_string(missile->targetPos.x) + " " + std::to_string(missile->targetPos.y) + " " + std::to_string(missile->vec.x) + " " + std::to_string(missile->vec.y) + " " + weaponType;

			CUser::getInst()->getClient()->emit("bulletInfo", bulletInfo);

		}
		else if (position == 2)
		{

			CWeapon* missile = new CWeapon();
			missile->body = cocos2d::Sprite::createWithSpriteFrameName("MISSILE.png");
			missile->speed = 300.f;
			missile->damage = DOKDO_MISSILE_DAMAGE;
			missile->body->setPosition(cocos2d::Vec2(E_mPtDokdo.x, E_mPtDokdo.y));
			missile->isEnermy = false;
			missile->targetPos = mPtShoot;
			missile->startPos = E_mPtDokdo;
			missile->eWeaponType = MISSILE;
			float length = sqrtf(powf(missile->targetPos.x - missile->startPos.x, 2) + powf(missile->targetPos.y - missile->startPos.y, 2)); // 길이
			missile->vec = cocos2d::Vec2((missile->targetPos.x - missile->startPos.x) / length, (missile->targetPos.y - missile->startPos.y) / length); // 방향벡터

	
			// 각 계산
			float angle = (float)atanf((E_mPtDokdo.x - mPtShoot.x) / (E_mPtDokdo.y - mPtShoot.y)) * (180.f / M_PI);
			if (mPtShoot.y > E_mPtDokdo.y)
				missile->body->setRotation(angle);
			else
				missile->body->setRotation(angle - 180);

			addChild(missile->body);
			addWeapon(missile);

			cocos2d::Sprite* tail = cocos2d::Sprite::createWithSpriteFrameName("bullet.png");
			tail->setOpacity(180);
			tail->setAnchorPoint(cocos2d::Point(0.5, 0.1));
			tail->setRotation(180);
			tail->setPosition(cocos2d::Point(3, 0));
			missile->body->addChild(tail);

			std::string weaponType = "";
			weaponType = "MISSILE";

			std::string bulletInfo = std::to_string(missile->speed) + " " + std::to_string(missile->startPos.x) + " " + std::to_string(missile->startPos.y) + " " + std::to_string(missile->targetPos.x) + " " + std::to_string(missile->targetPos.y) + " " + std::to_string(missile->vec.x) + " " + std::to_string(missile->vec.y) + " " +weaponType;

			CUser::getInst()->getClient()->emit("bulletInfo", bulletInfo);

		}
	}
}

void CGameScene::addWeapon(CWeapon * weapon)
{
	if (true == weapon->isEnermy)
		arrWeaponE.push_back(weapon);
	else
		arrWeaponA.push_back(weapon);
}

void CGameScene::addUnit(CUnit * unit)
{
	if (unit->isEnermy)
		arrUnitE[unit->UnitNumber] = unit;
		//arrUnitE.insert(std::pair<int, CUnit*>(unit->UnitNumber, unit));

}

void CGameScene::updateWeaponInterface(float dt)
{
	// 3가지 쿨타임에 진행중인 시간을 더해 준다.
	mVulcanCoolTime += dt;
	if (mVulcanCoolTime > DOKDO_VULCAN_COOLTIME)
		mVulcanCoolTime = DOKDO_VULCAN_COOLTIME;

	if (CUser::getInst()->getCannonCnt() > 0)
	{
		mCannonCoolTime += dt;
		if (mCannonCoolTime > DOKDO_CANNON_COOLTIME)
			mCannonCoolTime = DOKDO_CANNON_COOLTIME;
	}
	if (CUser::getInst()->getMissileCnt() > 0)
	{
		mMissileCoolTime += dt;
		if (mMissileCoolTime > DOKDO_MISSILE_COOLTIME)
			mMissileCoolTime = DOKDO_MISSILE_COOLTIME;
	}
	// interface SpriteBatchNode 획득
	cocos2d::SpriteBatchNode* sinterface = (cocos2d::SpriteBatchNode*)getChildByTag(TAG_SPRITE_BATCH_NODE_INTERFACE);

	// 각 무기별 Activate Sprite를 찾아서 현재 coolTime에 대한 비율로 scale을 변경해 준다.
	sinterface->getChildByTag(TAG_INTERFACE_VULCAN)
		->getChildByTag(TAG_INTERFACE_ACTIVATE)
		->setScale(mVulcanCoolTime / DOKDO_VULCAN_COOLTIME);

	sinterface->getChildByTag(TAG_INTERFACE_CANNON)
		->getChildByTag(TAG_INTERFACE_ACTIVATE)
		->setScale(mCannonCoolTime / DOKDO_CANNON_COOLTIME);

	sinterface->getChildByTag(TAG_INTERFACE_MISSILE)
		->getChildByTag(TAG_INTERFACE_ACTIVATE)
		->setScale(mMissileCoolTime / DOKDO_MISSILE_COOLTIME);

	shootFromDokdo();
}



void CGameScene::unitRemover(Node * sender)
{
	////CWeapon* weapon;

	////for (std::vector<CWeapon*>::iterator iter = arrWeaponA.begin(); iter != arrWeaponA.end(); iter++)
	////{
	////	weapon = (CWeapon*)*iter;
	////	weapon->release();
	////	delete weapon;
	////}
	////arrWeaponA.clear();

	////for (std::vector<CWeapon*>::iterator itr = arrWeaponE.begin(); itr != arrWeaponE.end(); itr++)
	////{
	////	weapon = (CWeapon*)*itr;
	////	weapon->release();
	////	delete weapon;
	////}
	////arrWeaponE.clear();

	////CUnit* unit;
	////for (std::vector<CUnit*>::iterator itr = arrUnitA.begin(); itr != arrUnitA.end(); itr++)
	////{
	////	unit = (CUnit*)*itr;
	////	unit->release();
	////	delete unit;
	////}
	////arrUnitA.clear();

	////for (std::vector<CUnit*>::iterator itr = arrUnitE.begin(); itr != arrUnitE.end(); itr++)
	////{
	////	unit = (CUnit*)*itr;
	////	unit->release();
	////	delete unit;
	////}
	////arrUnitE.clear();
}

void CGameScene::weaponRemover(Node * sender)
{
	CWeapon* weapon = NULL;
	//tag값 확인

	int tag = sender->getTag();
	if (TAG_WEAPON_A == tag)
	{
		//아군 무기 vector에서 찾아본다.
		for (std::vector<CWeapon*>::iterator itr = arrWeaponA.begin(); itr != arrWeaponA.end(); itr++)
		{
			weapon = (CWeapon*)*itr;
			if (weapon->body == sender)
			{
				arrWeaponA.erase(itr);
				break;
			}
			weapon = NULL;
		}
	}
	else
	{
		//적군 무기 vector에서 찾아본다.
		for (std::vector<CWeapon*>::iterator itr = arrWeaponE.begin(); itr != arrWeaponE.end(); itr++)
		{
			weapon = (CWeapon*)*itr;
			if (weapon->body == sender)
			{
				arrWeaponE.erase(itr);
				break;
			}
			weapon = NULL;
		}
	
		weapon->release();
		delete weapon;
	}
}

void CGameScene::selfRemover(Node * sender)
{
	sender->removeFromParentAndCleanup(true);

}

void CGameScene::enermyGunEffect()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("enermygun.wav");
}

void CGameScene::gunEffect1()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("gun01.wav");

}

void CGameScene::gunEffect2()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("gun02.wav");

}

void CGameScene::cannonEffect()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("cannon.wav");

}

void CGameScene::missileEffect()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("missile.wav");

}

void CGameScene::explosionEffect1()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explosion01.wav");

}

void CGameScene::explosionEffect2()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explosion02.wav");

}

void CGameScene::buttonEffect()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button.wav");

}

void CGameScene::onEnter()
{
	Scene::onEnter();
	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CGameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CGameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CGameScene::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(CGameScene::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CGameScene::onExit()
{
	_eventDispatcher->removeEventListener(listener);
	Scene::onExit();
}

bool CGameScene::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	mIsShoot = true;
	auto touchPoint = touch->getLocation();
	mPtShoot = touchPoint;
	CCLOG("onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touchPoint.x, touchPoint.y);
	return true;
}

void CGameScene::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto touchPoint = touch->getLocation();
	mPtShoot = touchPoint;

	CCLOG("onTouchMoved id = %d, x = %f, y = %f", touch->getID(), touchPoint.x, touchPoint.y);
}

void CGameScene::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	mIsShoot = false;

}

void CGameScene::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event)
{
	mIsShoot = false;

}

void CGameScene::checkCollision(float dt)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	CWeapon* weapon = NULL;
	CUnit* unit = NULL;
	for (std::vector<CWeapon*>::iterator iterWeapon = arrWeaponA.begin();
		iterWeapon != arrWeaponA.end();
		iterWeapon++)
	{
		weapon = (CWeapon*)*iterWeapon;


		bool bHit = false;
		for (std::unordered_map<int ,CUnit*>::iterator iterUnit = arrUnitE.begin();
			iterUnit != arrUnitE.end();
			iterUnit++)
		{
			unit = (CUnit*)iterUnit->second;


			cocos2d::Rect boundingBox = unit->body->getBoundingBox();
			if (boundingBox.containsPoint(weapon->body->getPosition()))
			{
				// 적중
				bHit = true;
				explosionEffect2();
				bool surface = unit->eUnitType <= HELICOPTER ? true : false;
				switch (weapon->eWeaponType)
				{
				case VULCAN:
					addExplosion(weapon->body->getPosition(), 0.1, 0.02, surface);
					break;
				case CANNON:
					addExplosion(weapon->body->getPosition(), 0.15, 0.03, surface);
					break;
				case MISSILE:
					addExplosion(weapon->body->getPosition(), 0.2, 0.04, surface);
					break;
				}
				std::string checkGun = std::to_string(weapon->eWeaponType) + " " + std::to_string(iterUnit->second->UnitNumber);
				CUser::getInst()->getClient()->emit("gun", checkGun);

				break;
			}
		}

		if (true == bHit)
		{
			weapon->release();
			delete weapon;
			iterWeapon = arrWeaponA.erase(iterWeapon);
			if (iterWeapon == arrWeaponA.end())
				break;
		}
	}
}

void CGameScene::checkEnemyCollision(float dt)
{
	cocos2d::Size winSize = cocos2d::Director::getInstance()->getWinSize();
	CWeapon* weapon = NULL;
	CUnit* unit = NULL;
	for (std::vector<CWeapon*>::iterator iterWeapon = arrWeaponE.begin();
		iterWeapon != arrWeaponE.end();
		iterWeapon++)
	{
		weapon = (CWeapon*)*iterWeapon;


		bool bHit = false;
		for (std::unordered_map<int, CUnit*>::iterator iterUnit = arrUnitE.begin();
			iterUnit != arrUnitE.end();
			iterUnit++)
		{
			unit = (CUnit*)iterUnit->second;


			cocos2d::Rect boundingBox = unit->body->getBoundingBox();
			if (boundingBox.containsPoint(weapon->body->getPosition()))
			{
				// 적중
				bHit = true;
				explosionEffect2();

				bool surface = unit->eUnitType <= HELICOPTER ? true : false;
				switch (weapon->eWeaponType)
				{
				case VULCAN:
					addExplosion(weapon->body->getPosition(), 0.1, 0.02, surface);
					break;
				case CANNON:
					addExplosion(weapon->body->getPosition(), 0.15, 0.03, surface);
					break;
				case MISSILE:
					addExplosion(weapon->body->getPosition(), 0.2, 0.04, surface);
					break;
				}
			}
		}

		if (true == bHit)
		{
			weapon->release();
			delete weapon;
			iterWeapon = arrWeaponE.erase(iterWeapon);
			if (iterWeapon == arrWeaponE.end())
				break;
		}
	}
}

void CGameScene::OverTimeUnitRemover(float dt)
{
	for (std::unordered_map<int, CUnit*>::iterator iterUnit = arrUnitE.begin();
		iterUnit != arrUnitE.end();
		iterUnit++)
	{
		CUnit* unit = (CUnit*)iterUnit->second;
		if (unit->body->getPosition().x >= 630)
		{
			unit->release();
			delete unit;
			iterUnit = arrUnitE.erase(iterUnit);
			if (iterUnit == arrUnitE.end())
				break;
		}
		
	}
}

void CGameScene::shoot(float dt)
{

		for (std::vector<CWeapon*>::iterator iterWeapon = arrWeaponA.begin();
			iterWeapon != arrWeaponA.end();
			iterWeapon++)
		{
			CWeapon* itr = (CWeapon*)*iterWeapon;
			
			itr->body->setPosition(itr->body->getPosition() + ((itr->vec*itr->speed*dt)));
			if (itr->body->getPosition().x >= 630 || itr->body->getPosition().y >= 630)
			{
				itr->release();
				delete itr;
				iterWeapon = arrWeaponA.erase(iterWeapon);
				if (iterWeapon == arrWeaponA.end())
					break;
			}
		}


		for (std::vector<CWeapon*>::iterator iterWeapon = arrWeaponE.begin();
			iterWeapon != arrWeaponE.end();
			iterWeapon++)
		{
			CWeapon* itr = (CWeapon*)*iterWeapon;

			itr->body->setPosition(itr->body->getPosition() + ((itr->vec*itr->speed*dt)));
			if (itr->body->getPosition().x >= 630 || itr->body->getPosition().y >= 630)
			{
				itr->release();
				delete itr;
				iterWeapon = arrWeaponE.erase(iterWeapon);
				if (iterWeapon == arrWeaponE.end())
					break;
			}
		}
	
}

void CGameScene::moveShip(float dt)
{
	for (std::unordered_map<int, CUnit*>::iterator iterUnit = arrUnitE.begin();
		iterUnit != arrUnitE.end();
		iterUnit++)
	{
		CUnit* unit = (CUnit*)iterUnit->second;

		float angle = (float)atanf((unit->startPos.x - unit->targetPos.x) / (unit->startPos.y- unit->targetPos.y)) * (180.f / M_PI);
		
		if (unit->targetPos.y > unit->startPos.y)
			unit->body->setRotation(angle-180);
		else
			unit->body->setRotation(angle);

		unit->body->setPosition(unit->body->getPosition() + ((unit->normalVec*unit->speed*dt)));
	
		if (unit->body->getPosition().x >= 630 || unit->body->getPosition().y >= 630)
		{
			unit->release();
			delete unit;
			iterUnit = arrUnitE.erase(iterUnit);
			if (iterUnit == arrUnitE.end())
				break;
		}
	}
}
