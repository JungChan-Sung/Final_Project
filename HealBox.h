#pragma once
#include "Game.h"
#include "Item.h"
#include "Unit.h"
#include "ObjLoader.h"

class HealBox : public Item
{
public:
	HealBox();
	~HealBox();
	void effectHeal(Unit* chr);
	void drawHealBox();
	void draw(CModel m);
};