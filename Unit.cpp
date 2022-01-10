#include "Unit.h"

Unit::Unit()
{
	this->speed = 0;
	this->rotSpeed = 0;
	this->maxSpeed = 0;
	this->maxRot = 0;
	this->acceleration = 0;
	this->currentSpeed = 0;
	this->hpBarPos = 0.375;
	this->hp = 1;
	this->maxHp = 1;
	this->fireDelay = 0;
	this->fireTime = 0;
	this->destroyedTime = 0;
	this->fragmentsCount = 16;
	this->destroyedFlag = false;
	this->hpBarView = false;
}

Unit::~Unit()
{

}

void Unit::setSpeed(GLfloat speed)
{
	this->speed = speed;
}

GLfloat Unit::getSpeed()
{
	return this->speed;
}

void Unit::setRotSpeed(GLfloat rotSpeed)
{
	this->rotSpeed = rotSpeed;
}

GLfloat Unit::getRotSpeed()
{
	return this->rotSpeed;
}

void Unit::setMaxSpeed(GLfloat maxSpeed)
{
	this->maxSpeed = maxSpeed;
}

GLfloat Unit::getMaxSpeed()
{
	return this->maxSpeed;
}

void Unit::setMaxRot(GLfloat maxRot)
{
	this->maxRot = maxRot;
}

GLfloat Unit::getMaxRot()
{
	return this->maxRot;
}

void Unit::setAcceleration(GLfloat acceleration)
{
	this->acceleration = acceleration;
}

GLfloat Unit::getAcceleration()
{
	return this->acceleration;
}

void Unit::setCurrentSpeed(GLfloat speed)
{
	this->currentSpeed = speed;
}

GLfloat Unit::getCurrentSpeed()
{
	return this->currentSpeed;
}

void Unit::setHpBarPos(GLfloat pos)
{
	this->hpBarPos = pos;
}

GLfloat Unit::getHpBarPos()
{
	return this->hpBarPos;
}

void Unit::setHp(int hp)
{
	this->hp = hp;
}

int Unit::getHp()
{
	return this->hp;
}

void Unit::setMaxHp(int maxHp)
{
	this->maxHp = maxHp;
}

int Unit::getMaxHp()
{
	return this->maxHp;
}


void Unit::setFireDelay(int delay)
{
	this->fireDelay = delay;
}

int Unit::getFireDelay()
{
	return this->fireDelay;
}

void Unit::setFireTime(int time)
{
	this->fireTime = time;
}

int Unit::getFireTime()
{
	return this->fireTime;
}

void Unit::setDestroyedTime(int time)
{
	this->destroyedTime = time;
}

int Unit::getDestroyedTime()
{
	return this->destroyedTime;
}

void Unit::setDestroyedFlag(bool flag)
{
	this->destroyedFlag = flag;
}

bool Unit::getDestroyedFlag()
{
	return this->destroyedFlag;
}

void Unit::setHpBarView(bool flag)
{
	this->hpBarView = flag;
}

bool Unit::getHpBarView()
{
	return this->hpBarView;
}

void Unit::incHp(int damage)
{
	this->hp += damage;
}

void Unit::decHp(int damage)
{
	this->hp -= damage;
}

bool Unit::checkDestroyed()
{
	if (this->destroyedFlag == false && this->hp <= 0)
	{
		this->destroyedFlag = true;
		this->destroyedTime = current;
		return true;
	}
	else
	{
		return false;
	}
}

void Unit::destructionUnit()
{
	if (this->destroyedFlag == true)
	{
		deleteObject();
	}
}

void Unit::createDeathFragments()
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
			speed = (rand() % (150 + 1 - 100) + 100) / 10000.0;
			accel = (rand() % (30 + 1 - 15) + 15) / 100000.0;
			size = (rand() % (75 + 1 - 50) + 50) / 1000.0;
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

void Unit::drawHpBar()
{
	const GLfloat base = 0.5;
	GLfloat hpbar = base * ((float)this->hp / (float)this->maxHp);
	if (hpbar <= 0)
		hpbar = 0;

	if (this->hpBarView == true || healthBarView == 1)
	{
		glPushMatrix();
			glTranslatef(this->position.getX(), 0, this->position.getZ() - this->hpBarPos);
			glPushMatrix();
				glTranslatef(base / 2.0, 0, 0);
				glRotatef(180, 0, 1, 0);
				glBegin(GL_QUADS);
					glColor3f(1, 0, 0);
					glVertex3f(0, 0, 0);
					glVertex3f(hpbar, 0, 0); // 체력바 좌표
					glVertex3f(hpbar, 0, 0.05); // 체력바 좌표
					glVertex3f(0, 0, 0.05);
				glEnd();
			glPopMatrix();
		glPopMatrix();
	}
}