#pragma once

#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <time.h>
#include <ctime>
#include <stdio.h>
#include <stdbool.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <list>
#include <thread>

#define _WINDOW_WIDTH 1200
#define _WINDOW_HEIGHT 800
#define MAX_X 6.0
#define MAX_Z 4.5
#define MIN_X -6.0
#define MIN_Z -4.5
#define PI 3.14159
#define TEXTURE_COUNT 1
#define STAR_COUNT 300

extern bool keyStates[256];
extern int initFlag;
extern int hitBoxView;
extern int healthBarView;
extern int subWeaponView;
extern int current;

extern cv::Mat image[TEXTURE_COUNT];
extern GLuint tex_ids[TEXTURE_COUNT];
extern int nTex;