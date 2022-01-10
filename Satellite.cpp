#include "Satellite.h"

Satellite::Satellite()
{
	this->accelDis = 0;
	this->fireFlag = false;
	this->fireDelay = 500;
	this->fireTime = 0;
	this->speed = 0;
	this->maxSpeed = 0.1;
	this->rotSpeed = 1;
	this->maxRot = 30;
	this->acceleration = 0.002;
}

Satellite::~Satellite()
{

}

void Satellite::setAccelDis(GLfloat distance)
{
	this->accelDis = distance;
}

GLfloat Satellite::getAccelDis()
{
	return this->accelDis;
}

void Satellite::setFireFlag(bool flag)
{
	this->fireFlag = flag;
}

bool Satellite::getFireFlag()
{
	return this->fireFlag;
}

void Satellite::setSBullet(SBullet* sbullet)
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

std::list<SBullet*>& Satellite::getSBullet()
{
	return this->sbullet;
}

void Satellite::moveSatellite(Unit* chr)
{
	GLfloat distance;
	Vector dir;
	Vector target = chr->getPosition();

	target += this->relativePos;
	dir = target - this->position;
	distance = dir.lengthVector();

	this->maxSpeed = chr->getCurrentSpeed();
	this->accelDis = pow(this->speed, 2) / (2 * this->acceleration);

	if (distance > this->accelDis)
		this->speed += this->acceleration;
	else if (distance <= this->accelDis)
		this->speed -= this->acceleration;

	if (this->speed > this->maxSpeed)
		this->speed = this->maxSpeed;
	else if (this->speed < 0)
		this->speed = 0;

	dir.nomalVector();
	this->position += dir * this->speed;
}

void Satellite::axisSatellite()
{
	GLfloat roll = this->rotate.getRoll();
	if (keyStates[GLUT_KEY_LEFT])
	{
		roll -= this->rotSpeed;
		if (roll <= -this->maxRot)
			roll = -this->maxRot;
	}

	if (keyStates[GLUT_KEY_RIGHT])
	{
		roll += this->rotSpeed;
		if (roll >= this->maxRot)
			roll = this->maxRot;
	}

	if (!keyStates[GLUT_KEY_LEFT])
	{
		if (roll <= 0)
			roll += this->rotSpeed;
	}

	if (!keyStates[GLUT_KEY_RIGHT])
	{
		if (roll >= 0)
			roll -= this->rotSpeed;
	}

	if (keyStates[GLUT_KEY_LEFT] && keyStates[GLUT_KEY_RIGHT])
	{
		if (roll < 0)
			roll += this->rotSpeed;
		else if (roll > 0)
			roll -= this->rotSpeed;
	}

	this->rotate.setRoll(roll);
}

void Satellite::launchSatellite()
{
	if ((current - this->fireTime) >= this->fireDelay && this->fireFlag == true && this->destroyedFlag == false)
	{
		this->createBullet();
	}
}

void Satellite::createBullet()
{
	SBullet* sbullet = new SBullet();
	Vector dir;
	const int fireAngle = 3;
	const int correction = 10;
	GLfloat angle = ((rand() % (fireAngle * correction) + 1) - (fireAngle * correction * 0.5)) / (correction * 0.5);

	this->fireTime = current;
	sbullet->setPosition(this->position);
	sbullet->setActive(true);
	this->direction.nomalVector();
	dir.angleVector(angle, this->direction);
	sbullet->getRotate().setYaw(angle);
	sbullet->setDirection(dir);
	this->sbullet.push_back(sbullet);
}

void Satellite::updateBullet(std::list<Enemy*>& enemies)
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

void Satellite::drawSatellite()
{
	if (this->destroyedFlag == false)
	{
		glPushMatrix();
			glTranslatef(this->position.getX(), this->position.getY(), this->position.getZ());
			glPushMatrix();
				glRotatef(this->rotate.getRoll(), 0, 0, 1);
				//모델링
				glColor3f(1, 1, 1);
				glScalef(0.25, 0.25, 0.25);
				satellite.draw(1);
				//glColor3f(0, 0, 1);
				//glutSolidCube(0.1);
			glPopMatrix();
		glPopMatrix();
	}

	//총알 그리기
	for (SBullet* m : this->sbullet)
	{
		m->drawSBullet();
	}
}