#include "Bullet.h"

Bullet::Bullet()
{
	this->type = 2;
	this->damage = 1;
	this->speed = 0.05;
	this->setPosition(15, 0, 0);
	this->setRange(0.05, 0.05);
	this->createHitBox();
}

Bullet::~Bullet() 
{

}

void Bullet::drawBullet()
{
	if (this->active == true)
	{
		glPushMatrix();
			glTranslatef(this->position.getX(), 0, this->position.getZ());
			glPushMatrix();
				//모델링
				glColor3f(1.0, 0.0, 0.0);
				glRotatef(this->rotate.getYaw(), 0, 1, 0);
				glutSolidSphere(0.05, 10, 10);
			glPopMatrix();
		glPopMatrix();

		//히트박스
		drawHitBox();
	}

	if (this->fragmentsFlag == true)
	{
		for (Particle* p : this->fragments)
		{
			p->drawParticle(1.0, 0.0, 0.0);
		}
	}
}