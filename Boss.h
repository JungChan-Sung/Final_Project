#pragma once
#include "Game.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Missile.h"
#include "Bullet.h"
#include "SBullet.h"

class Boss : public Enemy
{
private:
	int pattern;
	int reloadTime;
	int reloadDelay;
	int magazine;
	int maxMagazine;
	int bossState;
	bool lrFlag;
	bool udFlag;
	bool mimaFlag;
	bool reload;
	bool summonFlag;
	GLfloat minAngle;
	GLfloat maxAngle;
	GLfloat angle;
	GLfloat max_x;
	GLfloat min_x;
	GLfloat max_z;
	GLfloat min_z;
	Vector fireDir;
	std::list<Projectile*> projectile;
public:
	Boss();
	~Boss();
	void setPattern(int pattern);
	int getPattern();
	void setBossState(int state);
	void setProjectile(Projectile* projectile);
	std::list<Projectile*> getProjectile();
	void patternBoss(Unit* chr, std::list<Enemy*>& enemies);
	void moveBoss();
	void createProjectile(int type, int count, GLfloat a, Vector relativePos);
	void updateProjectile(Unit* chr);
	void createDeathFragments();
	void drawBoss();
	void draw();
};

