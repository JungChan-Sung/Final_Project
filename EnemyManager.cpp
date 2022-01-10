#pragma once
#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	this->enemyCount = 0;
	this->commonSpawnTime = 2000;
	this->commonLastSpawnTime = 0;
	this->turretSpawnTime = 2500;
	this->turretLastSpawnTime = 0;
	this->bossSpawn = false;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::setEnemyCount(int count)
{
	this->enemyCount = count;
}

int EnemyManager::getEnemyCount()
{
	return this->enemyCount;
}

void EnemyManager::setBossSpawn(bool spawn)
{
	this->bossSpawn = spawn;
}

bool EnemyManager::getBossSpawn()
{
	return this->bossSpawn;
}

void EnemyManager::setEnermies(Enemy* enemy)
{
	for (Enemy* e : enemies)
	{
		if (e->getObjNumber() == enemy->getObjNumber())
		{
			e = enemy;
			break;
		}
	}
}

std::list<Enemy*>& EnemyManager::getEnermies()
{
	return this->enemies;
}

void EnemyManager::createCommon()
{
	const int percent = 100;
	int random = rand() % percent + 1; // 100ºĞÀÇ 1 È®·ü
	if ((current - this->commonLastSpawnTime) >= this->commonSpawnTime && random == percent * 0.5)
	{
		Common* common = new Common();
		Vector spawnPosition;
		GLfloat x, z;
		GLfloat speed;
		const GLfloat correction = 1000;
		const GLfloat correction2 = 600.0;
		const GLfloat base = 1000.0;

		this->commonLastSpawnTime = current;
		x = (fmod(rand(), (MAX_X * correction) + 1) - (MAX_X * correction * 0.5)) / correction2;
		z = MAX_Z;
		spawnPosition.setVector(x, 0, z);
		common->setPosition(spawnPosition);
		common->setFireTime(current);
		speed = (fmod(rand(), (common->getMaxSpeed() * base) + 1 - (common->getSpeed() * base)) + (common->getSpeed() * base)) / base;
		common->setSpeed(speed);

		this->enemyCount += 1;
		this->enemies.push_back(common);
	}
}

void EnemyManager::createTurret()
{
	const int percent = 250;
	int random = rand() % percent + 1; // 250ºĞÀÇ 1 È®·ü
	if ((current - this->turretLastSpawnTime) >= this->turretSpawnTime && random == percent * 0.5)
	{
		Turret* turret = new Turret();
		Vector spawnPosition;
		GLfloat x, z;
		GLfloat speed;
		const GLfloat correction = 1000.0;
		const GLfloat correction2 = 600.0;
		const GLfloat base = 10000.0;

		this->turretLastSpawnTime = current;
		x = (fmod(rand(), (MAX_X * correction) + 1) - (MAX_X * correction * 0.5)) / correction2;
		z = MAX_Z;
		spawnPosition.setVector(x, 0, z);
		turret->setPosition(spawnPosition);
		turret->setFireTime(current);
		speed = (fmod(rand(), (turret->getMaxSpeed() * base) + 1 - (turret->getSpeed() * base)) + (turret->getSpeed() * base)) / base;
		turret->setSpeed(speed);

		this->enemyCount += 1;
		this->enemies.push_back(turret);
	}
}

void EnemyManager::createBoss()
{
	if (this->enemyCount >= 30 && this->bossSpawn == false)
	{
		Boss* boss = new Boss();
		Vector spawnPosition;

		spawnPosition.setVector(0, 0, 5);
		boss->setPosition(spawnPosition);

		this->bossSpawn = true;
		this->enemies.push_back(boss);
	}
}