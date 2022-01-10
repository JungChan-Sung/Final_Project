#include "Character.h"

Character::Character()
{
	this->hp = 5;
	this->fireDelay = 125;
	this->fireTime = 0;
	this->speed = 0.05;
	this->maxSpeed = 0.1;
	this->rotSpeed = 1;
	this->maxRot = 30;
	this->acceleration = 0.002;
	this->active = true;
	this->currentSpeed = this->speed;
	this->count = 0;
	this->life = 3;
	this->maxHp = 5;
	this->immortalStartTime = 0;
	this->immortalTime = 3000;
	this->respawnFlag = false;
	this->destroyedFlag = true;
	this->immortality = false;
	this->viewFlag = true;
	this->deathFlag = false;
	this->hpBarView = true;
	this->setPosition(0, 0, -5);
	this->setDirection(0, 0, 1);
	this->setRange(0.125, 0.225);
	this->createHitBox();
}

Character::~Character()
{
	for (Missile* m : this->missile)
	{
		delete m;
	}
	this->missile.clear();
}

void Character::setCount(int count)
{
	this->count = count;
}

int Character::getCount()
{
	this->count %= 2;
	return this->count;
}

void Character::setLife(int life)
{
	this->life = life;
}

int Character::getLife()
{
	return this->life;
}

void Character::setImmortalStartTime(int time)
{
	this->immortalStartTime = time;
}

int Character::getImmortalStartTime()
{
	return this->immortalStartTime;
}

void Character::setImmortalTime(int time)
{
	this->immortalTime = time;
}

int Character::getImmortalTime()
{
	return this->immortalTime;
}

void Character::setImmortality(bool flag)
{
	this->immortality = flag;
}

bool Character::getImmortality()
{
	return this->immortality;
}

void Character::setRespawnFlag(bool flag)
{
	this->respawnFlag = flag;
}

bool Character::getRespawnFlag()
{
	return this->respawnFlag;
}

void Character::setViewFlag(bool flag)
{
	this->viewFlag = flag;
}

bool Character::getViewFlag()
{
	return this->viewFlag;
}

void Character::setMissile(Missile* missile)
{
	for (Missile* m : this->missile)
	{
		if (m->getObjNumber() == missile->getObjNumber())
		{
			m = missile;
			break;
		}
	}
}

std::list<Missile*>& Character::getMissile()
{
	return this->missile;
}

void Character::setPlusWeapon(PlusWeapon* plus)
{
	for (PlusWeapon* p : this->weapon)
	{
		if (p->getObjNumber() == plus->getObjNumber())
		{
			p = plus;
			break;
		}
	}
}

std::vector<PlusWeapon*>& Character::getPlusWeapon()
{
	return this->weapon;
}

void Character::setSatellite(Satellite* satellite)
{
	for (Satellite* s : this->satellite)
	{
		if (s->getObjNumber() == satellite->getObjNumber())
		{
			s = satellite;
			break;
		}
	}
}

std::vector<Satellite*>& Character::getSatellite()
{
	return this->satellite;
}

void Character::move()
{
	Vector up, down, left, right, sum;

	if (keyStates[GLUT_KEY_UP] && this->getPosition().getZ() <= MAX_Z)
		up.setZ(1);

	if (keyStates[GLUT_KEY_DOWN] && this->getPosition().getZ() >= MIN_Z)
		down.setZ(-1);

	if (keyStates[GLUT_KEY_LEFT] && this->getPosition().getX() <= MAX_X)
		left.setX(1);

	if (keyStates[GLUT_KEY_RIGHT] && this->getPosition().getX() >= MIN_X)
		right.setX(-1);

	sum = up + down + left + right;

	sum.nomalVector();
	this->position += sum * this->speed;
}

void Character::axis()
{
	GLfloat roll = this->rotate.getRoll();
	if (keyStates[GLUT_KEY_LEFT])
	{
		roll -= this->rotSpeed;
		if (roll <= -this->maxRot)
			roll = -this->maxRot;
	}

	if (keyStates[GLUT_KEY_RIGHT])
	{
		roll += this->rotSpeed;
		if (roll >= this->maxRot)
			roll = this->maxRot;
	}

	if (!keyStates[GLUT_KEY_LEFT])
	{
		if (roll <= 0)
			roll += this->rotSpeed;
	}

	if (!keyStates[GLUT_KEY_RIGHT])
	{
		if (roll >= 0)
			roll -= this->rotSpeed;
	}

	if (keyStates[GLUT_KEY_LEFT] && keyStates[GLUT_KEY_RIGHT])
	{
		if (roll < 0)
			roll += this->rotSpeed;
		else if (roll > 0)
			roll -= this->rotSpeed;
	}

	this->rotate.setRoll(roll);
}


