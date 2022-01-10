#pragma once
#include "Game.h"
#include "Vector.h"
#include "Axis.h"

class Particle
{
private:
	Vector position;
	Vector direction;
	Axis rotate;
	GLfloat speed;
	GLfloat rotSpeed;
	GLfloat acceleration;
	GLfloat size;
	GLfloat sizeSpeed;
	int createTime;
	int durationTime;
	bool deleteFlag;
public:
	Particle();
	~Particle();
	void setPosition(const Vector& vector);
	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	Vector& getPosition();
	void setDirection(const Vector& dir);
	void setDirection(GLfloat x, GLfloat y, GLfloat z);
	Vector& getDirection();
	void setRotate(const Axis& axis);
	Axis& getRotate();
	void setSpeed(GLfloat speed);
	GLfloat getSpeed();
	void setRotSpeed(GLfloat rotSpeed);
	GLfloat getRotSpeed();
	void setAcceleration(GLfloat acceleration);
	GLfloat getAcceleration();
	void setSize(GLfloat size);
	GLfloat getSize();
	void setSizeSpeed(GLfloat speed);
	GLfloat getSizeSpeed();
	void setCreateTime(int time);
	int getCreateTime();
	void setDurationTime(int time);
	int getDurationTime();
	void setDeleteFlag(bool flag);
	bool getDeleteFlag();
	void drawParticle(GLfloat r, GLfloat g, GLfloat b);
};

