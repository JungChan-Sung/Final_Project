#pragma once
#include "Game.h"
#include "SubWeapon.h"
#include "SBullet.h"
#include "Unit.h"
#include "Enemy.h"

class Satellite : public SubWeapon
{
private:
	std::list<SBullet*> sbullet;
	GLfloat accelDis;
	bool fireFlag;
public:
	Satellite();
	~Satellite();
	void setAccelDis(GLfloat distance);
	GLfloat getAccelDis();
	void setFireFlag(bool flag);
	bool getFireFlag();
	void setSBullet(SBullet* sbullet);
	std::list<SBullet*>& getSBullet();
	void moveSatellite(Unit* chr);
	void axisSatellite();
	void launchSatellite();
	void createBullet();
	void updateBullet(std::list<Enemy*>& enemies);
	void drawSatellite();
};