#pragma once
#include "Game.h"

class Axis
{
private:
	GLfloat pitch;	//xรเ
	GLfloat yaw;	//yรเ
	GLfloat roll;	//zรเ
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
