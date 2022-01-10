#include "Game.h"
#include "Vector.h"
#include "Axis.h"
#include "Object.h"
#include "Unit.h"
#include "Projectile.h"
#include "Missile.h"
#include "Character.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Common.h"
#include "Turret.h"
#include "Boss.h"
#include "EnemyManager.h"
#include "Item.h"
#include "HealBox.h"
#include "PowBox.h"
#include "ObjLoader.h"

using namespace std;

GLfloat camx = 0, camy = 10, camz = 0;
GLfloat cam2x = 0, cam2y = 0, cam2z = 0;
GLfloat cam_upx = 0, cam_upy = 1, cam_upz = 1;

//게임관련
bool keyStates[256];
int initFlag = 0;
int hitBoxView = 0;
int healthBarView = 0;
int subWeaponView = 0;
bool bossDeath = false;
int bossDeathTime = 0;
int bossDeathDelay = 500;
bool gameClear = false;
int gameClearTime = 0;
int gameClearDelay = 1000;
bool endingFlag = false;
GLfloat starPosition[STAR_COUNT][3];

//시간관련
clock_t start;
int current;

//객체관련
Character* spaceCraft;
EnemyManager* emg;
list<Item*> items;

//텍스쳐관련
cv::Mat image[TEXTURE_COUNT];
GLuint tex_ids[TEXTURE_COUNT] = { 1 };
int nTex;

//모델관련
CModel character;
CModel satellite;
//CModel plusWeapon;
CModel common;
CModel turretHead;
CModel turretBody;
//CModel boss;
CModel healBox;
CModel powBox;
CModel weaponBox;

