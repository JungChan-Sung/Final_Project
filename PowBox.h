#pragma once
#include "Game.h"
#include "Item.h"
#include "Unit.h"
#include "ObjLoader.h"

class PowBox : public Item
{
public:
	PowBox();
	~PowBox();
	void effectPow(Unit* chr);
	void drawPowBox();
};

