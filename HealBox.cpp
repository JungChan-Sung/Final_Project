#include "HealBox.h"

HealBox::HealBox()
{
	this->type = 1;
}

HealBox::~HealBox()
{

}

void HealBox::effectHeal(Unit* chr)
{
	if (this->checkCollision(chr))
	{
		this->effectFlag = true;
		chr->incHp(1);
		if (chr->getHp() >= chr->getMaxHp())
			chr->setHp(chr->getMaxHp());
	}
}

void HealBox::drawHealBox()
{
	if (this->viewFlag == true)
	{
		glPushMatrix();
			glTranslatef(this->position.getX(), 0, this->position.getZ());
			glPushMatrix();
				//모델링
				glColor3f(1, 1, 1);
				glScalef(0.25, 0.25, 0.25);
				healBox.draw(20);
				//glColor3f(0.8, 0.8, 0.8);
				//glutSolidCube(0.1);
			glPopMatrix();
		glPopMatrix();
	}

	//히트박스
	drawHitBox();
}

void HealBox::draw(CModel m)
{
	GLfloat x, y, z, nx, ny, nz, tx, ty;
	int v_id, vt_id, vn_id;
	int idx;

	for (int o = 0; o < m.objs.size(); o++)
	{
		if (o == 0)
			glColor3f(1, 1, 1);
		else if (o == 1)
			glColor3f(1, 0, 0);
		else if (o == 2)
			glColor3f(0.5, 0.5, 0.5);

		int nFaces = m.objs[o].f.size();
		for (int k = 0; k < nFaces; k++)
		{
			int nPoints = m.objs[o].f[k].v_pairs.size();
			glBegin(GL_POLYGON);
			for (int i = 0; i < nPoints; i++)
			{
				v_id = m.objs[o].f[k].v_pairs[i].d[0];
				vt_id = m.objs[o].f[k].v_pairs[i].d[1];
				vn_id = m.objs[o].f[k].v_pairs[i].d[2];

				x = m.objs[o].v[v_id - 1].d[0];
				y = m.objs[o].v[v_id - 1].d[1];
				z = m.objs[o].v[v_id - 1].d[2];

				nx = m.objs[o].vn[vn_id - 1].d[0];
				ny = m.objs[o].vn[vn_id - 1].d[1];
				nz = m.objs[o].vn[vn_id - 1].d[2];

				tx = m.objs[o].vt[vt_id - 1].d[0];
				ty = m.objs[o].vt[vt_id - 1].d[1];

				glNormal3f(nx, ny, nz);
				glTexCoord2f(tx, ty);
				glVertex3f(x, y, z);
			}
			glEnd();
		}
	}
}