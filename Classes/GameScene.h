#pragma once
#include "cocos2d.h"
#include "ui\UIEditBox\UIEditBox.h"
#include "ui\CocosGUI.h"
#include <network\SocketIO.h>
#include "Weapon.h"
#include "Unit.h"
using namespace cocos2d::network;
using namespace cocos2d::ui;

class CGameScene : public cocos2d::Scene, public SocketIO::SIODelegate
{
public:
	virtual bool init();
	static cocos2d::Scene* scene();
	CREATE_FUNC(CGameScene);

	virtual void onConnect(SIOClient* client);
	virtual void onMessage(SIOClient* client, const std::string& data);
	virtual void onClose(SIOClient* client);
	virtual void onError(SIOClient* client, const std::string& data);

	void textFieldEvent();
	void shootFromDokdo();
	void setFirstPosition(cocos2d::Sprite* sprite, int x, int y);
	
	void onCreateShip(SIOClient* client, const std::string& data);
	void onMoveShip(SIOClient* client, const std::string& data);
	void onUnitKill(SIOClient* client, const std::string& data);
	void onPlayerPosition(SIOClient* client, const std::string& data);
	void E_shootFromDokdo(SIOClient* client, const std::string& data);
	void onCreateCloud(SIOClient* client, const std::string& data);
	void onGameEnd(SIOClient* client, const std::string& data);
	void onEnemyResult(SIOClient* client, const std::string& data);
	void onMyResult(SIOClient* client, const std::string& data);
	void showResult(SIOClient* client, const std::string& data);
	void playerInfo(SIOClient* client, const std::string& data);

	void hideScoreDlg();
	void showScoreDlg();

	void addExplosion(cocos2d::Point pt, float wRatio, float delayPerUnit, bool surface);

	void addWeapon(CWeapon* weapon);
	void addUnit(CUnit* unit);
	void updateWeaponInterface(float dt);
	//위치, 화염의 사이즈를 계산할 높이에 따른 비율, 애니메이션 속도, 지상/공중 여부, 각도
	void addShootFire(cocos2d::Point pt, float hRatio, float delayPerUnit, bool surface, float angle);

	void addWeaponInterface(const char* imgPath, const char* txtPath, cocos2d::Point pt, int tag);

	
	void unitRemover(Node* sender);
	void weaponRemover(Node* sender);
	void selfRemover(Node* sender);

	//사운드
	void enermyGunEffect();
	void gunEffect1();
	void gunEffect2();
	void cannonEffect();
	void missileEffect();
	void explosionEffect1();
	void explosionEffect2();
	void buttonEffect();


	//터치 이벤트
	cocos2d::EventListenerTouchOneByOne* listener;
	void onEnter();
	void onExit();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

	void checkCollision(float dt);
	void checkEnemyCollision(float dt);
	void OverTimeUnitRemover(float dt);

	void shoot(float dt);
	void moveShip(float dt);

	//위력, 무기 타입, 지상에서 발사여부 
	void fireBullet(float damage, WEAPON_TYPE type, bool surface);
	void fireMissile(float damage, bool enemy);



private:
	cocos2d::ui::EditBox* insertChat;
	std::string _chat;
	int index;
	//SIOClient* _client;
	TextField* editBox;
	std::vector<char*> info;
	cocos2d::Sprite *spriteSEA;



	float mVulcanCoolTime, mCannonCoolTime, mMissileCoolTime;
	bool mIsShoot;
	cocos2d::Point mPtShoot;
	cocos2d::Point mPtDokdo;
	cocos2d::Point E_mPtDokdo;
	cocos2d::Point E_mPtShoot;
	std::vector<CWeapon*> arrWeaponA; // 아군이 발사한 무기
	std::vector<CWeapon*> arrWeaponE; // 적군이 발사한 무기

	std::vector<CUnit*> arrUnitA; // 아군 유닛	
	std::unordered_map<int, CUnit*> arrUnitE; // 적군 유닛
	int position;



	std::string userName;
	std::string enemyName;
	std::string winStr = "WIN";
	std::string loseStr = "LOSE";
	int userScore;
	int enemyScore;
};

