#pragma once
#include "Game.h"
#include "Unit.h"
#include "Enemy.h"
#include "Missile.h"
#include "PlusWeapon.h"
#include "Satellite.h"
#include "SBullet.h"
#include "ObjLoader.h"

class Character : public Unit
{
private:
	std::list<Missile*> missile;
	std::vector<PlusWeapon*> weapon;
	std::vector<Satellite*> satellite;
	int count;
	int life;
	int immortalStartTime;
	int immortalTime;
	bool immortality;
	bool respawnFlag;
	bool viewFlag;
	bool deathFlag;
public:
	Character();
	~Character();
	void setCount(int count);
	int getCount();
	void setLife(int life);
	int getLife();
	void setImmortalStartTime(int time);
	int getImmortalStartTime();
	void setImmortalTime(int time);
	int getImmortalTime();
	void setImmortality(bool flag);
	bool getImmortality();
	void setRespawnFlag(bool flag);
	bool getRespawnFlag();
	void setViewFlag(bool flag);
	bool getViewFlag();
	void setMissile(Missile* missile);
	std::list<Missile*>& getMissile();
	void setPlusWeapon(PlusWeapon* plus);
	std::vector<PlusWeapon*>& getPlusWeapon();
	void setSatellite(Satellite* satellite);
	std::vector<Satellite*>& getSatellite();
	void move();
	void axis();
	void launch();
	void createMissile();
	void updateMissile(std::list<Enemy*>& enemies);
	void checkRespawn();
	void respawnCharacter();
	void immortalCharacter();
	void viewToggle();
	void hitCharacter();
	void createPlusWeapon();
	void createSatellite();
	void volleySatellite();
	void drawCharacter();
};

extern Character* spaceCraft;