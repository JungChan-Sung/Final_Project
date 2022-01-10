#pragma once
#include "Game.h"
#include "Object.h"

class Projectile : public Object
{
protected:
	int type;
	int damage;
	GLfloat speed;
	bool destroyFlag;
	bool deleteFlag;
public:
	Projectile();
	~Projectile();
	void setType(int type);
	int getType();
	void setDamage(int damage);
	int getDamage();
	void setSpeed(GLfloat speed);
	GLfloat getSpeed();
	void setDestroyFlag(bool flag);
	bool getDestroyFlag();
	void setDeleteFlag(bool flag);
	bool getDeleteFlag();
	void createFragments();
};

