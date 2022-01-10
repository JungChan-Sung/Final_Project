#include "Turret.h"

Turret::Turret()
{
	this->fireDelay = 2000;
	this->speed = 0.01;
	this->maxSpeed = 0.013;
	this->acceleration = 0.00004;
	this->active = true;
	this->fireDelay2 = 125;
	this->fireTime2 = 0;
	this->reload = false;
	this->magazine = 0;
	this->maxMagazine = 3;
	this->rotSpeed = 0.2;
	this->fireAngle = 0;
	this->type = 2;
	this->hp = 8;
	this->maxHp = 8;
	this->movement = 0;
	this->hpBarPos = 0.6;
	this->getRotate().setYaw(180);
	this->setDirection(0, 0, -1);
	this->setRange(0.15, 0.15);
	this->createHitBox();
}

Turret::~Turret()
{

}

void Turret::setBullet(Bullet* bullet)
{
	for (Bullet* b : this->bullet)
	{
		if (b->getObjNumber() == bullet->getObjNumber())
		{
			b = bullet;
			break;
		}
	}
}

std::list<Bullet*> Turret::getBullet()
{
	return this->bullet;
}

void Turret::setFireDelay2(int delay)
{
	this->fireDelay2 = delay;
}

int Turret::getFireDelay2()
{
	return this->fireDelay2;
}

void Turret::setFireTime2(int time)
{
	this->fireTime2 = time;
}

int Turret::getFireTime2()
{
	return this->fireTime2;
}

void Turret::setReload(bool reload)
{
	this->reload = reload;
}

bool Turret::getReload()
{
	return this->reload;
}

void Turret::setMagazine(int magazine)
{
	this->magazine = magazine;
}

int Turret::getMagazine()
{
	return this->magazine;
}

void Turret::setMaxMagazine(int maxMagazine)
{
	this->maxMagazine = maxMagazine;
}

int Turret::getMaxMagazine()
{
	return this->maxMagazine;
}

void Turret::setFireAngle(GLfloat angle)
{
	this->fireAngle = angle;
}

int Turret::getFireAngle()
{
	return this->fireAngle;
}

void Turret::createBullet()
{
	if ((current - this->fireTime2) >= this->fireDelay2 && reload == true && movement != 0)
	{
		Bullet* bullet;
		Vector dir;
		int count = this->magazine;

		this->fireTime2 = current;

		bullet = new Bullet();
		bullet->setPosition(this->position);
		bullet->setActive(true);

		dir.angleVector(this->rotate.getYaw() - 180, this->direction);

		bullet->setDirection(dir);
		bullet->getPosition() += bullet->getDirection() * 0.5;
		this->bullet.push_back(bullet);

		if (this->magazine > 0)
		{
			count--;
			this->magazine = count;

			if (this->magazine == 0)
			{
				this->magazine = 0;
				this->fireTime = current;
				this->reload = false;
			}
		}
	}

	//장전
	if ((current - this->fireTime) >= this->fireDelay && reload == false && (this->magazine == 0) && movement != 0)
	{
		this->magazine = this->maxMagazine;
		this->reload = true;
	}
}

void Turret::updateBullet(Unit* chr)
{
	std::list<Bullet*> ::iterator b;

	for (b = this->bullet.begin(); b != this->bullet.end();)
	{
		Vector front;

		(*b)->updateFragments();
		if ((*b)->getActive() == true)
		{
			front.setVector((*b)->getDirection());
			front.nomalVector().mulVector((*b)->getSpeed());
			(*b)->getPosition().addVector(front);
		}

		if ((*b)->checkOutside()) // 맵 밖 나감
		{
			(*b)->setDestroyFlag(true);
		}
		else if ((*b)->checkCollision(chr) && chr->getCollision() == true && (*b)->getCollision() == true) // 캐릭터 충돌
		{
			Character* cPtr = (Character*)chr;
			(*b)->setCollision(false);
			cPtr->decHp((*b)->getDamage());
			cPtr->hitCharacter();
			(*b)->setDestroyFlag(true);
			break;
		}

		if ((*b)->getDestroyFlag() == true)
		{
			(*b)->createFragments();
			(*b)->setActive(false);
		}

		if ((*b)->getActive() == false && (*b)->getFragments().empty())
		{
			(*b)->setDeleteFlag(true);
		}

		if ((*b)->getDeleteFlag() == true)
		{
			(*b)->deleteObject();
			b = this->bullet.erase(b);
		}
		else
		{
			b++;
		}
	}
}

void Turret::updateTurret(Unit* chr)
{
	if (this->getReload() == false)
	{
		Vector dir = this->position;
		dir -= chr->getPosition();
		this->direction.nomalVector();
		dir.nomalVector();

		GLfloat dot = this->direction.dotVector(dir);
		GLfloat cross = this->direction.crossVector2D(dir);
		GLfloat radian = (cross < 0 ? 1 : -1) * acos(dot);
		GLfloat degree = radian * (180.0 / PI);

		GLfloat yaw = this->rotate.getYaw();

		if (degree != yaw)
		{
			GLfloat change = 0;
			GLfloat diff = fmodf(yaw - degree, 360);
			if (diff < 0)
				change = this->rotSpeed;
			else
				change = -this->rotSpeed;

			if (abs(diff) > 180)
				change = 0 - change;

			yaw += change;
		}

		this->rotate.setYaw(yaw);
	}
}

void Turret::moveTurrect()
{
	Vector dir;

	dir.setZ(-1);

	if (this->movement == 0)
	{
		this->speed -= this->acceleration;
		this->position += dir * this->speed;
		if (this->speed <= 0)
		{
			this->movement = 1;
			this->waitTime = current;
			this->waitDelay = rand() % (8000 + 1 - 6000) + 6000; // 6초 ~ 8초
		}
	}
	if (this->movement == 1 && (current - this->waitTime) >= this->waitDelay)
	{
		this->speed += this->acceleration;
		if (this->speed > this->maxSpeed)
			this->speed = this->maxSpeed;
		this->position += dir * this->speed;
	}
}

void Turret::drawTurretHead()
{
	if (this->destroyedFlag == false)
	{

		glPushMatrix();
			glTranslatef(this->position.getX(), 0, this->position.getZ());
			//모델링
			glPushMatrix();
				glRotatef(this->rotate.getYaw(), 0, 1, 0);
				glPushMatrix();
					glPushMatrix(); // 레이저 표시기
						glColor3f(1, 0.0, 0.0);
						glTranslatef(0, 0, 0.5);
						glScalef(0.1, 0.1, 10);
						glutSolidCube(0.1);
					glPopMatrix();
					glPushMatrix(); //머리
						glColor3f(1, 1, 1);
						glScalef(0.5, 0.5, 0.5);
						turretHead.draw(11);
						//glColor3f(0.0, 0.0, 0.0);
						//glutSolidCube(0.2);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();

		//히트박스
		drawHitBox();

		//체력바
		drawHpBar();
	}

	//파편 그리기
	if (this->fragmentsFlag == true)
	{
		for (Particle* p : this->fragments)
		{
			p->drawParticle(0.45, 0.45, 0.45);
		}
	}

	for (Bullet* b : this->bullet)
	{
		b->drawBullet();
	}
}

void Turret::drawTurretBody()
{
	if (this->destroyedFlag == false)
	{
		glPushMatrix(); //몸통
			glTranslatef(this->position.getX(), 0, this->position.getZ());
			glColor3f(1, 1, 1);
			turretBody.draw(12);
		glPopMatrix();
	}
}