#include "Boss.h"
#include "Character.h"
#include "Common.h"
#include "Turret.h"

Boss::Boss()
{
	this->pattern = 0;
	this->reloadTime = 0;
	this->reloadDelay = 500;
	this->magazine = 0;
	this->maxMagazine = 5;
	this->bossState = 0;
	this->lrFlag = false;
	this->udFlag = false;
	this->mimaFlag = false;
	this->reload = false;
	this->summonFlag = false;
	this->minAngle = 0;
	this->maxAngle = 0;
	this->angle = 0;
	this->speed = 0.01;
	this->maxSpeed = 0.02;
	this->acceleration = 0.00002;
	this->active = true;
	this->type = 3;
	this->hp = 1000;
	this->maxHp = 1000;
	this->fragmentsCount = 128;
	this->hpBarPos = 1;
	this->collision = false;
	this->setDirection(0, 0, -1);
	this->fireDir = this->direction;
	this->rotate.setYaw(180);
	this->setRange(1.2, 0.7);
	this->createHitBox();
}

Boss::~Boss()
{

}

void Boss::setPattern(int pattern)
{
	this->pattern = pattern;
}

int Boss::getPattern()
{
	return this->pattern;
}

void Boss::setBossState(int state)
{
	this->bossState = state;
}

void Boss::setProjectile(Projectile* projectile)
{
	for (Projectile* p : this->projectile)
	{
		if (p->getObjNumber() == projectile->getObjNumber())
		{
			p = projectile;
			break;
		}
	}
}

std::list<Projectile*> Boss::getProjectile()
{
	return this->projectile;
}

