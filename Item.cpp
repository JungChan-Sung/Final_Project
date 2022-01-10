#include "Item.h"

Item::Item()
{
	this->speed = 0.035;
	this->maxSpeed = 0;
	this->type = 0;
	this->createTime = 0;
	this->durationTime = 15000;
	this->movement = rand() % 4; // 0, 1, 2, 3
	this->lrFlag = false; // left
	this->udFlag = false; // up
	if (this->movement == 1)
	{
		this->lrFlag = true; // right
		this->udFlag = false; // up
	}
	else if (this->movement == 2)
	{
		this->lrFlag = true; // right
		this->udFlag = true; // down
	}
	else if (this->movement == 3)
	{
		this->lrFlag = false; // left
		this->udFlag = true; // down
	}
	this->effectFlag = false;
	this->viewFlag = true;
	this->setDirection(0, 0, 1);
	this->setRange(0.15, 0.15);
	this->createHitBox();
}

Item::~Item()
{

}

void Item::setSpeed(GLfloat speed)
{
	this->speed = speed;
}

GLfloat Item::getSpeed()
{
	return this->speed;
}

void Item::setMaxSpeed(GLfloat maxSpeed)
{
	this->maxSpeed = maxSpeed;
}

GLfloat Item::getMaxSpeed()
{
	return this->maxSpeed;
}

void Item::setType(int type)
{
	this->type = type;
}

int Item::getType()
{
	return this->type;
}

void Item::setCreateTime(int time)
{
	this->createTime = time;
}

int Item::getCreateTime()
{
	return this->createTime;
}

void Item::setDurationTime(int time)
{
	this->durationTime = time;
}

int Item::getDurationTime()
{
	return this->durationTime;
}

void Item::setMoveMent(int move)
{
	this->movement = move;
}

int Item::getMoveMent()
{
	return this->movement;
}

void Item::setEffectFlag(bool flag)
{
	this->effectFlag = flag;
}

bool Item::getEffectFlag()
{
	return this->effectFlag;
}

void Item::setViewFlag(bool flag)
{
	this->viewFlag = flag;
}

bool Item::getViewFlag()
{
	return this->viewFlag;
}

void Item::moveItem()
{
	Vector dir;
	const float correction = 1; //º® º¸Á¤
	
	if (this->position.getX() >= MAX_X - correction)
		lrFlag = true; // ¿ÞÂÊº® ºÎµúÈû
	else if (this->position.getX() <= MIN_X + correction)
		lrFlag = false; // ¿À¸¥ÂÊº® ºÎµúÈû
	else if (this->position.getZ() >= MAX_Z - correction)
		udFlag = true; // À§ÂÊº® ºÎµúÈû
	else if (this->position.getZ() <= MIN_Z + correction)
		udFlag = false; // ¾Æ·¡º® ºÎµúÈû

	if (this->lrFlag == false)
		dir.setX(1);
	if (this->lrFlag == true)
		dir.setX(-1);
	if (this->udFlag == false)
		dir.setZ(1);
	if (this->udFlag == true)
		dir.setZ(-1);

	dir.nomalVector();
	this->position += dir * this->speed;
}

void Item::updateItem()
{
	const int viewTime = 3000;
	if ((current - this->createTime) >= this->durationTime)
	{
		this->effectFlag = true; // »ý¼ºÈÄ Áö¼Ó½Ã°£ Áö³ª¸é ¾ÆÀÌÅÛ ¼Ò¸ê
	}
	else if ((current - this->createTime) >= (this->durationTime - viewTime) && (current - this->createTime) < this->durationTime) // ¼Ò¸êÀü Á¡¸ê
	{
		viewToggle();
	}
}

void Item::viewToggle()
{
	const int toggleTime = 125;
	if ((current - this->createTime) % toggleTime <= (toggleTime * 0.5))
	{
		viewFlag = false;
	}
	else if ((current - this->createTime) % toggleTime > (toggleTime * 0.5))
	{
		viewFlag = true;
	}
}