void InitLight()
{
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	GLfloat ambientLight[] = { 0.3f,0.3f,0.3f,1.0f };
	GLfloat diffuseLight[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat specular[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat specref[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat position[] = { 30.0,30.0,30.0,1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
}
void init_texture()
{
	glGenTextures(nTex, tex_ids);
	glEnable(GL_TEXTURE_2D);
}
void InitStarPosition()
{
	for (int i = 0; i < STAR_COUNT; i++)
	{
		GLfloat x, y, z;
		int posY = rand() % 3 + 1; // 1 ~ 3
		switch (posY)
		{
		case 1:
			y = -10;
			break;
		case 2:
			y = -17.5;
			break;
		case 3:
			y = -25;
			break;
		}

		x = (fmod(rand(), ((MAX_X * 3) * 1000 * 2) + 1) - ((MAX_X * 3) * 1000)) / 1000;
		z = (fmod(rand(), ((MAX_Z * 3) * 1000 * 2) + 1) - ((MAX_Z * 3) * 1000)) / 1000;

		starPosition[i][0] = x;
		starPosition[i][1] = y;
		starPosition[i][2] = z;
	}
}
void drawStars()
{
	for (int i = 0; i < STAR_COUNT; i++)
	{
		glPushMatrix();
			glColor3f(1, 1, 0.8);
			glTranslatef(starPosition[i][0], starPosition[i][1], starPosition[i][2]);
			glutSolidCube(0.01);
		glPopMatrix();
	}
}
void updateStars()
{
	for (int i = 0; i < STAR_COUNT; i++)
	{
		starPosition[i][2] -= 0.005;
		if (starPosition[i][2] <= MIN_Z * 3)
		{
			starPosition[i][0] = (fmod(rand(), ((MAX_X * 3) * 1000 * 2) + 1) - ((MAX_X * 3) * 1000)) / 1000;
			starPosition[i][2] = MAX_Z * 3;
		}
	}
}
void drawBackground()
{
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);

		int idx = 0;
		glBindTexture(GL_TEXTURE_2D, tex_ids[idx]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[idx].cols, image[idx].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[idx].data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLfloat v1[3] = { -30,-30,20 };
		GLfloat v2[3] = { 30,-30,20 };
		GLfloat v3[3] = { 30,-30,-20 };
		GLfloat v4[3] = { -30,-30,-20 };

		glBegin(GL_POLYGON);
			glNormal3f(0, -1, 0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(v1[0], v1[1], v1[2]);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(v2[0], v2[1], v2[2]);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(v3[0], v3[1], v3[2]);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(v4[0], v4[1], v4[2]);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camx, camy, camz,
		cam2x, cam2y, cam2z,
		cam_upx, cam_upy, cam_upz);

	//배경 그리기
	drawStars();
	drawBackground();

	//캐릭터 그리기
	spaceCraft->drawCharacter();
	//캐릭터 보조무기 그리기
	for (Satellite* s : spaceCraft->getSatellite())
	{
		s->drawSatellite();
	}
	for (PlusWeapon* p : spaceCraft->getPlusWeapon())
	{
		p->drawPlusWeapon();
	}

	//적 그리기
	for (Enemy* e : emg->getEnermies())
	{
		if (e->getType() == 1)
		{
			Common* cPtr = (Common*)e;
			cPtr->drawCommon();
		}
		else if (e->getType() == 2)
		{
			Turret* tPtr = (Turret*)e;
			tPtr->drawTurretHead();
			tPtr->drawTurretBody();
		}
		else if (e->getType() == 3)
		{
			Boss* bPtr = (Boss*)e;
			bPtr->drawBoss();
		}
	}

	//아이템 그리기
	for (Item* i : items)
	{
		if (i->getType() == 1)
		{
			HealBox* hPtr = (HealBox*)i;
			hPtr->drawHealBox();
		}
		else if (i->getType() == 2)
		{
			PowBox* pPtr = (PowBox*)i;
			pPtr->drawPowBox();
		}
		else if (i->getType() == 3)
		{
			WeaponBox* pPtr = (WeaponBox*)i;
			pPtr->drawWeaponBox();
		}
	}

	glutSwapBuffers();
}
void reshape(int width, int height)
{
	GLfloat left = 0, bottom = 0;
	glViewport(left, bottom, width - left, height - bottom);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat ratio = (float)(width - left) / (height - bottom);
	gluPerspective(40, ratio, 0.1, 50);
}
void timer(int value)
{
	glutTimerFunc(1000 / 60, timer, 1);
	list<Enemy*>::iterator e;
	list<Item*>::iterator i;
	vector<Satellite*>::iterator s;
	vector<PlusWeapon*>::iterator p;

	current = (int)clock(); // 현재 시간 측정
	updateStars(); // 별들 업데이트

	//캐릭터 업데이트
	if (spaceCraft->getDestroyedFlag() == false && bossDeath == false)
	{
		spaceCraft->move();
		spaceCraft->axis();
		spaceCraft->launch();
		spaceCraft->volleySatellite();
		spaceCraft->checkDestroyed();
	}
	else if (spaceCraft->getDestroyedFlag() == true)
	{
		spaceCraft->createDeathFragments();
		spaceCraft->checkRespawn();
		for (Satellite* s : spaceCraft->getSatellite())
			s->setDestroyedFlag(true);
		for (PlusWeapon* p : spaceCraft->getPlusWeapon())
			p->setDestroyedFlag(true);
	}
	spaceCraft->respawnCharacter();
	spaceCraft->updateMissile(emg->getEnermies());
	spaceCraft->immortalCharacter();
	spaceCraft->updateFragments();
	//캐릭터 보조무기 업데이트
	for (s = spaceCraft->getSatellite().begin(); s != spaceCraft->getSatellite().end();)
	{
		bool flag = false;
		(*s)->moveSatellite(spaceCraft);
		(*s)->axisSatellite();
		(*s)->updateBullet(emg->getEnermies());
		if ((*s)->getDestroyedFlag() == true && (*s)->getSBullet().empty())
		{
			(*s)->setActive(false);
			(*s)->deleteObject();
			s = spaceCraft->getSatellite().erase(s);
			flag = true;
		}

		if (flag == false)
			s++;
	}
	for (p = spaceCraft->getPlusWeapon().begin(); p != spaceCraft->getPlusWeapon().end();)
	{
		bool flag = false;
		(*p)->movePlusWeapon(spaceCraft);
		if (bossDeath == false)
			(*p)->launchPlusWeapon();
		(*p)->updateBullet(emg->getEnermies());
		if ((*p)->getDestroyedFlag() == true && (*p)->getSBullet().empty())
		{
			(*p)->setActive(false);
			(*p)->deleteObject();
			p = spaceCraft->getPlusWeapon().erase(p);
			flag = true;
		}

		if (flag == false)
			p++;
	}

	//적 생성
	if (emg->getBossSpawn() == false)
	{
		emg->createCommon();
		emg->createTurret();
		emg->createBoss();
	}

	//적 업데이트
	for (e = emg->getEnermies().begin(); e != emg->getEnermies().end();)
	{
		bool flag = false;

		if ((*e)->getPosition().getZ() <= MIN_Z)
		{
			(*e)->setDestroyedFlag(true);
		}

		if ((*e)->getType() == 1) // 일반 적
		{
			Common* cPtr = (Common*)(*e);
			cPtr->updateBullet(spaceCraft);
			cPtr->moveCommon();
			cPtr->updateFragments();
			if (cPtr->getDestroyedFlag() == false)
			{
				cPtr->createBullet();
			}
			else if (cPtr->getDestroyedFlag() == true)
			{
				cPtr->setCollision(false); // 충돌 무시
				if (cPtr->getHp() <= 0) // 적 처치시
				{
					cPtr->createItem(items); // 아이템 생성
					cPtr->createDeathFragments(); // 파편 생성
				}
				if (cPtr->getBullet().empty() && cPtr->getFragments().empty()) // 화면에 오브젝트가 다사라지면 삭제
				{
					cPtr->setActive(false);
					cPtr->deleteObject();
					e = emg->getEnermies().erase(e);
					flag = true;
				}
			}
		}
		else if ((*e)->getType() == 2) // 터렛형 적
		{
			Turret* tPtr = (Turret*)(*e);
			tPtr->updateBullet(spaceCraft);
			tPtr->moveTurrect();
			tPtr->updateFragments();
			if (tPtr->getDestroyedFlag() == false)
			{
				tPtr->updateTurret(spaceCraft);
				tPtr->createBullet();
			}
			else if (tPtr->getDestroyedFlag() == true)
			{
				tPtr->setCollision(false); // 충돌 무시
				if (tPtr->getHp() <= 0) // 적 처치시
				{
					tPtr->createItem(items); // 아이템 생성
					tPtr->createDeathFragments(); // 파편 생성
				}
				if (tPtr->getBullet().empty() && tPtr->getFragments().empty()) // 화면에 오브젝트가 다사라지면 삭제
				{
					tPtr->setActive(false);
					tPtr->deleteObject();
					e = emg->getEnermies().erase(e);
					flag = true;
				}
			}
		}
		else if ((*e)->getType() == 3) // 보스
		{
			Boss* bPtr = (Boss*)(*e);
			bPtr->updateFragments();
			bPtr->updateProjectile(spaceCraft);
			if (bPtr->getDestroyedFlag() == false)
			{
				bPtr->patternBoss(spaceCraft, emg->getEnermies());
				bPtr->moveBoss();
			}
			else if (bPtr->getDestroyedFlag() == true)
			{
				Vector dir;
				dir.setVector(0, -1, -1);
				bPtr->setCollision(false); // 충돌 무시
				bPtr->setBossState(3);
				bPtr->setSpeed(0.001);
				bPtr->getPosition() += dir * bPtr->getSpeed();
				if (bPtr->getHp() <= 0) // 적 처치시
				{
					bPtr->createDeathFragments(); // 파편 생성
					if (bossDeath == false)
					{
						bossDeath = true;
						bossDeathTime = current;
					}
				}
				if (bPtr->getProjectile().empty() && bPtr->getFragments().empty() && bPtr->getPosition().getY() <= -3) // 화면에 오브젝트가 다사라지면 삭제
				{
					bPtr->setActive(false);
					bPtr->deleteObject();
					e = emg->getEnermies().erase(e);
					flag = true;
				}
			}
		}

		if (flag == false)
			e++;
	}

	//아이템 업데이트
	for (i = items.begin(); i != items.end();)
	{
		bool flag = false;
		if ((*i)->getType() == 1)
		{
			HealBox* hPtr = (HealBox*)(*i);
			hPtr->moveItem();
			hPtr->updateItem();
			if (hPtr->getEffectFlag() == false)
			{
				hPtr->effectHeal(spaceCraft);
			}
			else if (hPtr->getEffectFlag() == true)
			{
				hPtr->setCollision(false);
				hPtr->setActive(false);
				hPtr->deleteObject();
				i = items.erase(i);
				flag = true;
			}
		}
		else if ((*i)->getType() == 2)
		{
			PowBox* pPtr = (PowBox*)(*i);
			pPtr->moveItem();
			pPtr->updateItem();
			if (pPtr->getEffectFlag() == false)
			{
				pPtr->effectPow(spaceCraft);
			}
			else if (pPtr->getEffectFlag() == true)
			{
				pPtr->setCollision(false);
				pPtr->setActive(false);
				pPtr->deleteObject();
				i = items.erase(i);
				flag = true;
			}
		}
		else if ((*i)->getType() == 3)
		{
			WeaponBox* wPtr = (WeaponBox*)(*i);
			wPtr->moveItem();
			wPtr->updateItem();
			if (wPtr->getEffectFlag() == false)
			{
				wPtr->effectWeapon(spaceCraft);
			}
			else if (wPtr->getEffectFlag() == true)
			{
				wPtr->setCollision(false);
				wPtr->setActive(false);
				wPtr->deleteObject();
				i = items.erase(i);
				flag = true;
			}
		}

		if (flag == false)
			i++;
	}

	if (bossDeath == true)
	{
		if (gameClear == false)
		{
			GLfloat roll = spaceCraft->getRotate().getRoll();
			if (roll <= 0)
				roll += spaceCraft->getRotSpeed();
			else if (roll >= 0)
				roll -= spaceCraft->getRotSpeed();
			spaceCraft->getRotate().setRoll(roll);
		}

		if ((current - bossDeathTime) >= bossDeathDelay)
		{
			for (Enemy* e : emg->getEnermies())
			{
				e->setDestroyedFlag(true);
				e->setHp(0);
			}
			for (Item* i : items)
			{
				i->setEffectFlag(true);
			}

			if (gameClear == false)
			{
				gameClear = true;
				gameClearTime = current;
				spaceCraft->setSpeed(0.025);
				spaceCraft->setRotSpeed(0.5);
			}
		}

		//엔딩
		if (gameClear == true && (current - gameClearTime) >= gameClearDelay)
		{
			Vector dir;
			GLfloat roll = spaceCraft->getRotate().getRoll();
			GLfloat speed = spaceCraft->getSpeed();
			spaceCraft->setCollision(false);
			if (spaceCraft->getPosition().getX() < -0.25) // 우측
			{
				dir.setX(1); // 좌로 이동
				spaceCraft->getPosition() += dir * speed;
				roll -= spaceCraft->getRotSpeed();
				if (roll <= -spaceCraft->getMaxRot())
					roll = -spaceCraft->getMaxRot();
				spaceCraft->getRotate().setRoll(roll);
			}
			else if (spaceCraft->getPosition().getX() > 0.25) // 좌측
			{
				dir.setX(-1); // 우로 이동
				spaceCraft->getPosition() += dir * speed;
				roll += spaceCraft->getRotSpeed();
				if (roll >= spaceCraft->getMaxRot())
					roll = spaceCraft->getMaxRot();
				spaceCraft->getRotate().setRoll(roll);
			}
			else // 중앙
			{
				if (endingFlag == false)
				{
					spaceCraft->setSpeed(0);
					spaceCraft->setAcceleration(0.001);
					endingFlag = true;
				}
				dir.setZ(1);
				if (roll <= 0)
					roll += spaceCraft->getRotSpeed();
				else if (roll >= 0)
					roll -= spaceCraft->getRotSpeed();
				spaceCraft->getRotate().setRoll(roll);

				speed += spaceCraft->getAcceleration();
				spaceCraft->setSpeed(speed);
				spaceCraft->getPosition() += dir * speed;
			}
		}
	}

	glutPostRedisplay();
}
void keyboardSDown(int KeyPressed, int x, int y)
{
	if (KeyPressed == GLUT_KEY_UP)
	{
		keyStates[GLUT_KEY_UP] = true;
	}
	if (KeyPressed == GLUT_KEY_DOWN)
	{
		keyStates[GLUT_KEY_DOWN] = true;
	}
	if (KeyPressed == GLUT_KEY_LEFT)
	{
		keyStates[GLUT_KEY_LEFT] = true;
	}
	if (KeyPressed == GLUT_KEY_RIGHT)
	{
		keyStates[GLUT_KEY_RIGHT] = true;
	}
	if (KeyPressed == GLUT_KEY_F1)
		hitBoxView = (++hitBoxView) % 2;
	if (KeyPressed == GLUT_KEY_F2)
		healthBarView = (++healthBarView) % 2;
	if (KeyPressed == GLUT_KEY_F3 && spaceCraft->getDestroyedFlag() == false)
		spaceCraft->createSatellite();
	if (KeyPressed == GLUT_KEY_F4 && spaceCraft->getDestroyedFlag() == false)
		spaceCraft->createPlusWeapon();
	if (KeyPressed == GLUT_KEY_F5)
		subWeaponView = (++subWeaponView) % 2;
}
void keyboardSUp(int KeyPressed, int x, int y)
{
	if (KeyPressed == GLUT_KEY_UP)
	{
		keyStates[GLUT_KEY_UP] = false;
	}
	if (KeyPressed == GLUT_KEY_DOWN)
	{
		keyStates[GLUT_KEY_DOWN] = false;
	}
	if (KeyPressed == GLUT_KEY_LEFT)
	{
		keyStates[GLUT_KEY_LEFT] = false;
	}
	if (KeyPressed == GLUT_KEY_RIGHT)
	{
		keyStates[GLUT_KEY_RIGHT] = false;
	}
}
void keyboardDown(unsigned char KeyPressed, int x, int y)
{
	switch (KeyPressed)
	{
	case 'z':
	case 'Z':
		keyStates['z'] = true;
		break;
	case 'x':
	case 'X':
		keyStates['x'] = true;
		break;
	}
}
void keyboardUp(unsigned char KeyPressed, int x, int y)
{
	switch (KeyPressed)
	{
	case 'z':
	case 'Z':
		keyStates['z'] = false;
		break;
	case 'x':
	case 'X':
		keyStates['x'] = false;
		break;
	}
}
void idle()
{
	//list<Enemy*>::iterator e;
	//list<Item*>::iterator i;
	//vector<Satellite*>::iterator s;
	//vector<PlusWeapon*>::iterator p;

	//current = (int)clock(); // 현재 시간 측정
	//updateStars(); // 별들 업데이트

	////캐릭터 업데이트
	//if (spaceCraft->getDestroyedFlag() == false && bossDeath == false)
	//{
	//	spaceCraft->move();
	//	spaceCraft->axis();
	//	spaceCraft->launch();
	//	spaceCraft->volleySatellite();
	//	spaceCraft->checkDestroyed();
	//}
	//else if (spaceCraft->getDestroyedFlag() == true)
	//{
	//	spaceCraft->createDeathFragments();
	//	spaceCraft->checkRespawn();
	//	for (Satellite* s : spaceCraft->getSatellite())
	//		s->setDestroyedFlag(true);
	//	for (PlusWeapon* p : spaceCraft->getPlusWeapon())
	//		p->setDestroyedFlag(true);
	//}
	//spaceCraft->respawnCharacter();
	//spaceCraft->updateMissile(emg->getEnermies());
	//spaceCraft->immortalCharacter();
	//spaceCraft->updateFragments();
	////캐릭터 보조무기 업데이트
	//for (s = spaceCraft->getSatellite().begin(); s!=spaceCraft->getSatellite().end();)
	//{
	//	bool flag = false;
	//	(*s)->moveSatellite(spaceCraft);
	//	(*s)->axisSatellite();
	//	(*s)->updateBullet(emg->getEnermies());
	//	if ((*s)->getDestroyedFlag() == true && (*s)->getSBullet().empty())
	//	{
	//		(*s)->setActive(false);
	//		(*s)->deleteObject();
	//		s = spaceCraft->getSatellite().erase(s);
	//		flag = true;
	//	}

	//	if (flag == false)
	//		s++;
	//}
	//for (p = spaceCraft->getPlusWeapon().begin(); p != spaceCraft->getPlusWeapon().end();)
	//{
	//	bool flag = false;
	//	(*p)->movePlusWeapon(spaceCraft);
	//	if(bossDeath == false)
	//		(*p)->launchPlusWeapon();
	//	(*p)->updateBullet(emg->getEnermies());
	//	if ((*p)->getDestroyedFlag() == true && (*p)->getSBullet().empty())
	//	{
	//		(*p)->setActive(false);
	//		(*p)->deleteObject();
	//		p = spaceCraft->getPlusWeapon().erase(p);
	//		flag = true;
	//	}

	//	if (flag == false)
	//		p++;
	//}

	////적 생성
	//if (emg->getBossSpawn() == false)
	//{
	//	emg->createCommon();
	//	emg->createTurret();
	//	emg->createBoss();
	//}

	////적 업데이트
	//for (e = emg->getEnermies().begin(); e != emg->getEnermies().end();)
	//{
	//	bool flag = false;

	//	if ((*e)->getPosition().getZ() <= MIN_Z)
	//	{
	//		(*e)->setDestroyedFlag(true);
	//	}

	//	if ((*e)->getType() == 1) // 일반 적
	//	{
	//		Common* cPtr = (Common*)(*e);
	//		cPtr->updateBullet(spaceCraft);
	//		cPtr->moveCommon();
	//		cPtr->updateFragments();
	//		if (cPtr->getDestroyedFlag() == false)
	//		{
	//			cPtr->createBullet();
	//		}
	//		else if (cPtr->getDestroyedFlag() == true)
	//		{
	//			cPtr->setCollision(false); // 충돌 무시
	//			if (cPtr->getHp() <= 0) // 적 처치시
	//			{
	//				cPtr->createItem(items); // 아이템 생성
	//				cPtr->createDeathFragments(); // 파편 생성
	//			}
	//			if (cPtr->getBullet().empty() && cPtr->getFragments().empty()) // 화면에 오브젝트가 다사라지면 삭제
	//			{
	//				cPtr->setActive(false);
	//				cPtr->deleteObject();
	//				e = emg->getEnermies().erase(e);
	//				flag = true;
	//			}
	//		}
	//	}
	//	else if ((*e)->getType() == 2) // 터렛형 적
	//	{
	//		Turret* tPtr = (Turret*)(*e);
	//		tPtr->updateBullet(spaceCraft);
	//		tPtr->moveTurrect();
	//		tPtr->updateFragments();
	//		if (tPtr->getDestroyedFlag() == false)
	//		{
	//			tPtr->updateTurret(spaceCraft);
	//			tPtr->createBullet();
	//		}
	//		else if(tPtr->getDestroyedFlag() == true)
	//		{
	//			tPtr->setCollision(false); // 충돌 무시
	//			if (tPtr->getHp() <= 0) // 적 처치시
	//			{
	//				tPtr->createItem(items); // 아이템 생성
	//				tPtr->createDeathFragments(); // 파편 생성
	//			}
	//			if (tPtr->getBullet().empty() && tPtr->getFragments().empty()) // 화면에 오브젝트가 다사라지면 삭제
	//			{
	//				tPtr->setActive(false);
	//				tPtr->deleteObject();
	//				e = emg->getEnermies().erase(e);
	//				flag = true;
	//			}
	//		}
	//	}
	//	else if ((*e)->getType() == 3) // 보스
	//	{
	//		Boss* bPtr = (Boss*)(*e);
	//		bPtr->updateFragments();
	//		bPtr->updateProjectile(spaceCraft);
	//		if (bPtr->getDestroyedFlag() == false)
	//		{
	//			bPtr->patternBoss(spaceCraft, emg->getEnermies());
	//			bPtr->moveBoss();
	//		}
	//		else if (bPtr->getDestroyedFlag() == true)
	//		{
	//			Vector dir;
	//			dir.setVector(0, -1, -1);
	//			bPtr->setCollision(false); // 충돌 무시
	//			bPtr->setBossState(3);
	//			bPtr->setSpeed(0.005);
	//			bPtr->getPosition() += dir * bPtr->getSpeed();
	//			if (bPtr->getHp() <= 0) // 적 처치시
	//			{
	//				bPtr->createDeathFragments(); // 파편 생성
	//				if (bossDeath == false)
	//				{
	//					bossDeath = true;
	//					bossDeathTime = current;
	//				}
	//			}
	//			if (bPtr->getProjectile().empty() && bPtr->getFragments().empty() && bPtr->getPosition().getY() <= -3) // 화면에 오브젝트가 다사라지면 삭제
	//			{
	//				bPtr->setActive(false);
	//				bPtr->deleteObject();
	//				e = emg->getEnermies().erase(e);
	//				flag = true;
	//			}
	//		}
	//	}

	//	if (flag == false)
	//		e++;
	//}

	////아이템 업데이트
	//for (i = items.begin(); i != items.end();)
	//{
	//	bool flag = false;
	//	if ((*i)->getType() == 1)
	//	{
	//		HealBox* hPtr = (HealBox*)(*i);
	//		hPtr->moveItem();
	//		hPtr->updateItem();
	//		if (hPtr->getEffectFlag() == false)
	//		{
	//			hPtr->effectHeal(spaceCraft);
	//		}
	//		else if (hPtr->getEffectFlag() == true)
	//		{
	//			hPtr->setCollision(false);
	//			hPtr->setActive(false);
	//			hPtr->deleteObject();
	//			i = items.erase(i);
	//			flag = true;
	//		}
	//	}
	//	else if ((*i)->getType() == 2)
	//	{
	//		PowBox* pPtr = (PowBox*)(*i);
	//		pPtr->moveItem();
	//		pPtr->updateItem();
	//		if (pPtr->getEffectFlag() == false)
	//		{
	//			pPtr->effectPow(spaceCraft);
	//		}
	//		else if (pPtr->getEffectFlag() == true)
	//		{
	//			pPtr->setCollision(false);
	//			pPtr->setActive(false);
	//			pPtr->deleteObject();
	//			i = items.erase(i);
	//			flag = true;
	//		}
	//	}
	//	else if ((*i)->getType() == 3)
	//	{
	//		WeaponBox* wPtr = (WeaponBox*)(*i);
	//		wPtr->moveItem();
	//		wPtr->updateItem();
	//		if (wPtr->getEffectFlag() == false)
	//		{
	//			wPtr->effectWeapon(spaceCraft);
	//		}
	//		else if (wPtr->getEffectFlag() == true)
	//		{
	//			wPtr->setCollision(false);
	//			wPtr->setActive(false);
	//			wPtr->deleteObject();
	//			i = items.erase(i);
	//			flag = true;
	//		}
	//	}

	//	if (flag == false)
	//		i++;
	//}

	//if (bossDeath == true)
	//{
	//	if (gameClear == false)
	//	{
	//		GLfloat roll = spaceCraft->getRotate().getRoll();
	//		if (roll <= 0)
	//			roll += spaceCraft->getRotSpeed();
	//		else if (roll >= 0)
	//			roll -= spaceCraft->getRotSpeed();
	//		spaceCraft->getRotate().setRoll(roll);
	//	}

	//	if ((current - bossDeathTime) >= bossDeathDelay)
	//	{
	//		for (Enemy* e : emg->getEnermies())
	//		{
	//			e->setDestroyedFlag(true);
	//			e->setHp(0);
	//		}
	//		for (Item* i : items)
	//		{
	//			i->setEffectFlag(true);
	//		}

	//		if (gameClear == false)
	//		{
	//			gameClear = true;
	//			gameClearTime = current;
	//			spaceCraft->setSpeed(0.025);
	//			spaceCraft->setRotSpeed(0.5);
	//		}
	//	}

	//	//엔딩
	//	if (gameClear == true && (current - gameClearTime) >= gameClearDelay)
	//	{
	//		Vector dir;
	//		GLfloat roll = spaceCraft->getRotate().getRoll();
	//		GLfloat speed = spaceCraft->getSpeed();
	//		spaceCraft->setCollision(false);
	//		if (spaceCraft->getPosition().getX() < -0.25) // 우측
	//		{
	//			dir.setX(1); // 좌로 이동
	//			spaceCraft->getPosition() += dir * speed;
	//			roll -= spaceCraft->getRotSpeed();
	//			if (roll <= -spaceCraft->getMaxRot())
	//				roll = -spaceCraft->getMaxRot();
	//			spaceCraft->getRotate().setRoll(roll);
	//		}
	//		else if (spaceCraft->getPosition().getX() > 0.25) // 좌측
	//		{
	//			dir.setX(-1); // 우로 이동
	//			spaceCraft->getPosition() += dir * speed;
	//			roll += spaceCraft->getRotSpeed();
	//			if (roll >= spaceCraft->getMaxRot())
	//				roll = spaceCraft->getMaxRot();
	//			spaceCraft->getRotate().setRoll(roll);
	//		}
	//		else // 중앙
	//		{
	//			if (endingFlag == false)
	//			{
	//				spaceCraft->setSpeed(0);
	//				spaceCraft->setAcceleration(0.001);
	//				endingFlag = true;
	//			}
	//			dir.setZ(1);
	//			if (roll <= 0)
	//				roll += spaceCraft->getRotSpeed();
	//			else if (roll >= 0)
	//				roll -= spaceCraft->getRotSpeed();
	//			spaceCraft->getRotate().setRoll(roll);

	//			speed += spaceCraft->getAcceleration();
	//			spaceCraft->setSpeed(speed);
	//			spaceCraft->getPosition() += dir * speed;
	//		}
	//	}
	//}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	srand((unsigned)time(NULL));

	//처음 초기화
	if (initFlag == 0)
	{
		//게임 초기화
		InitStarPosition();
		start = clock();
		spaceCraft = new Character();
		emg = new EnemyManager();

		//텍스쳐
		image[0] = cv::imread("./texture/space.jpg", cv::IMREAD_COLOR);
		cvtColor(image[0], image[0], cv::COLOR_RGB2BGR);

		//오브젝트파일
		string filepath = "./obj/hpbox.obj";
		ifstream fin(filepath);
		healBox.loadObj(fin);
		fin.close();

		string filepath2 = "./obj/powbox.obj";
		ifstream fin2(filepath2);
		powBox.loadObj(fin2);
		fin2.close();

		string filepath3 = "./obj/weaponbox.obj";
		ifstream fin3(filepath3);
		weaponBox.loadObj(fin3);
		fin3.close();

		string filepath4 = "./obj/character.obj";
		ifstream fin4(filepath4);
		character.loadObj(fin4);
		fin4.close();

		string filepath5 = "./obj/satellite.obj";
		ifstream fin5(filepath5);
		satellite.loadObj(fin5);
		fin5.close();

		string filepath6 = "./obj/common.obj";
		ifstream fin6(filepath6);
		common.loadObj(fin6);
		fin6.close();

		string filepath7 = "./obj/turret.obj";
		ifstream fin7(filepath7);
		turretHead.loadObj(fin7);
		fin7.close();

		string filepath8 = "./obj/turret2.obj";
		ifstream fin8(filepath8);
		turretBody.loadObj(fin8);
		fin8.close();

		initFlag++;
	}

	//glut 함수
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Final Project");
	glClearColor(0.015, 0.015, 0.05, 1.0);
	InitLight();
	init_texture();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIgnoreKeyRepeat(1);
	//glutIdleFunc(idle);
	glutSpecialFunc(keyboardSDown);
	glutSpecialUpFunc(keyboardSUp);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutTimerFunc(1000 / 60, timer, 1);

	glutMainLoop();

	//동적할당 해제
	delete spaceCraft;
	delete emg;

	//함수 종료
	return 0;
}