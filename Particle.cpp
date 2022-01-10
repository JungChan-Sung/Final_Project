#include "Particle.h"

Particle::Particle()
{
	this->speed = 0;
	this->rotSpeed = 0;
	this->acceleration = 0;
	this->size = 0;
	this->sizeSpeed = 0;
	this->createTime = 0;
	this->durationTime = 1500;
	this->deleteFlag = false;
}

Particle::~Particle()
{

}

void Particle::setPosition(const Vector& vector)
{
	this->position = vector;
}

void Particle::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
	this->position.setX(x);
	this->position.setY(y);
	this->position.setZ(z);
}

Vector& Particle::getPosition()
{
	return this->position;
}

void Particle::setDirection(const Vector& dir)
{
	this->direction = dir;
}

void Particle::setDirection(GLfloat x, GLfloat y, GLfloat z)
{
	this->direction.setX(x);
	this->direction.setY(y);
	this->direction.setZ(z);
}

Vector& Particle::getDirection()
{
	return this->direction;
}

void Particle::setRotate(const Axis& axis)
{
	this->rotate = axis;
}

Axis& Particle::getRotate()
{
	return this->rotate;
}

void Particle::setSpeed(GLfloat speed)
{
	this->speed = speed;
}

GLfloat Particle::getSpeed()
{
	return this->speed;
}

void Particle::setRotSpeed(GLfloat rotSpeed)
{
	this->rotSpeed = rotSpeed;
}

GLfloat Particle::getRotSpeed()
{
	return this->rotSpeed;
}

void Particle::setAcceleration(GLfloat acceleration)
{
	this->acceleration = acceleration;
}

GLfloat Particle::getAcceleration()
{
	return this->acceleration;
}

void Particle::setSize(GLfloat size)
{
	this->size = size;
}

GLfloat Particle::getSize()
{
	return this->size;
}

void Particle::setSizeSpeed(GLfloat speed)
{
	this->sizeSpeed = speed;
}

GLfloat Particle::getSizeSpeed()
{
	return this->sizeSpeed;
}

void Particle::setCreateTime(int time)
{
	this->createTime = time;
}

int Particle::getCreateTime()
{
	return this->createTime;
}

void Particle::setDurationTime(int time)
{
	this->durationTime = time;
}

int Particle::getDurationTime()
{
	return this->durationTime;
}

void Particle::setDeleteFlag(bool flag)
{
	this->deleteFlag = flag;
}

bool Particle::getDeleteFlag()
{
	return this->deleteFlag;
}

void Particle::drawParticle(GLfloat r, GLfloat g, GLfloat b)
{
	glPushMatrix();
		glTranslatef(this->position.getX(), this->position.getY(), this->position.getZ());
			glPushMatrix();
			glRotatef(this->rotate.getRoll(), 0, 0, 1);
			glRotatef(this->rotate.getYaw(), 0, 1, 0);
			glRotatef(this->rotate.getPitch(), 1, 0, 0);
			glColor3f(r, g, b);
			glutSolidCube(this->size);
		glPopMatrix();
	glPopMatrix();
}