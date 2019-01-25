#pragma once
#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <gl/glut.h>

double move_x = 2.2, move_y = 1.9, move_z = 11.7;//the position of the camera
double target_x = -0.42, target_y = -0.42, target_z = 7.89;//the position of the target

int ox = 0, oy = 0;
int flag = 0;

extern bool b;

void mykeyboard(unsigned char key, int x, int y);
void mymouse(int button, int state, int x, int y);
void mouseMove(int x, int y);

void mykeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':move_x += 0.1; break;
	case 'w':move_x -= 0.1; break;
	case 'd':move_y += 0.1; break;
	case 'a':move_y -= 0.1; break;
	case 'q':move_z -= 0.1; break;
	case 'e':move_z += 0.1; break;
	case 'b':b = true; break;
	}
	glutPostRedisplay();
}

void mymouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		flag = 1;
		ox = x;
		oy = y;
	}
	if (state == GLUT_UP) flag = 0;
}

void mouseMove(int x, int y)
{
	if (flag)
	{
		target_x += (double)(x - ox)*0.03;
		target_y += (double)(x - ox)*0.03;
		target_z += (double)(y - oy)*0.03;
		glutPostRedisplay();
	}
	ox = x;
	oy = y;
}

#endif