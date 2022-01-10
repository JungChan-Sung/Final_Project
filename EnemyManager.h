#pragma once
#include "Game.h"
#include "Enemy.h"
#include "Common.h"
#include "Turret.h"
#include "Boss.h"

class EnemyManager
{
private:
	std::list<Enemy*> enemies;
	int enemyCount;
	int commonSpawnTime;
	int commonLastSpawnTime;
	int turretSpawnTime;
	int turretLastSpawnTime;
	bool bossSpawn;
public:
	EnemyManager();
	~EnemyManager();
	void setEnemyCount(int count);
	int getEnemyCount();
	void setBossSpawn(bool spawn);
	bool getBossSpawn();
	void setEnermies(Enemy* enemy);
	std::list<Enemy*>& getEnermies();
	void createCommon();
	void createTurret();
	void createBoss();
};

extern EnemyManager* emg;