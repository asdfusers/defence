#include "Weapon.h"

CWeapon::CWeapon() : body(NULL), isEnermy(false), damage(0), eWeaponType(VULCAN), eTargetType(ALL)
{
}

CWeapon::~CWeapon()
{
	release();
}

void CWeapon::release()
{
	if (NULL != body)
	{
		body->removeFromParentAndCleanup(true);
		body = NULL;
	}
}