void Character::launch()
{
	if (keyStates['z'])
	{
		if ((current - this->fireTime) >= this->fireDelay)
		{
			this->createMissile();
		}

		if (!this->satellite.empty() && (current - this->satellite[0]->getFireTime()) >= this->satellite[0]->getFireDelay())
		{
			this->satellite[0]->setFireFlag(true);
		}
	}
}

void Character::createMissile()
{
	Missile* missile = new Missile();
	GLfloat x = (this->count % 2 == 0) ? 0.1 : -0.1;
	this->count += 1;
	this->fireTime = current;
	missile->setPosition(this->position);
	missile->getPosition().addVector(x, 0, 0);
	missile->setActive(true);
	this->missile.push_back(missile);
}

void Character::updateMissile(std::list<Enemy*>& enemies)
{
	std::list<Missile*> ::iterator m;
	std::list<Enemy*> ::iterator e;

	for (m = this->missile.begin(); m != this->missile.end();)
	{
		Vector front;

		(*m)->updateFragments();
		if ((*m)->getActive() == true)
		{
			front.setVector((*m)->getDirection());
			front.nomalVector();
			(*m)->getPosition() += front * (*m)->getSpeed();
		}

		if ((*m)->checkOutside()) // 맵 밖 나감
		{
			(*m)->setDestroyFlag(true);
		}

		for (e = enemies.begin(); e != enemies.end();) // 적 리스트 가져옴
		{
			if ((*m)->checkCollision(*e) && (*e)->getCollision() == true && (*m)->getCollision() == true) // 적 충돌 감지
			{
				(*m)->setCollision(false);
				(*e)->decHp((*m)->getDamage()); // 적 체력 감소
				(*e)->setHpBarView(true);
				(*e)->checkDestroyed();
				(*m)->setDestroyFlag(true);
				break;
			}
			else
				e++;
		}

		if ((*m)->getDestroyFlag() == true)
		{
			(*m)->createFragments();
			(*m)->setActive(false);
		}

		if ((*m)->getActive() == false && (*m)->getFragments().empty())
		{
			(*m)->setDeleteFlag(true);
		}
		
		if ((*m)->getDeleteFlag() == true)
		{
			(*m)->deleteObject();
			m = this->missile.erase(m);
		}
		else
		{
			m++;
		}
	}
}

void Character::checkRespawn()
{
	if (this->respawnFlag == false)
	{
		if (this->life > 0)
		{
			this->position.setVector(0, 0, -5);
			this->rotate.setRoll(0);
			this->speed = this->maxSpeed;
			this->life -= 1;
			this->respawnFlag = true;
			this->immortality = true;
			this->immortalStartTime = current;
			this->immortalTime = 3000;
		}
		else
		{
			this->position.setVector(0, 0, -10);
			this->viewFlag = false;
			this->collision = false;
			this->deathFlag = true;
		}
	}
}

void Character::respawnCharacter()
{
	if (this->respawnFlag == true)
	{
		Vector dir;

		dir.setZ(1);
		this->speed -= this->acceleration;
		this->position += dir * this->speed;

		if (this->speed < 0)
		{
			this->respawnFlag = false;
			this->destroyedFlag = false;
			this->speed = this->currentSpeed;
			this->hp = this->maxHp;
			this->fragmentsFlag = false;
		}
	}
}

void Character::immortalCharacter()
{
	if (immortality == true)
	{
		this->collision = false;
		this->viewToggle();
		this->updateHitBox();
		if ((current - this->immortalStartTime) >= this->immortalTime)
		{
			this->collision = true;
			this->immortality = false;
			this->viewFlag = true;
		}
	}
}

void Character::viewToggle()
{
	const int toggleTime = 150;
	if ((current - this->immortalStartTime) % toggleTime <= (toggleTime * 0.5))
	{
		viewFlag = false;
	}
	else if ((current - this->immortalStartTime) % toggleTime > (toggleTime * 0.5))
	{
		viewFlag = true;
	}
}

