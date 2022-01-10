#pragma once
#include "Game.h"
#include "Item.h"
#include "Unit.h"
#include "ObjLoader.h"

class WeaponBox : public Item
{
public:
	WeaponBox();
	~WeaponBox();
	void effectWeapon(Unit* chr);
	void drawWeaponBox();
};