void Boss::patternBoss(Unit* chr, std::list<Enemy*>& enemies)
{
	Vector dir, pos;
	dir.setZ(-1);

	if (this->pattern == 0) // 등장
	{
		this->speed -= this->acceleration;
		this->position += dir * this->speed;
		if (this->speed <= 0)
		{
			this->pattern = 1;
			this->collision = true;
			this->waitTime = current;
			this->waitDelay = rand() % (2000 + 1 - 1000) + 1000; // 1 ~ 2초
			this->max_x = this->position.getX() + 1;
			this->min_x = this->position.getX() - 1;
			this->max_z = this->position.getZ() + 0.5;
			this->min_z = this->position.getZ() - 0.75;
		}
	}
	else if (this->pattern == 1) // 대기
	{
		if ((current - this->waitTime) >= this->waitDelay)
		{
			this->pattern = rand() % (6 + 1 - 2) + 2; // 2 ~ 6
			this->waitTime = current;
			this->lrFlag = rand() % 2;
			this->udFlag = rand() % 2;
			this->speed = 0.005;
			this->bossState = 0;

			if (this->pattern == 2)
			{
				this->mimaFlag = rand() % 2;
				this->waitDelay = rand() % (5000 + 1 - 4000) + 4000; // 4 ~ 5초 패턴 지속시간
				this->bossState = 1;
			}
			else if (this->pattern == 3)
			{
				this->mimaFlag = rand() % 2;
				this->waitDelay = rand() % (7000 + 1 - 5000) + 5000; // 5 ~ 7초 패턴 지속시간
				this->bossState = 1;
			}
			else if (this->pattern == 4)
			{
				this->mimaFlag = rand() % 2;
				this->waitDelay = rand() % (5000 + 1 - 3000) + 3000; // 3 ~ 5초 패턴 지속시간
				this->bossState = 2;
			}
			else if (this->pattern == 5)
			{
				this->waitTime = current;
				this->waitDelay = rand() % (6000 + 1 - 5000) + 5000; // 5 ~ 6초
			}
			else if (this->pattern == 6)
			{
				this->waitTime = current;
				this->waitDelay = rand() % (5000 + 1 - 4000) + 4000; // 4 ~ 5초
			}
		}
	}
	else if (this->pattern == 2) // 미사일 전방 발사
	{
		this->fireDelay = 300;
		this->minAngle = -30;
		this->maxAngle = 30;

		if ((current - this->fireTime) >= this->fireDelay)
		{
			this->fireTime = current;

			pos.setVector(0, 0, -0.5);
			this->createProjectile(1, 4, 10, pos);

			if (this->mimaFlag == false)
			{
				this->angle -= 5;
				if (this->angle <= this->minAngle)
					this->mimaFlag = true;
			}
			else if (this->mimaFlag == true)
			{
				this->angle += 5;
				if (this->angle >= this->maxAngle)
					this->mimaFlag = false;
			}

			this->fireDir.angleVector(this->angle, this->direction);
		}

		if ((current - this->waitTime) >= this->waitDelay)
		{
			this->pattern = 1; //대기 상태
			this->waitTime = current;
			this->waitDelay = rand() % (2000 + 1 - 1000) + 1000; // 1 ~ 2초
			this->fireDir = this->direction;
			this->angle = 0;
			this->bossState = 0;
		}
	}
	else if (this->pattern == 3) // 총알 전방향 발사
	{
		this->fireDelay = 300;

		if ((current - this->fireTime) >= this->fireDelay)
		{
			this->fireTime = current;

			pos.setVector(0, 0, 0);
			this->createProjectile(2, 12, 15, pos);

			if (this->mimaFlag == false)
			{
				this->angle -= 2.5;
			}
			else if (this->mimaFlag == true)
			{
				this->angle += 2.5;
			}

			this->fireDir.angleVector(this->angle, this->direction);
		}

		if ((current - this->waitTime) >= this->waitDelay)
		{
			this->pattern = 1; //대기 상태
			this->waitTime = current;
			this->waitDelay = rand() % (2000 + 1 - 1000) + 1000; // 1 ~ 2초
			this->fireDir = this->direction;
			this->angle = 0;
			this->bossState = 0;
		}
	}
	else if (this->pattern == 4) // 양쪽 레이저 발사
	{
		this->fireDelay = 125;

		if ((current - this->fireTime) >= this->fireDelay && reload == true)
		{
			GLfloat dot, cross, radian, degree;

			this->fireTime = current;

			//왼쪽
			this->direction.nomalVector();
			pos.setVector(1, 0, -0.5);
			dir = this->position;
			dir += pos;
			dir -= chr->getPosition();
			dir.nomalVector();
			
			dot = this->direction.dotVector(dir);
			cross = this->direction.crossVector2D(dir);
			radian = (cross > 0 ? 1 : -1) * acos(dot);
			degree = radian * (180.0 / PI);
			this->angle = -degree;
			this->fireDir.angleVector(degree + 90);
			this->createProjectile(3, 1, 30, pos);

			//오른쪽
			pos.setVector(-1, 0, -0.5);
			dir = this->position;
			dir += pos;
			dir -= chr->getPosition();
			dir.nomalVector();

			dot = this->direction.dotVector(dir);
			cross = this->direction.crossVector2D(dir);
			radian = (cross > 0 ? 1 : -1) * acos(dot);
			degree = radian * (180.0 / PI);
			this->angle = -degree;
			this->fireDir.angleVector(degree + 90);
			this->createProjectile(3, 1, 30, pos);

			if (this->magazine > 0)
			{
				this->magazine -= 1;
				if (this->magazine == 0)
				{
					this->magazine = 0;
					this->reloadTime = current;
					this->reload = false;
				}
			}
		}

		if ((current - this->reloadTime) >= this->reloadDelay && reload == false && this->magazine == 0)
		{
			this->magazine = this->maxMagazine;
			this->reload = true;
		}

		if ((current - this->waitTime) >= this->waitDelay)
		{
			this->pattern = 1; //대기 상태
			this->waitTime = current;
			this->waitDelay = rand() % (2000 + 1 - 1000) + 1000; // 1 ~ 2초
			this->fireDir = this->direction;
			this->angle = 0;
			this->magazine = 0;
			this->reload = false;
			this->bossState = 0;
		}
	}
	else if (this->pattern == 5)
	{
		if (this->summonFlag == false)
		{
			const GLfloat interval = 1.5;
			const int count = 4; // 짝수만
			Common* common[count];

			for (int i = 1; i <= count; i += 2)
			{
				common[i - 1] = new Common();
				common[i] = new Common();
				GLfloat x1, x2;

				x1 = interval * i;
				x2 = -interval * i;

				common[i - 1]->setPosition(x1, 0, 4.5);
				common[i - 1]->setSpeed(common[i - 1]->getMaxSpeed());
				enemies.push_back(common[i - 1]);

				common[i]->setPosition(x2, 0, 4.5);
				common[i]->setSpeed(common[i]->getMaxSpeed());
				enemies.push_back(common[i]);
			}

			this->summonFlag = true;
		}

		if ((current - this->waitTime) >= this->waitDelay)
		{
			this->pattern = 1; //대기 상태
			this->waitTime = current;
			this->waitDelay = rand() % (2000 + 1 - 1000) + 1000; // 1 ~ 2초
			this->summonFlag = false;
		}
	}
	else if (this->pattern == 6)
	{
		if (this->summonFlag == false)
		{
			const GLfloat interval = 3;
			const int count = 2; // 짝수만
			Turret* turret[count];

			for (int i = 1; i <= count; i += 2)
			{
				turret[i - 1] = new Turret();
				turret[i] = new Turret();
				GLfloat x1, x2;

				x1 = interval * i;
				x2 = -interval * i;

				turret[i - 1]->setPosition(x1, 0, 4.5);
				turret[i - 1]->setSpeed(turret[i - 1]->getMaxSpeed());
				enemies.push_back(turret[i - 1]);

				turret[i]->setPosition(x2, 0, 4.5);
				turret[i]->setSpeed(turret[i - 1]->getMaxSpeed());
				enemies.push_back(turret[i]);
			}

			this->summonFlag = true;
		}

		if ((current - this->waitTime) >= this->waitDelay)
		{
			this->pattern = 1; //대기 상태
			this->waitTime = current;
			this->waitDelay = rand() % (2000 + 1 - 1000) + 1000; // 1 ~ 2초
			this->summonFlag = false;
		}
	}
}

