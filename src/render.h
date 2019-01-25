#pragma once
#ifndef _RENDER_H_
#define _RENDER_H_

#include "MassSpringSystem.h"
#include "interactive.h"
#include <iostream>

bool b = false;

MassSpringSystem simulator;

void init();
void reshape(int width, int height);
void display();

void init()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Mass and Spring");
	glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
}

void reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(move_x, move_y, move_z, target_x, target_y, target_z, 0, 0, 1);
	glColor3f(1.0, 1.0, 1.0);
	simulator.draw();
	if (b)
	{
		simulator.update(0.01);
	}
	glutPostRedisplay();
	glutSwapBuffers();
}


#endif