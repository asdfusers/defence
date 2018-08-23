
#ifndef __DokdoDefenceCommand__Unit__
#define __DokdoDefenceCommand__Unit__

#include "cocos2d.h"
#include "define.h"

enum UNIT_TYPE {
	SUBMARINE = 0,
	SHIP,
	HELICOPTER,
	AIRCRAFT
};

class CUnit
{
public:
	CUnit();
	virtual ~CUnit();

	void setEnergy(float max);
	float subEnergy(float damage);
	void release();

	cocos2d::Sprite* body;

	UNIT_TYPE eUnitType;
	float maxEnergy;
	float curEnergy;
	int UnitNumber;
	bool isEnermy;

	float speed;

	cocos2d::Vec2 startPos;
	cocos2d::Vec2 targetPos;
	cocos2d::Vec2 normalVec;
};

#endif 