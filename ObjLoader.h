#pragma once

#include "Game.h"

using namespace std;

class CPoint2f
{
public:
	vector <float> d{ 0, 0 };
};

class CPoint3f
{
public:
	vector <float> d{ 0, 0, 0 };
};

class CPoint3i
{
public:
	vector <int> d{ 0, 0, 0 };
};

class CFace
{
public:
	vector <CPoint3i> v_pairs;
};

class CObj
{
public:
	string name;
	vector <CPoint3f> v;
	vector <CPoint2f> vt;
	vector <CPoint3f> vn;
	vector <CFace> f;
};

class CModel
{
public:
	vector <CObj> objs;

	vector<float> my_strtok_f(char* str, char* delimeter)
	{
		vector <float> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);

		while (tok != NULL)
		{
			v.push_back(atof(tok));
			tok = strtok_s(NULL, delimeter, &context);
		}
		return v;
	}

	vector<string> my_strtok_s(char* str, char* delimeter)
	{
		vector <string> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);

		while (tok != NULL)
		{
			v.push_back(tok);
			tok = strtok_s(NULL, delimeter, &context);
		}
		return v;
	}

	vector<int> my_strtok_i(char* str, char* delimeter)
	{
		vector <int> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);

		while (tok != NULL)
		{
			v.push_back(atoi(tok));
			tok = strtok_s(NULL, delimeter, &context);
		}
		return v;
	}

	void loadObj(ifstream& fin)
	{
		string line;
		CObj obj_tmp;
		int cnt = 0;
		//이전 obj까지의 누적 벡터수
		int cnt_prev_vertex = 0;
		int cnt_prev_texture = 0;
		int cnt_prev_normal = 0;

		while (getline(fin, line))
		{
			int len = line.length();
			vector<float> vf;
			vector<string> s;
			vector<int> vi;
			CPoint3f p3;
			CPoint2f p2;
			CPoint3i p3i;

			if (line[0] == 'o' && line[1] == ' ')
			{
				obj_tmp.name = line.substr(2, len - 2);
				objs.push_back(obj_tmp);
				if (cnt > 0)
				{
					cnt_prev_vertex += objs[cnt - 1].v.size();
					cnt_prev_texture += objs[cnt - 1].vt.size();
					cnt_prev_normal += objs[cnt - 1].vn.size();
				}
				cnt += 1;
			}

			if (line[0] == 'v' && line[1] == ' ')
			{
				vf = my_strtok_f((char*)line.substr(2, len - 2).c_str(), (char*)" ");
				p3.d = { vf[0], vf[1], vf[2] };
				objs[cnt - 1].v.push_back(p3);
			}
			else if (line[0] == 'v' && line[1] == 't')
			{
				vf = my_strtok_f((char*)line.substr(3, len - 3).c_str(), (char*)" ");
				p2.d = { vf[0], vf[1] };
				objs[cnt - 1].vt.push_back(p2);
			}
			else if (line[0] == 'v' && line[1] == 'n')
			{
				vf = my_strtok_f((char*)line.substr(3, len - 3).c_str(), (char*)" ");
				p3.d = { vf[0], vf[1], vf[2] };
				objs[cnt - 1].vn.push_back(p3);
			}
			else if (line[0] == 'f' && line[1] == ' ')
			{
				s = my_strtok_s((char*)line.substr(2, len - 2).c_str(), (char*)" ");
				int nVertexes = s.size();
				CFace face_tmp;
				for (int i = 0; i < nVertexes; i++)
				{
					vi = my_strtok_i((char*)s[i].c_str(), (char*)"/");
					p3i.d = { vi[0] - cnt_prev_vertex, vi[1] - cnt_prev_texture, vi[2] - cnt_prev_normal };
					face_tmp.v_pairs.push_back(p3i);
				}
				objs[cnt - 1].f.push_back(face_tmp);
			}
		}
	}

	void draw(int modelCode)
	{
		GLfloat x, y, z, nx, ny, nz, tx, ty;
		int v_id, vt_id, vn_id;
		int idx;

		for (int o = 0; o < this->objs.size(); o++)
		{
			switch (modelCode) // 0 ~ 캐릭터, 10 ~ 적, 20 ~ 아이템
			{
			case 0: // 캐릭터
				switch (o)
				{
				case 0:
					glColor3f(1, 1, 1);
					break;
				case 1:
					glColor3f(0.05, 0.05, 0.05);
					break;
				case 2:
					glColor3f(0.55, 0.55, 0.55);
					break;
				case 3:
					glColor3f(0.15, 0.15, 0.15);
					break;
				case 4:
					glColor3f(0.35, 0.35, 0.35);
					break;
				}
				break;
			case 1: // 위성
				switch (o)
				{
				case 0:
					glColor3f(0.55, 0.55, 0.55);
					break;
				case 1:
					glColor3f(1, 1, 1);
					break;
				case 2:
					glColor3f(0.35, 0.35, 0.35);
					break;
				}
				break;
			case 10: // 일반 적
				switch (o)
				{
				case 0:
					glColor3f(0.5, 0.5, 0.5);
					break;
				case 1:
					glColor3f(0.31, 0.73, 0.87);
					break;
				}
				break;
			case 11: // 터렛 머리
				switch (o)
				{
				case 0:
					glColor3f(0.65, 0.65, 0.65);
					break;
				case 1:
					glColor3f(0.15, 0.15, 0.15);
					break;
				case 2:
					glColor3f(0.45, 0.45, 0.45);
					break;
				case 3:
					glColor3f(0.25, 0.25, 0.25);
					break;
				case 4:
					glColor3f(1, 1, 1);
					break;
				}
				break;
			case 12: // 터렛 몸통
				glColor3f(0.12, 0.12, 0.12);
				break;
			case 20: // 체력박스
				switch (o)
				{
				case 0:
					glColor3f(1, 1, 1);
					break;
				case 1:
					glColor3f(1, 0, 0);
					break;
				case 2:
					glColor3f(0.5, 0.5, 0.5);
					break;
				}
				break;
			case 21: // 파워박스
				switch (o)
				{
				case 0:
					glColor3f(1, 1, 1);
					break;
				case 1:
					glColor3f(0.5, 0.5, 0.5);
					break;
				case 2:
					glColor3f(0, 0, 1);
					break;
				}
				break;
			case 22: // 무기박스
				switch (o)
				{
				case 0:
					glColor3f(1, 1, 1);
					break;
				case 1:
					glColor3f(0.5, 0.5, 0.5);
					break;
				case 2:
					glColor3f(0, 0, 0);
					break;
				}
				break;
			default:
				glColor3f(1, 1, 1);
				break;
			}

			int nFaces = this->objs[o].f.size();
			for (int k = 0; k < nFaces; k++)
			{
				int nPoints = this->objs[o].f[k].v_pairs.size();
				glBegin(GL_POLYGON);
				for (int i = 0; i < nPoints; i++)
				{
					v_id = this->objs[o].f[k].v_pairs[i].d[0];
					vn_id = this->objs[o].f[k].v_pairs[i].d[2];

					x = this->objs[o].v[v_id - 1].d[0];
					y = this->objs[o].v[v_id - 1].d[1];
					z = this->objs[o].v[v_id - 1].d[2];

					nx = this->objs[o].vn[vn_id - 1].d[0];
					ny = this->objs[o].vn[vn_id - 1].d[1];
					nz = this->objs[o].vn[vn_id - 1].d[2];

					glNormal3f(nx, ny, nz);
					glVertex3f(x, y, z);
				}
				glEnd();
			}
		}
	}
};

extern CModel character;
extern CModel satellite;
//extern CModel plusWeapon;
extern CModel common;
extern CModel turretHead;
extern CModel turretBody;
//extern CModel boss;
extern CModel healBox;
extern CModel powBox;
extern CModel weaponBox;