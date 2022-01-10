#include "PlusWeapon.h"

PlusWeapon::PlusWeapon()
{
	this->fireDelay = 375;
	this->fireTime = 0;
	this->speed = 0;
}

PlusWeapon::~PlusWeapon()
{

}


void PlusWeapon::setSBullet(SBullet* sbullet)
{
	for (SBullet* s : this->sbullet)
	{
		if (s->getObjNumber() == sbullet->getObjNumber())
		{
			s = sbullet;
			break;
		}
	}
}

std::list<SBullet*>& PlusWeapon::getSBullet()
{
	return this->sbullet;
}

void PlusWeapon::movePlusWeapon(Unit* chr)
{
	GLfloat distance;
	Vector dir;
	Vector target = chr->getPosition();

	target += this->relativePos;
	dir = target - this->position;
	distance = dir.lengthVector();

	this->speed = chr->getCurrentSpeed();

	if (distance < 0.025)
		dir.setVector(0, 0, 0);

	dir.nomalVector();
	this->position += dir * this->speed;
}

void PlusWeapon::launchPlusWeapon()
{
	if (keyStates['z'])
	{
		if ((current - this->fireTime) >= this->fireDelay && this->destroyedFlag == false)
		{
			this->createBullet();
		}
	}
}

void PlusWeapon::createBullet()
{
	SBullet* sbullet = new SBullet();
	Vector dir;
	GLfloat angle = this->rotate.getYaw();

	this->fireTime = current;
	sbullet->setPosition(this->position);
	sbullet->setActive(true);
	this->direction.nomalVector();
	dir.angleVector(angle, this->direction);
	sbullet->getRotate().setYaw(angle);
	sbullet->setDirection(dir);
	this->sbullet.push_back(sbullet);
}

void PlusWeapon::updateBullet(std::list<Enemy*>& enemies)
{
	std::list<SBullet*> ::iterator s;
	std::list<Enemy*> ::iterator e;

	for (s = this->sbullet.begin(); s != this->sbullet.end();)
	{
		Vector front;

		(*s)->updateFragments();
		if ((*s)->getActive() == true)
		{
			front.setVector((*s)->getDirection());
			front.nomalVector();
			(*s)->getPosition() += front * (*s)->getSpeed();
		}

		if ((*s)->checkOutside()) // 맵 밖 나감
		{
			(*s)->setDestroyFlag(true);
		}

		for (e = enemies.begin(); e != enemies.end();) // 적 리스트 가져옴
		{
			if ((*s)->checkCollision(*e) && (*e)->getCollision() == true && (*s)->getCollision() == true) // 적 충돌 감지
			{
				(*s)->setCollision(false);
				(*e)->decHp((*s)->getDamage()); // 적 체력 감소
				(*e)->setHpBarView(true);
				(*e)->checkDestroyed();
				(*s)->setDestroyFlag(true);
				break;
			}
			else
				e++;
		}

		if ((*s)->getDestroyFlag() == true)
		{
			(*s)->createFragments();
			(*s)->setActive(false);
		}

		if ((*s)->getActive() == false && (*s)->getFragments().empty())
		{
			(*s)->setDeleteFlag(true);
		}

		if ((*s)->getDeleteFlag() == true)
		{
			(*s)->deleteObject();
			s = this->sbullet.erase(s);
		}
		else
		{
			s++;
		}
	}
}

void PlusWeapon::drawPlusWeapon()
{
	if (this->destroyedFlag == false && subWeaponView == 1)
	{
		glPushMatrix();
			glTranslatef(this->position.getX(), this->position.getY(), this->position.getZ());
			glPushMatrix();
				//모델링
				glRotatef(this->rotate.getYaw(), 0, 1, 0);
				glColor3f(0, 0, 0);
				glScalef(0.5, 0.5, 1.25);
				glutSolidCube(0.1);
			glPopMatrix();
		glPopMatrix();
	}

	//총알 그리기
	for (SBullet* m : this->sbullet)
	{
		m->drawSBullet();
	}
}