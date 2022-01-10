#include "PowBox.h"
#include "Character.h"

PowBox::PowBox()
{
	this->type = 2;
}

PowBox::~PowBox()
{

}

void PowBox::effectPow(Unit* chr)
{
	if (this->checkCollision(chr) && chr->getCollision() == true)
	{
		Character* cPtr = (Character*)chr;

		this->effectFlag = true;
		cPtr->createSatellite();
	}
}

void PowBox::drawPowBox()
{
	if (this->viewFlag == true)
	{
		glPushMatrix();
			glTranslatef(this->position.getX(), 0, this->position.getZ());
			glPushMatrix();
				//모델링
				glColor3f(1, 1, 1);
				glScalef(0.25, 0.25, 0.25);
				powBox.draw(21);
				//glColor3f(0, 0, 1);
				//glutSolidCube(0.1);
			glPopMatrix();
		glPopMatrix();
	}

	//히트박스
	drawHitBox();
}
