#include "Vector.h"

Vector::Vector()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector::Vector(GLfloat x, GLfloat y, GLfloat z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::~Vector()
{

}

void Vector::setVector(GLfloat x, GLfloat y, GLfloat z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector::setVector(const Vector& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Vector& Vector::getVector()
{
	return *this;
}

void Vector::setX(GLfloat x)
{
	this->x = x;
}

GLfloat Vector::getX()
{
	return this->x;
}

void Vector::setY(GLfloat y)
{
	this->y = y;
}

GLfloat Vector::getY()
{
	return this->y;
}

void Vector::setZ(GLfloat z)
{
	this->z = z;
}

GLfloat Vector::getZ()
{
	return this->z;
}

Vector& Vector::operator=(const Vector& v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	return *this;
}

Vector& Vector::operator+(const Vector& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector& Vector::operator-(const Vector& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vector& Vector::operator*(const GLfloat& scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	return *this;
}

GLfloat Vector::operator*(const Vector& v) // 벡터 내적
{
	return (this->x * v.x + this->y * v.y + this->z * v.z);
}

Vector& Vector::operator+=(const Vector& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector& Vector::operator-=(const Vector& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vector& Vector::operator*=(const GLfloat& scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	return *this;
}

GLfloat Vector::operator*=(const Vector& v) // 벡터 내적
{
	return (this->x * v.x + this->y * v.y + this->z * v.z);
}

Vector& Vector::addVector(GLfloat x, GLfloat y, GLfloat z)
{
	this->x += x;
	this->y += y;
	this->z += z;
	return *this;
}

Vector& Vector::addVector(const Vector& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vector& Vector::subVector(const Vector& other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

Vector& Vector::mulVector(GLfloat scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	return *this;
}

GLfloat Vector::lengthVector()
{
	return (GLfloat)sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

Vector& Vector::nomalVector()
{
	GLfloat length = lengthVector();
	if (length != 0)
	{
		this->x /= length;
		this->y /= length;
		this->z /= length;
	}
	return *this;
}

GLfloat Vector::dotVector(const Vector& other)
{
	return (this->x * other.x + this->y * other.y + this->z * other.z);
}

GLfloat Vector::crossVector2D(const Vector& other) // 2차원 외적 x, z축
{
	return (this->x * other.z - this->z * other.x);
}

Vector Vector::crossVector3D(const Vector& other) // 2차원 외적 x, z축
{
	return Vector(this->y * other.x - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x);
}

void Vector::angleVector(GLfloat angle) // 좌상단 기준 각도
{
	double theta = angle * (PI / 180.0);
	this->x = cos(theta);
	this->z = sin(theta);
}

void Vector::angleVector(GLfloat angle, const Vector& dir) // 방향벡터기준 각도
{
	double theta = angle * (PI / 180.0);
	this->x = dir.x * cos(theta) + dir.z * sin(theta);
	this->z = -dir.x * sin(theta) + dir.z * cos(theta);
}