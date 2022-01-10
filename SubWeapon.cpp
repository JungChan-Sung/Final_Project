#include "SubWeapon.h"

SubWeapon::SubWeapon()
{
	this->hp = 0;
	this->active = true;
	this->collision = false;
	this->setDirection(0, 0, 1);
}

SubWeapon::~SubWeapon()
{

}


void SubWeapon::setRelativePos(Vector v)
{
	this->relativePos = v;
}

Vector& SubWeapon::getRelativePos()
{
	return this->relativePos;
}