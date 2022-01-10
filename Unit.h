#pragma once
#include "Game.h"
#include "Object.h"
#include "Particle.h"

class Unit : public Object
{
protected:
	GLfloat speed;
	GLfloat rotSpeed;
	GLfloat maxSpeed;
	GLfloat maxRot;
	GLfloat acceleration;
	GLfloat currentSpeed;
	GLfloat hpBarPos;
	int hp;
	int maxHp;
	int fireDelay;
	int fireTime;
	int destroyedTime;
	bool destroyedFlag;
	bool hpBarView;
public:
	Unit();
	~Unit();
	void setSpeed(GLfloat speed);
	GLfloat getSpeed();
	void setRotSpeed(GLfloat rotSpeed);
	GLfloat getRotSpeed();
	void setMaxSpeed(GLfloat maxSpeed);
	GLfloat getMaxSpeed();
	void setMaxRot(GLfloat maxRot);
	GLfloat getMaxRot();
	void setAcceleration(GLfloat acceleration);
	GLfloat getAcceleration();
	void setCurrentSpeed(GLfloat speed);
	GLfloat getCurrentSpeed();
	void setHpBarPos(GLfloat pos);
	GLfloat getHpBarPos();
	void setHp(int hp);
	int getHp();
	void setMaxHp(int maxHp);
	int getMaxHp();
	void setFireDelay(int delay);
	int getFireDelay();
	void setFireTime(int time);
	int getFireTime();
	void setDestroyedTime(int time);
	int getDestroyedTime();
	void setDestroyedFlag(bool flag);
	bool getDestroyedFlag();
	void setHpBarView(bool flag);
	bool getHpBarView();
	void incHp(int damage);
	void decHp(int damage);
	bool checkDestroyed();
	void destructionUnit();
	void createDeathFragments();
	void drawHpBar();
};

