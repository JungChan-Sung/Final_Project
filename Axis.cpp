#include "Axis.h"

Axis::Axis()
{
	this->pitch = 0;
	this->yaw = 0;
	this->roll = 0;
}

Axis::Axis(GLfloat pitch, GLfloat yaw, GLfloat roll)
{
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
}

Axis::~Axis()
{
	
}

void Axis::setAxis(GLfloat pitch, GLfloat yaw, GLfloat roll)
{
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
}

Axis& Axis::getAxis()
{
	return *this;
}

void Axis::setPitch(GLfloat pitch)
{
	this->pitch = pitch;
}

GLfloat Axis::getPitch()
{
	return this->pitch;
}

void Axis::setYaw(GLfloat yaw)
{
	this->yaw = yaw;
}

GLfloat Axis::getYaw()
{
	return this->yaw;
}

void Axis::setRoll(GLfloat roll)
{
	this->roll = roll;
}

GLfloat Axis::getRoll()
{
	return this->roll;
}