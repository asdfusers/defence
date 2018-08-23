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
	//��ġ, ȭ���� ����� ����� ���̿� ���� ����, �ִϸ��̼� �ӵ�, ����/���� ����, ����
	void addShootFire(cocos2d::Point pt, float hRatio, float delayPerUnit, bool surface, float angle);

	void addWeaponInterface(const char* imgPath, const char* txtPath, cocos2d::Point pt, int tag);

	
	void unitRemover(Node* sender);
	void weaponRemover(Node* sender);
	void selfRemover(Node* sender);

	//����
	void enermyGunEffect();
	void gunEffect1();
	void gunEffect2();
	void cannonEffect();
	void missileEffect();
	void explosionEffect1();
	void explosionEffect2();
	void buttonEffect();


	//��ġ �̺�Ʈ
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

	//����, ���� Ÿ��, ���󿡼� �߻翩�� 
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
	std::vector<CWeapon*> arrWeaponA; // �Ʊ��� �߻��� ����
	std::vector<CWeapon*> arrWeaponE; // ������ �߻��� ����

	std::vector<CUnit*> arrUnitA; // �Ʊ� ����	
	std::unordered_map<int, CUnit*> arrUnitE; // ���� ����
	int position;



	std::string userName;
	std::string enemyName;
	std::string winStr = "WIN";
	std::string loseStr = "LOSE";
	int userScore;
	int enemyScore;
};

