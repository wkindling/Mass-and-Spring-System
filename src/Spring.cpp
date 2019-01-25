#include "Spring.h"
#include <cassert>
#include <cmath>
#include <iostream>

Spring::Spring(double _stiffness, double _rest_length, 
			   MassPoint* _point1, MassPoint* _point2)
{
	initialize(_stiffness, _rest_length, _point1, _point2);
}

void Spring::initialize(double _stiffness, double _rest_length, MassPoint* _point1, MassPoint* _point2)
{
	stiffness = _stiffness;
	rest_length = _rest_length;
	point1 = _point1;
	point2 = _point2;
}

Spring::~Spring(){}

Vector3d Spring::getForce(MassPoint* orient)
{

	assert(orient == point1 || orient == point2);
	Vector3d force;

	Vector3d dir = point2->position - point1->position;
	double distance = dir.norm();
	double scalar = stiffness * (distance - rest_length);
	dir.normalize();

	double d1 = point1->velocity.dot(dir);
	double d2 = point2->velocity.dot(dir);
	double damp = -0.4*(d1 + d2);

	if (orient == point1) return (scalar + damp)*dir;
	else return (-scalar + damp)*dir;
}

void Spring::draw()
{
	if (point1->is_clamped&&point2->is_clamped) glColor3d(1, 0, 0);
	else glColor3d(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3d(point1->position(0), point1->position(1), point1->position(2));
	glVertex3d(point2->position(0), point2->position(1), point2->position(2));
	glEnd();
}