void Boss::moveBoss()
{
	if (this->pattern > 1)
	{
		Vector dir;

		if (this->position.getX() >= this->max_x)
			lrFlag = true; // 왼쪽벽 부딪힘
		else if (this->position.getX() <= this->min_x)
			lrFlag = false; // 오른쪽벽 부딪힘
		else if (this->position.getZ() >= this->max_z)
			udFlag = true; // 위쪽벽 부딪힘
		else if (this->position.getZ() <= this->min_z)
			udFlag = false; // 아래벽 부딪힘

		if (this->lrFlag == false)
		{
			dir.setX(1); // 왼쪽
		}
		else if (this->lrFlag == true)
		{
			dir.setX(-1); // 오른쪽
		}

		if (this->udFlag == false)
		{
			dir.setZ(1); // 위쪽
		}
		else if (this->udFlag == true)
		{
			dir.setZ(-1); // 아래쪽
		}

		dir.nomalVector();
		this->position += dir * this->speed;
	}
}

void Boss::createProjectile(int type, int count, GLfloat a, Vector relativePos)
{
	Vector dir;
	GLfloat angle;

	for (int i = 0; i <= count; i++)
	{
		Projectile* p = NULL;
		Projectile* p2 = NULL;

		if (type == 1) // 미사일
		{
			Missile* m = new Missile();
			p = m;
			if (i != 0)
			{
				Missile* m2 = new Missile();
				p2 = m2;
			}
		}
		else if (type == 2) // 총알
		{
			Bullet* b = new Bullet();
			p = b;
			if (i != 0)
			{
				Bullet* b2 = new Bullet();
				p2 = b2;
			}
		}
		else if (type == 3) // 보조총알
		{
			SBullet* s = new SBullet();
			p = s;
			if (i != 0)
			{
				SBullet* s2 = new SBullet();
				p2 = s2;
			}
		}

		angle = (float)i * a;

		p->setPosition(this->position);
		p->getPosition() += relativePos;
		p->setActive(true);

		dir.angleVector(angle, this->fireDir);
		p->getRotate().setYaw(angle + this->angle);
		p->setDirection(dir);
		this->projectile.push_back(p);

		if (i != 0)
		{
			p2->setPosition(this->position);
			p2->getPosition() += relativePos;
			p2->setActive(true);

			dir.angleVector(-angle, this->fireDir);
			p2->getRotate().setYaw(-angle + this->angle);
			p2->setDirection(dir);
			this->projectile.push_back(p2);
		}
	}
}

