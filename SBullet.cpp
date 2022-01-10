#include "SBullet.h"

SBullet::SBullet()
{
	this->type = 3;
	this->damage = 1;
	this->speed = 0.1;
	this->setPosition(20, 0, 0);
	this->setRange(0.025, 0.025);
	this->createHitBox();
}

SBullet::~SBullet()
{

}

void SBullet::drawSBullet()
{
	if (this->active == true)
	{
		glPushMatrix();
			glTranslatef(this->position.getX(), 0, this->position.getZ());
			glPushMatrix();
				//모델링
				glColor3f(0.0, 1.0, 0.0);
				glRotatef(this->rotate.getYaw(), 0, 1, 0);
				glScalef(1, 1, 10);
				glutSolidCube(0.025);
			glPopMatrix();
		glPopMatrix();

		//히트박스
		drawHitBox();
	}

	if (this->fragmentsFlag == true)
	{
		for (Particle* p : this->fragments)
		{
			p->drawParticle(0.0, 1.0, 0.0);
		}
	}
}