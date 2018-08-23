#ifndef __DokdoDefenceCommand__Weapon__
#define __DokdoDefenceCommand__Weapon__

#include "cocos2d.h"
#include "define.h"

enum WEAPON_TYPE {
	VULCAN = 0,     // ��ĭ
	CANNON,         // ����
	MISSILE,        // �̻���

};

enum TARGET_TYPE {
	SURFACE = 0,    // ���� Ÿ��
	AIR,            // ���� Ÿ��
	ALL             // ��� Ÿ��
};

class CWeapon
{
public:
	CWeapon();
	virtual ~CWeapon();

	void release();

	cocos2d::Sprite* body;          // ���� �̹���

	bool isEnermy;                  // �Ǿ�
	WEAPON_TYPE eWeaponType;        // ������ ����
	TARGET_TYPE eTargetType;        // Ÿ���� ����
	float damage;                   // ����
	float speed;
	cocos2d::Vec2 vec;
	cocos2d::Vec2 startPos;
	cocos2d::Vec2 targetPos;

	cocos2d::Vec2 E_vec;
	cocos2d::Vec2 E_startPos;
	cocos2d::Vec2 E_targetPos;


};

#endif /* defined(__DokdoDefenceCommand__Weapon__) */