void Boss::updateProjectile(Unit* chr)
{
	std::list<Projectile*> ::iterator p;

	for (p = this->projectile.begin(); p != this->projectile.end();)
	{
		Vector front;

		(*p)->updateFragments();
		if ((*p)->getActive() == true)
		{
			front.setVector((*p)->getDirection());
			front.nomalVector().mulVector((*p)->getSpeed());
			(*p)->getPosition().addVector(front);
		}

		if ((*p)->checkOutside()) // 맵 밖 나감
		{
			(*p)->setDestroyFlag(true);
		}
		else if ((*p)->checkCollision(chr) && chr->getCollision() == true && (*p)->getCollision() == true) // 캐릭터 충돌
		{
			Character* cPtr = (Character*)chr;
			(*p)->setCollision(false);
			cPtr->decHp((*p)->getDamage());
			cPtr->hitCharacter();
			(*p)->setDestroyFlag(true);
			break;
		}

		if ((*p)->getDestroyFlag() == true)
		{
			(*p)->createFragments();
			(*p)->setActive(false);
		}

		if ((*p)->getActive() == false && (*p)->getFragments().empty())
		{
			(*p)->setDeleteFlag(true);
		}

		if ((*p)->getDeleteFlag() == true)
		{
			(*p)->deleteObject();
			p = this->projectile.erase(p);
		}
		else
		{
			p++;
		}
	}
}

void Boss::createDeathFragments()
{
	if (this->fragmentsFlag == false)
	{
		for (int i = 0; i < this->fragmentsCount; i++)
		{
			Particle* p = new Particle();
			Vector dir;
			Vector za;
			Axis axis;
			GLfloat x, z;
			GLfloat speed, rotSpeed, accel, size, sizeSpeed;

			//위치 방향 설정
			p->getPosition() = this->position;
			x = (rand() % 201 - 100) / 100.0;
			z = (rand() % 201 - 100) / 100.0;
			p->setDirection(x, 0, z);
			p->getDirection().nomalVector();

			//각도 설정
			za.setVector(0, 0, 1);
			GLfloat dot = za.dotVector(p->getDirection());
			Vector cross = za.crossVector3D(p->getDirection());
			GLfloat radian = (cross.getY() > 0 ? 1 : -1) * acos(dot);
			GLfloat degree = radian * (180.0 / PI);
			p->getRotate().setYaw(degree);

			//속도 크기 설정 rand() % (b + 1 - a) + a => a~b
			speed = (rand() % (300 + 1 - 250) + 250) / 10000.0;
			accel = (rand() % (30 + 1 - 15) + 15) / 100000.0;
			size = (rand() % (150 + 1 - 125) + 125) / 1000.0;
			sizeSpeed = (rand() % (50 + 1 - 30) + 30) / 100000.0;
			p->setSpeed(speed);
			p->setAcceleration(accel);
			p->setSize(size);
			p->setSizeSpeed(sizeSpeed);

			p->setCreateTime(current);
			p->setDurationTime(10000);

			this->fragments.push_back(p);
		}
		this->fragmentsFlag = true;
	}
}

void Boss::drawBoss()
{
	glPushMatrix();
		glTranslatef(this->position.getX(), this->position.getY(), this->position.getZ() - 0.25);
		glRotatef(this->rotate.getYaw(), 0, 1, 0);
		glPushMatrix();
			//모델링
			this->draw();
			//glColor3f(0.5, 0.5, 0.5);
			//glScalef(2, 0.5, 1);
			//glutSolidCube(1);
		glPopMatrix();
	glPopMatrix();

	if (this->collision == true)
	{
		//히트박스
		drawHitBox();
	}

	//체력바
	drawHpBar();

	//파편 그리기
	if (this->fragmentsFlag == true)
	{
		for (Particle* p : this->fragments)
		{
			glPushMatrix();
			glTranslatef(0, this->position.getY(), 0);
				p->drawParticle(1, 1, 1);
			glPopMatrix();
		}
	}

	// 총알 그리기
	for (Projectile* b : this->projectile)
	{
		if (b->getType() == 1)
		{
			Missile* mPtr = (Missile*)b;
			mPtr->drawMissile();
		}
		else if (b->getType() == 2)
		{
			Bullet* bPtr = (Bullet*)b;
			bPtr->drawBullet();
		}
		else if (b->getType() == 3)
		{
			SBullet* sPtr = (SBullet*)b;
			sPtr->drawSBullet();
		}
	}
}

