#pragma once
#include "Game.h"

class Vector
{
private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
public:
	Vector();
	Vector(GLfloat x, GLfloat y, GLfloat z);
	~Vector();
	void setVector(GLfloat x, GLfloat y, GLfloat z);
	void setVector(const Vector& other);
	Vector& getVector();
	void setX(GLfloat x);
	GLfloat getX();
	void setY(GLfloat y);
	GLfloat getY();
	void setZ(GLfloat z);
	GLfloat getZ();
	Vector& operator=(const Vector& v);
	Vector& operator+(const Vector& v);
	Vector& operator-(const Vector& v);
	Vector& operator*(const GLfloat& scalar);
	GLfloat operator*(const Vector& v);
	Vector& operator+=(const Vector& v);
	Vector& operator-=(const Vector& v);
	Vector& operator*=(const GLfloat& scalar);
	GLfloat operator*=(const Vector& v);
	Vector& addVector(GLfloat x, GLfloat y, GLfloat z);
	Vector& addVector(const Vector& other);
	Vector& subVector(const Vector& other);
	Vector& mulVector(GLfloat scalar);
	GLfloat lengthVector();
	Vector& nomalVector();
	GLfloat dotVector(const Vector& other);
	GLfloat crossVector2D(const Vector& other);
	Vector crossVector3D(const Vector& other);
	void angleVector(GLfloat angle);
	void angleVector(GLfloat angle, const Vector& dir);
};