#pragma once
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Character.h"

class Common : public Enemy
{
private:
	int movement;
	std::list<Bullet*> bullet;
public:
	Common();
	~Common();
	void setMovement(int move);
	int getMovement();
	void setBullet(Bullet* bullet);
	std::list<Bullet*> getBullet();
	void createBullet();
	void updateBullet(Unit* chr);
	void moveCommon();
	void drawCommon();
};
