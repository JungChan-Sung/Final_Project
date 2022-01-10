#pragma once
#include "Game.h"
#include "Unit.h"
#include "Item.h"
#include "HealBox.h"
#include "PowBox.h"
#include "WeaponBox.h"

class Enemy : public Unit
{
protected:
	int type;
	int waitTime;
	int waitDelay;
	int moveTime;
	bool itemFlag;
public:
	Enemy();
	~Enemy();
	void setType(int type);
	int getType();
	void setWaitTime(int time);
	int getWaitTime();
	void setWaitDelay(int delay);
	int getWaitDelay();
	void setMoveTime(int time);
	int getMoveTime();
	void setItemFlag(bool flag);
	bool getItemFlag();
	void createItem(std::list<Item*>& items);
};
