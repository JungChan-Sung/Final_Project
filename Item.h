#pragma once
#include "Game.h"
#include "Object.h"

class Item : public Object
{
protected:
	GLfloat speed;
	GLfloat maxSpeed;
	int type;
	int createTime;
	int durationTime;
	int movement;
	bool lrFlag;
	bool udFlag;
	bool effectFlag;
	bool viewFlag;
public:
	Item();
	~Item();
	void setSpeed(GLfloat speed);
	GLfloat getSpeed();
	void setMaxSpeed(GLfloat maxSpeed);
	GLfloat getMaxSpeed();
	void setType(int type);
	int getType();
	void setCreateTime(int time);
	int getCreateTime();
	void setDurationTime(int time);
	int getDurationTime();
	void setMoveMent(int move);
	int getMoveMent();
	void setEffectFlag(bool flag);
	bool getEffectFlag();
	void setViewFlag(bool flag);
	bool getViewFlag();
	void moveItem();
	void updateItem();
	void viewToggle();
};

extern std::list<Item*> items;