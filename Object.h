#pragma once
#include "Game.h"
#include "Vector.h"
#include "Axis.h"
#include "Particle.h"

class Object
{
private:
	static int objCounter;
protected:
	int objNumber;
	int fragmentsCount;
	bool active;
	bool collision;
	bool fragmentsFlag;
	Vector position;
	Vector direction;
	Axis rotate;
	GLfloat xRange;
	GLfloat zRange;
	std::vector<std::pair<GLfloat, GLfloat>> hitBox;
	std::list<Particle*> fragments;
public:
	Object();
	~Object();
	int getObjNumber();
	void setFragmentsCount(int count);
	int getFragmentsCount();
	void setDirection(const Vector& dir);
	void setDirection(GLfloat x, GLfloat y, GLfloat z);
	Vector& getDirection();
	void setPosition(const Vector& vector);
	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	Vector& getPosition();
	void setRotate(const Axis& axis);
	Axis& getRotate();
	void setActive(bool active);
	bool getActive();
	void setCollision(bool collision);
	bool getCollision();
	void setFragmentsFlag(bool flag);
	bool getFragmentsFlag();
	void setRange(GLfloat xRange, GLfloat zRange);
	std::vector <std::pair<GLfloat, GLfloat>> getHitBox();
	std::list<Particle*> getFragments();
	void createHitBox();
	void updateHitBox();
	bool checkCollision(Object* dst);
	bool checkOutside();
	void deleteObject();
	void updateFragments();
	void drawHitBox();
};
