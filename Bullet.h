#pragma once
#include "Game.h"
#include "Projectile.h"

class Bullet : public Projectile
{
public:
	Bullet();
	~Bullet();
	void drawBullet();
};
