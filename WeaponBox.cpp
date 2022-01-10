#include "WeaponBox.h"
#include "Character.h"

WeaponBox::WeaponBox()
{
	this->type = 3;
}

WeaponBox::~WeaponBox()
{

}

void WeaponBox::effectWeapon(Unit* chr)
{
	if (this->checkCollision(chr) && chr->getCollision() == true)
	{
		Character* cPtr = (Character*)chr;

		this->effectFlag = true;
		cPtr->createPlusWeapon();
	}
}

void WeaponBox::drawWeaponBox()
{
	if (this->viewFlag == true)
	{
		glPushMatrix();
			glTranslatef(this->position.getX(), 0, this->position.getZ());
			glPushMatrix();
				//모델링
				glColor3f(1, 1, 1);
				glScalef(0.25, 0.25, 0.25);
				weaponBox.draw(22);
				//glColor3f(1, 0, 0);
				//glutSolidCube(0.1);
			glPopMatrix();
		glPopMatrix();
	}

	//히트박스
	drawHitBox();
}