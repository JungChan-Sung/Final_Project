#include "Common.h"

Common::Common()
{
	this->movement = 0;
	this->fireDelay = 1000;
	this->speed = 0.04;
	this->maxSpeed = 0.05;
	this->acceleration = 0.0004;
	this->active = true;
	this->type = 1;
	this->hp = 3;
	this->maxHp = 3;
	this->rotSpeed = 0.2;
	this->maxRot = 45;
	this->setDirection(0, 0, -1);
	this->setRange(0.15, 0.2);
	this->createHitBox();
}

Common::~Common()
{

}

void Common::setMovement(int move)
{
	this->movement = move;
}

int Common::getMovement()
{
	return this->movement;
}

void Common::setBullet(Bullet* bullet)
{
	for (Bullet* b : this->bullet)
	{
		if (b->getObjNumber() == bullet->getObjNumber())
		{
			b = bullet;
			break;
		}
	}
}

std::list<Bullet*> Common::getBullet()
{
	return this->bullet;
}

void Common::createBullet()
{
	if ((current - this->fireTime) >= this->fireDelay && this->movement != 0)
	{
		int i = 0;
		Bullet* bullet[3];
		Vector dir;
		GLfloat angle = 15;

		this->fireTime = current;

		for (Bullet* b : bullet)
		{
			b = new Bullet();
			b->setPosition(this->getPosition());
			b->getPosition().addVector(0, 0, -0.25);
			b->setActive(true);
			b->setSpeed(b->getSpeed() + this->speed);
			this->direction.nomalVector();

			if (i == 0)
				dir.angleVector(0.0, this->direction);
			else if (i == 1)
				dir.angleVector(angle, this->direction);
			else if (i == 2)
				dir.angleVector(-angle, this->direction);

			b->setDirection(dir);
			this->bullet.push_back(b);
			i++;
		}
	}
}

void Common::updateBullet(Unit* chr)
{
	std::list<Bullet*> ::iterator b;

	for (b = this->bullet.begin(); b != this->bullet.end();)
	{
		Vector front;

		(*b)->updateFragments();
		if ((*b)->getActive() == true)
		{
			front.setVector((*b)->getDirection());
			front.nomalVector().mulVector((*b)->getSpeed());
			(*b)->getPosition().addVector(front);
		}

		if ((*b)->checkOutside()) // 맵 밖 나감
		{
			(*b)->setDestroyFlag(true);
		}
		else if ((*b)->checkCollision(chr) && chr->getCollision() == true && (*b)->getCollision() == true) // 캐릭터 충돌
		{
			Character* cPtr = (Character*)chr;
			(*b)->setCollision(false);
			cPtr->decHp((*b)->getDamage());
			cPtr->hitCharacter();
			(*b)->setDestroyFlag(true);
			break;
		}
		
		if ((*b)->getDestroyFlag() == true)
		{
			(*b)->createFragments();
			(*b)->setActive(false);
		}

		if ((*b)->getActive() == false && (*b)->getFragments().empty())
		{
			(*b)->setDeleteFlag(true);
		}

		if ((*b)->getDeleteFlag() == true)
		{
			(*b)->deleteObject();
			b = this->bullet.erase(b);
		}
		else
		{
			b++;
		}
	}
}

void Common::moveCommon()
{
	Vector dir;

	dir.setZ(-1);

	if (this->movement == 0)
	{
		this->speed -= this->acceleration;
		this->position += dir * this->speed;
		if (this->speed <= 0)
		{
			this->movement = rand() % 3 + 1; // 1, 2, 3
			this->waitTime = current;
			this->waitDelay = rand() % (4000 + 1 - 1500) + 1500; // 1.5초 ~ 4초
		}
	}
	else if ((current - this->waitTime) >= this->waitDelay)
	{
		if (this->movement == 1)
		{
			this->speed += this->acceleration;
			if (this->speed > this->maxSpeed)
				this->speed = this->maxSpeed;
			this->position += dir * this->speed;
		}
		else if (this->movement == 2)
		{
			//이동
			Vector left;

			this->moveTime += 1;
			left.setX(1);
			this->speed += this->acceleration;
			if (this->speed > this->maxSpeed)
				this->speed = this->maxSpeed;

			left *= this->speed * moveTime / 300.0;
			dir *= this->speed / 2.0;

			this->position += dir + left;

			//회전
			GLfloat roll = this->rotate.getRoll();

			roll -= this->rotSpeed;
			if (roll <= -this->maxRot)
				roll = -this->maxRot;

			this->rotate.setRoll(roll);
		}
		else if (this->movement == 3)
		{
			//이동
			Vector right;

			this->moveTime += 1;
			right.setX(-1);
			this->speed += this->acceleration;
			if (this->speed > this->maxSpeed)
				this->speed = this->maxSpeed;

			right *= this->speed * moveTime / 300.0;
			dir *= this->speed / 2.0;
			this->position += dir + right;

			//회전
			GLfloat roll = this->rotate.getRoll();

			roll += this->rotSpeed;
			if (roll >= this->maxRot)
				roll = this->maxRot;

			this->rotate.setRoll(roll);
		}
	}
}

void Common::drawCommon()
{
	if (this->destroyedFlag == false)
	{
		glPushMatrix();
			glTranslatef(this->position.getX(), 0, this->position.getZ());
			glPushMatrix();
				glRotatef(this->rotate.getRoll(), 0, 0, 1);
				//모델링
				glColor3f(1, 1, 1);
				glScalef(0.2, 0.2, 0.2);
				common.draw(10);
				//glColor3f(0.5, 0.5, 0.5);
				//glutSolidCube(0.2);
			glPopMatrix();
		glPopMatrix();

		//히트박스
		drawHitBox();

		//체력바
		drawHpBar();
	}

	//파편 그리기
	if (this->fragmentsFlag == true)
	{
		for (Particle* p : this->fragments)
		{
			p->drawParticle(0.31, 0.73, 0.87);
		}
	}

	// 총알 그리기
	for (Bullet* b : this->bullet)
	{
		b->drawBullet();
	}
}