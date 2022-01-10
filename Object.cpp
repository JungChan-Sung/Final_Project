#include "Object.h"

int Object::objCounter = 0;

Object::Object()
{
	this->objCounter += 1;
	this->objNumber = objCounter;
	this->fragmentsCount = 4;
	this->active = false;
	this->collision = true;
	this->fragmentsFlag = false;
	this->xRange = 0;
	this->zRange = 0;
}

Object::~Object()
{

}

int Object::getObjNumber()
{
	return this->objNumber;
}

void Object::setFragmentsCount(int count)
{
	this->fragmentsCount = count;
}

int Object::getFragmentsCount()
{
	return this->fragmentsCount;
}

void Object::setDirection(const Vector& dir)
{
	this->direction = dir;
}

void Object::setDirection(GLfloat x, GLfloat y, GLfloat z)
{
	this->direction.setX(x);
	this->direction.setY(y);
	this->direction.setZ(z);
}

Vector& Object::getDirection()
{
	return this->direction;
}

void Object::setPosition(const Vector& vector)
{
	this->position = vector;
}

void Object::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
	this->position.setX(x);
	this->position.setY(y);
	this->position.setZ(z);
}

Vector& Object::getPosition()
{
	return this->position;
}

void Object::setRotate(const Axis& axis)
{
	this->rotate = axis;
}

Axis& Object::getRotate()
{
	return this->rotate;
}

void Object::setActive(bool active)
{
	this->active = active;
}

bool Object::getActive()
{
	return this->active;
}

void Object::setCollision(bool collision)
{
	this->collision = collision;
}

bool Object::getCollision()
{
	return this->collision;
}

void Object::setFragmentsFlag(bool flag)
{
	this->fragmentsFlag = flag;
}

bool Object::getFragmentsFlag()
{
	return this->fragmentsFlag;
}

void Object::setRange(GLfloat xRange, GLfloat zRange)
{
	this->xRange = xRange;
	this->zRange = zRange;
}

std::vector<std::pair<GLfloat, GLfloat>> Object::getHitBox()
{
	return this->hitBox;
}

std::list<Particle*> Object::getFragments()
{
	return this->fragments;
}

void Object::createHitBox()
{
	GLfloat x, z;
	for (int i = 0; i < 5; i++)
	{
		x = xRange + this->position.getX();
		z = zRange + this->position.getZ();

		if (i == 1)
			x = -xRange + this->position.getX();
		else if (i == 2)
		{
			x = -xRange + this->position.getX();
			z = -zRange + this->position.getZ();
		}
		else if (i == 3)
			z = -zRange + this->position.getZ();

		this->hitBox.push_back(std::pair<GLfloat, GLfloat>(x, z));
	}
}

void Object::updateHitBox()
{
	GLfloat x, z;
	for (int i = 0; i < 5; i++)
	{
		x = xRange + this->position.getX();
		z = zRange + this->position.getZ();

		if (i == 1)
			x = -xRange + this->position.getX();
		else if (i == 2)
		{
			x = -xRange + this->position.getX();
			z = -zRange + this->position.getZ();
		}
		else if (i == 3)
			z = -zRange + this->position.getZ();

		this->hitBox[i].first = x;
		this->hitBox[i].second = z;
	}
}

bool Object::checkCollision(Object* dst)
{
	//obj1 좌표
	GLfloat x1[2] = { this->hitBox[0].first, this->hitBox[2].first }; //obj1의 x , -x / maxX , minX
	GLfloat z1[2] = { this->hitBox[0].second, this->hitBox[2].second }; //obj1의 z , -z / maxZ , minZ

	//obj2 좌표
	GLfloat x2[2] = { dst->hitBox[0].first, dst->hitBox[2].first }; //obj2의 x , -x / maxX , minX
	GLfloat z2[2] = { dst->hitBox[0].second, dst->hitBox[2].second }; //obj2의 z , -z / maxZ , minZ

	bool collisionX = ((x1[1] <= x2[0]) && (x1[0] >= x2[1])); //this->minX < dst.maxX && this->maxX > dst.minX
	bool collisionZ = ((z1[1] <= z2[0]) && (z1[0] >= z2[1])); //this->minZ < dst.maxZ && this->maxZ > dst.minZ

	return collisionX && collisionZ;
}

bool Object::checkOutside()
{
	if (this->position.getZ() >= MAX_Z || this->position.getZ() <= MIN_Z || this->position.getX() >= MAX_X || this->position.getX() <= MIN_X)
		return true;
	else
		return false;
}

void Object::deleteObject()
{
	delete this;
}

void Object::updateFragments()
{
	std::list<Particle*>::iterator p;

	for (p = this->fragments.begin(); p != this->fragments.end();)
	{
		Vector dir;
		GLfloat speed, size;

		dir.setVector((*p)->getDirection());
		speed = (*p)->getSpeed() - (*p)->getAcceleration();
		(*p)->setSpeed(speed);
		(*p)->getPosition() += dir * (*p)->getSpeed();
		size = (*p)->getSize() - (*p)->getSizeSpeed();
		(*p)->setSize(size);

		if ((*p)->getSpeed() <= 0)
			(*p)->setSpeed(0);

		if ((*p)->getSize() <= 0)
			(*p)->setSize(0);

		if ((*p)->getSpeed() <= 0 || (*p)->getSize() <= 0 || (current - (*p)->getCreateTime()) >= (*p)->getDurationTime())
		{
			delete (*p);
			p = this->fragments.erase(p);
		}
		else
			p++;
	}
}

void Object::drawHitBox()
{
	this->updateHitBox();

	glPushMatrix();
		//히트박스
		if (hitBoxView == 1)
		{
			glColor3f(1.0, 0, 0);
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < this->hitBox.size(); i++)
				glVertex3f(this->hitBox[i].first, 0, this->hitBox[i].second);
			glEnd();
		}
	glPopMatrix();
}