void Character::hitCharacter()
{
	this->immortality = true;
	this->immortalStartTime = current;
	this->immortalTime = 1000;
}

void Character::createPlusWeapon()
{
	int count = this->weapon.size();
	if (count < 16)
	{
		const GLfloat disX = 0.025;
		const GLfloat disY = 0.125;
		const GLfloat disZ = 0.025;
		const GLfloat dirAngle = 10;
		GLfloat x, y, z, angle;
		PlusWeapon* p = new PlusWeapon();

		x = 0.125;
		y = disY;
		z = 0.1;
		angle = 0;

		if (count % 2 == 0 && count > 0) // 왼쪽
		{
			x = this->weapon[count - 2]->getRelativePos().getX() + disX;
			if (count > 1)
			{
				angle = this->weapon[count - 2]->getRotate().getYaw() + dirAngle;
			}
		}
		else if (count % 2 == 1 && count > 0) // 오른쪽
		{
			x = -this->weapon[count - 1]->getRelativePos().getX();
			if (count > 1)
			{
				angle = this->weapon[count - 2]->getRotate().getYaw() - dirAngle;
			}
		}

		if (count > 1)
		{
			z = this->weapon[count - 2]->getRelativePos().getZ() - disZ;
		}

		p->getRotate().setYaw(angle);
		p->getRelativePos().setVector(x, y, z);
		p->getPosition().setVector(this->position);
		p->getPosition().addVector(p->getRelativePos());
		this->weapon.push_back(p);
	}
}

void Character::createSatellite()
{
	int count = this->satellite.size();
	if (count < 8)
	{
		const GLfloat disX = 0.375;
		const GLfloat disY = 0.1;
		const GLfloat disZ = 0.25;
		GLfloat x, y, z;
		Satellite* s = new Satellite();

		s->getPosition().setVector(0, 0, -5);

		x = 0.5;
		y = disY;
		z = -0.375;

		if (count % 2 == 0 && count > 0) // 왼쪽
		{
			x = this->satellite[count - 2]->getRelativePos().getX() + disX;
		}
		else if (count % 2 == 1 && count > 0) // 오른쪽
		{
			x = -this->satellite[count - 1]->getRelativePos().getX();
		}

		if (count > 1)
		{
			z = this->satellite[count - 2]->getRelativePos().getZ() - disZ;
			s->setAcceleration(this->satellite[count - 2]->getAcceleration() * 0.75);
		}

		if (count % 4 >= 2 && count > 1)
		{
			y = -this->satellite[count - 2]->getRelativePos().getY();
		}

		s->getRelativePos().setVector(x, y, z);
		this->satellite.push_back(s);
	}
}

void Character::volleySatellite()
{
	const GLfloat fireInterval = 200;
	std::vector<Satellite*>::iterator s;

	for (s = this->satellite.begin(); s != this->satellite.end(); s++)
	{
		(*s)->launchSatellite();
		if ((*s) != this->satellite.back() 
			&& (current - (*s)->getFireTime()) >= fireInterval 
			&& (*s)->getFireFlag() == true)
		{
			(*s)->setFireFlag(false);
			(*(s + 1))->setFireFlag(true);
		}
		else if ((*s) == this->satellite.back())
		{
			(*s)->setFireFlag(false);
		}
	}
}

void Character::drawCharacter()
{
	if (this->viewFlag == true)
	{
		glPushMatrix();
			glTranslatef(this->position.getX(), 0, this->position.getZ());
			glPushMatrix();
				//모델링
				glColor3f(1, 1, 1);
				glRotatef(this->rotate.getRoll(), 0, 0, 1);
				glScalef(0.2, 0.2, 0.2);
				character.draw(0);
				//glColor3f(0.25, 0.25, 0.25);
				//glRotatef(this->rotate.getRoll(), 0, 0, 1);
				//glScalef(1, 0.5, 2);
				//glutSolidCube(0.2);
				//glutWireCube(0.2);
			glPopMatrix();
		glPopMatrix();
	}

	if (this->immortality == false)
	{
		//히트박스
		drawHitBox();
	}

	drawHpBar();

	//파편 그리기
	if (this->fragmentsFlag == true)
	{
		for (Particle* p : this->fragments)
		{
			p->drawParticle(0.25, 0.25, 0.25);
		}
	}

	//미사일 그리기
	for (Missile* m : this->missile)
	{
		m->drawMissile();
	}
}