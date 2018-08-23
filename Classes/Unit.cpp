#include "Unit.h"



CUnit::CUnit() : body(NULL), maxEnergy(100), curEnergy(0), eUnitType(SHIP), isEnermy(true)
{
}


CUnit::~CUnit()
{
	release();
}

void CUnit::setEnergy(float max)
{
	maxEnergy = curEnergy = max;
}

float CUnit::subEnergy(float damage)
{
	curEnergy -= damage;
	if (curEnergy < 0)
		curEnergy = 0;
	return curEnergy;
}

void CUnit::release()
{
	if (NULL != body)
	{
		body->removeFromParentAndCleanup(true);
		body = NULL;
	}
}
