#pragma once
#include "Enemy.h"

Enemy::Enemy()
{
	this->type = 0;
	this->waitTime = 0;
	this->waitDelay = 0;
	this->moveTime = 0;
	this->itemFlag = false;
}

Enemy::~Enemy()
{

}

void Enemy::setType(int type)
{
	this->type = type;
}

int Enemy::getType()
{
	return this->type;
}

void Enemy::setWaitTime(int time)
{
	this->waitTime = time;
}

int Enemy::getWaitTime()
{
	return this->waitTime;
}

void Enemy::setWaitDelay(int delay)
{
	this->waitDelay = delay;
}

int Enemy::getWaitDelay()
{
	return this->waitDelay;
}

void Enemy::setMoveTime(int time)
{
	this->moveTime = time;
}

int Enemy::getMoveTime()
{
	return this->moveTime;
}

void Enemy::setItemFlag(bool flag)
{
	this->itemFlag = flag;
}

bool Enemy::getItemFlag()
{
	return this->itemFlag;
}

void Enemy::createItem(std::list<Item*>& items)
{
	if (this->itemFlag == false)
	{
		const int percent = 5;
		int random = rand() % percent + 1; // 1 ~ 5

		this->itemFlag = true;

		if (random == 1)
		{
			HealBox* h = new HealBox();

			h->getPosition().setVector(this->position);
			h->setCreateTime(current);

			items.push_back(h);
		}
		else if (random == 2)
		{
			PowBox* p = new PowBox();

			p->getPosition().setVector(this->position);
			p->setCreateTime(current);

			items.push_back(p);
		}
		else if (random == 3)
		{
			WeaponBox* w = new WeaponBox();

			w->getPosition().setVector(this->position);
			w->setCreateTime(current);

			items.push_back(w);
		}
	}
}