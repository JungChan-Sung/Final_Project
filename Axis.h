#pragma once
#include "Game.h"

class Axis
{
private:
	GLfloat pitch;	//x��
	GLfloat yaw;	//y��
	GLfloat roll;	//z��
public:
	Axis();
	Axis(GLfloat pitch, GLfloat yaw, GLfloat roll);
	~Axis();
	void setAxis(GLfloat pitch, GLfloat yaw, GLfloat roll);
	Axis& getAxis();
	void setPitch(GLfloat pitch);
	GLfloat getPitch();
	void setYaw(GLfloat yaw);
	GLfloat getYaw();
	void setRoll(GLfloat roll);
	GLfloat getRoll();
};
