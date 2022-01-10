#include "Missile.h"

Missile::Missile()
{
	this->type = 1;
	this->damage = 2;
	this->speed = 0.1;
	this->setPosition(10, 0, 0); // 멀리서 생성
	this->setDirection(0, 0, 1);
	this->setRange(0.03, 0.05);
	this->createHitBox();
}

Missile::~Missile()
{

}

void Missile::drawMissile()
{
	if (this->active == true)
	{
		glPushMatrix();
			glTranslatef(this->position.getX(), 0, this->position.getZ());
			glPushMatrix();
				//모델링
				glColor3f(1.0, 1.0, 0.0);
				glRotatef(this->rotate.getYaw(), 0, 1, 0);
				glScalef(0.3, 0.05, 1.0);
				glutSolidCube(0.1);
			glPopMatrix();
		glPopMatrix();

		//히트박스
		drawHitBox();
	}

	if (this->fragmentsFlag == true)
	{
		for (Particle* p : this->fragments)
		{
			p->drawParticle(1.0, 1.0, 0.0);
		}
	}
}