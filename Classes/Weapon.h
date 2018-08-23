#ifndef __DokdoDefenceCommand__Weapon__
#define __DokdoDefenceCommand__Weapon__

#include "cocos2d.h"
#include "define.h"

enum WEAPON_TYPE {
	VULCAN = 0,     // 발칸
	CANNON,         // 대포
	MISSILE,        // 미사일

};

enum TARGET_TYPE {
	SURFACE = 0,    // 지상 타겟
	AIR,            // 공중 타겟
	ALL             // 모든 타겟
};

class CWeapon
{
public:
	CWeapon();
	virtual ~CWeapon();

	void release();

	cocos2d::Sprite* body;          // 무기 이미지

	bool isEnermy;                  // 피아
	WEAPON_TYPE eWeaponType;        // 무기의 종류
	TARGET_TYPE eTargetType;        // 타겟의 종류
	float damage;                   // 위력
	float speed;
	cocos2d::Vec2 vec;
	cocos2d::Vec2 startPos;
	cocos2d::Vec2 targetPos;

	cocos2d::Vec2 E_vec;
	cocos2d::Vec2 E_startPos;
	cocos2d::Vec2 E_targetPos;


};

#endif /* defined(__DokdoDefenceCommand__Weapon__) */
