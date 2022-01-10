#pragma once
#include "Game.h"
#include "Unit.h"

class SubWeapon : public Unit
{
protected:
	Vector relativePos;
public:
	SubWeapon();
	~SubWeapon();
	void setRelativePos(Vector v);
	Vector& getRelativePos();
};