void Boss::draw()
{
	glPushMatrix();
		glPushMatrix();
			glColor3f(1, 1, 1);
			glScalef(5, 1, 1);
			glutSolidCube(0.2);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0, 0, -0.4);
			glScalef(3, 1, 1);
			glutSolidCube(0.2);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.6, 0, 0.1);
			glScalef(1, 1, 2);
			glutSolidCube(0.2);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.6, 0, 0.1);
			glScalef(1, 1, 2);
			glutSolidCube(0.2);
		glPopMatrix();
		//평상시
		if (bossState == 0)
		{
			glPushMatrix();
				glTranslatef(0, 0, -0.2);
				glScalef(11, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(1, 0, 0.1);
				glScalef(1, 1, 2);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-1, 0, 0.1);
				glScalef(1, 1, 2);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-0.7, 0, -0.4);
				glScalef(2, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.7, 0, -0.4);
				glScalef(2, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-0.3, 0, 0.4);
				glScalef(2, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.3, 0, 0.4);
				glScalef(2, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glColor3f(0, 0, 0);
				glTranslatef(-0.4, 0, -0.4);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.4, 0, -0.4);
				glutSolidCube(0.2);
			glPopMatrix();
		}
		//공격
		else if (bossState == 1)
		{
			glPushMatrix();
				glTranslatef(0, 0, -0.2);
				glScalef(9, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(1, 0, -0.7);
				glScalef(1, 1, 2);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-1, 0, -0.7);
				glScalef(1, 1, 2);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-0.8, 0, -0.4);
				glScalef(3, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.8, 0, -0.4);
				glScalef(3, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-0.4, 0, 0.4);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.4, 0, 0.4);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glColor3f(1, 0, 0);
				glTranslatef(-0.4, 0, -0.4);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.4, 0, -0.4);
				glutSolidCube(0.2);
			glPopMatrix();
		}
		//공격2
		if (bossState == 2)
		{
			glPushMatrix();
				glTranslatef(0, 0, -0.2);
				glScalef(11, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(1, 0, 0.1);
				glScalef(1, 1, 2);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-1, 0, 0.1);
				glScalef(1, 1, 2);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-0.7, 0, -0.4);
				glScalef(2, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.7, 0, -0.4);
				glScalef(2, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-0.4, 0, 0.4);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.4, 0, 0.4);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glColor3f(1, 0, 0);
				glTranslatef(-0.4, 0, -0.4);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.4, 0, -0.4);
				glutSolidCube(0.2);
			glPopMatrix();
		}
		//죽음
		else if (bossState == 3)
		{
			glPushMatrix();
				glTranslatef(0, 0, -0.2);
				glScalef(11, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(1, 0, 0.1);
				glScalef(1, 1, 2);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-1, 0, 0.1);
				glScalef(1, 1, 2);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-0.6, 0, -0.4);
				glScalef(3, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.6, 0, -0.4);
				glScalef(3, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-0.3, 0, 0.4);
				glScalef(2, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.3, 0, 0.4);
				glScalef(2, 1, 1);
				glutSolidCube(0.2);
			glPopMatrix();
			glPushMatrix();
				glColor3f(0, 0, 0);
				glTranslatef(-0.3, 0.05, -0.4);
				glPushMatrix();
					glRotatef(45, 0, 1, 0);
					glScalef(1.5, 1, 0.5);
					glutSolidCube(0.2);
				glPopMatrix();
				glPushMatrix();
					glRotatef(-45, 0, 1, 0);
					glScalef(1.5, 1, 0.5);
					glutSolidCube(0.2);
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.3, 0.05, -0.4);
				glPushMatrix();
					glRotatef(45, 0, 1, 0);
					glScalef(1.5, 1, 0.5);
					glutSolidCube(0.2);
				glPopMatrix();
				glPushMatrix();
					glRotatef(-45, 0, 1, 0);
					glScalef(1.5, 1, 0.5);
					glutSolidCube(0.2);
				glPopMatrix();
			glPopMatrix();
		}
		glColor3f(1, 1, 1);
		glPushMatrix();
			glTranslatef(0, 0, -0.6);
			glScalef(7, 1, 1);
			glutSolidCube(0.2);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.4, 0, -0.8);
			glutSolidCube(0.2);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.4, 0, -0.8);
			glutSolidCube(0.2);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.6, 0, -1);
			glutSolidCube(0.2);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.6, 0, -1);
			glutSolidCube(0.2);
		glPopMatrix();
	glPopMatrix();
}