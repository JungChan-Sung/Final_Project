#include "Projectile.h"

Projectile::Projectile()
{
	this->type = 0;
	this->damage = 0;
	this->speed = 0;
	this->destroyFlag = false;
	this->deleteFlag = false;
}

Projectile::~Projectile()
{

}

void Projectile::setType(int type)
{
	this->type = type;
}

int Projectile::getType()
{
	return this->type;
}

void Projectile::setDamage(int damage)
{
	this->damage = damage;
}

int Projectile::getDamage()
{
	return this->damage;
}

void Projectile::setSpeed(GLfloat speed)
{
	this->speed = speed;
}

GLfloat Projectile::getSpeed()
{
	return this->speed;
}

void Projectile::setDestroyFlag(bool flag)
{
	this->destroyFlag = flag;
}

bool Projectile::getDestroyFlag()
{
	return this->destroyFlag;
}

void Projectile::setDeleteFlag(bool flag)
{
	this->deleteFlag = flag;
}

bool Projectile::getDeleteFlag()
{
	return this->deleteFlag;
}

void Projectile::createFragments()
{
	if (this->fragmentsFlag == false)
	{
		for (int i = 0; i < this->fragmentsCount; i++)
		{
			Particle* p = new Particle();
			Vector dir;
			GLfloat x, y, z;
			GLfloat speed, rotSpeed, accel, size, sizeSpeed;

			//위치 방향 설정
			p->getPosition() = this->position;
			x = (rand() % 201 - 100) / 100.0;
			y = (rand() % 201 - 100) / 100.0;
			z = (rand() % 201 - 100) / 100.0;
			p->setDirection(x, y, z);
			p->getDirection().nomalVector();

			//속도 크기 설정 rand() % (b + 1 - a) + a => a~b
			speed = (rand() % (200 + 1 - 150) + 150) / 10000.0;
			accel = (rand() % (30 + 1 - 20) + 20) / 100000.0;
			size = (rand() % (50 + 1 - 25) + 25) / 1000.0;
			sizeSpeed = (rand() % (30 + 1 - 20) + 20) / 100000.0;
			p->setSpeed(speed);
			p->setAcceleration(accel);
			p->setSize(size);
			p->setSizeSpeed(sizeSpeed);

			p->setCreateTime(current);

			this->fragments.push_back(p);
		}

		this->fragmentsFlag = true;
	}
}