#pragma once
#include "Game.h"
#include "Projectile.h"

class Missile : public Projectile
{
public:
	Missile();
	~Missile();
	void drawMissile();
};

