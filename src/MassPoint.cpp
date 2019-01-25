#include "MassPoint.h"
#include <iostream>

MassPoint::MassPoint(double _mass, Vector3d _position, bool _is_clamped)
{
	initialize(_mass, _position, _is_clamped);
}

void MassPoint::initialize(double _mass, Vector3d _position, bool _is_clamped)
{
	mass = _mass;
	position = _position;
	is_clamped = _is_clamped;
	velocity = Vector3d::Zero();
	acceleration = Vector3d::Zero();
	connected_springs.reserve(8);
}

MassPoint::~MassPoint(){}

Vector3d MassPoint::getForce()
{
	Vector3d stretch_force = Vector3d::Zero();
	Vector3d gravity = Vector3d(0, 0, -9.8)*mass;
	for (unsigned int i = 0; i < connected_springs.size(); i++)
	{
		stretch_force += connected_springs[i]->getForce(this);
	}

	Vector3d force = stretch_force+gravity;
	return force;
}

void MassPoint::addSpring(Spring* spring)
{
	connected_springs.push_back(spring);
}

void MassPoint::draw()
{
	if (is_clamped) glColor3d(1.0, 0.0, 0.0);
	else glColor3d(1.0, 0.0, 0.0);
	glPointSize(2.0);
	glBegin(GL_POINTS);
	glVertex3d(position(0), position(1), position(2));
	glEnd();
}

