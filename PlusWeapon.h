#pragma once
#include "Game.h"
#include "SubWeapon.h"
#include "SBullet.h"
#include "Unit.h"
#include "Enemy.h"

class PlusWeapon : public SubWeapon
{
private:
	std::list<SBullet*> sbullet;
public:
	PlusWeapon();
	~PlusWeapon();
	void setSBullet(SBullet* sbullet);
	std::list<SBullet*>& getSBullet();
	void movePlusWeapon(Unit* chr);
	void launchPlusWeapon();
	void createBullet();
	void updateBullet(std::list<Enemy*>& enemies);
	void drawPlusWeapon();
};

