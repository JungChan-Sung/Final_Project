#pragma once
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Unit.h"
#include "Character.h"

class Turret : public Enemy
{
private:
	std::list<Bullet*> bullet;
	int fireDelay2;
	int fireTime2;
	bool reload;
	int magazine;
	int maxMagazine;
	int movement;
	GLfloat fireAngle;
public:
	Turret();
	~Turret();
	void setBullet(Bullet* bullet);
	std::list<Bullet*> getBullet();
	void setFireDelay2(int delay);
	int getFireDelay2();
	void setFireTime2(int time);
	int getFireTime2();
	void setReload(bool reload);
	bool getReload();
	void setMagazine(int magazine);
	int getMagazine();
	void setMaxMagazine(int maxMagazine);
	int getMaxMagazine();
	void setFireAngle(GLfloat angle);
	int getFireAngle();
	void createBullet();
	void updateBullet(Unit* chr);
	void updateTurret(Unit* chr);
	void moveTurrect();
	void drawTurretHead();
	void drawTurretBody();
};
