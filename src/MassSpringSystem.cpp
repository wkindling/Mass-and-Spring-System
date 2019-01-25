#include "MassSpringSystem.h"
#include <iostream>

MassSpringSystem::MassSpringSystem(){}

MassSpringSystem::~MassSpringSystem(){}

MassPoint* MassSpringSystem::addMassPoint(double mass, Vector3d position,bool is_clamped)
{
	MassPoint* point = new MassPoint(mass, position, is_clamped);
	points.push_back(point);
	return point;
}

Spring* MassSpringSystem::addSpring(double stiffness, double rest_length, MassPoint* point1, MassPoint* point2)
{
	Spring* spring = new Spring(stiffness,rest_length,point1,point2);
	point1->addSpring(spring);
	point2->addSpring(spring);
	springs.push_back(spring);
	return spring;
}

void MassSpringSystem::updateRK4(double dt)
{
	for (unsigned int i = 0; i < points.size(); i++)
	{
	if (points[i]->is_clamped) continue;
	points[i]->v1 = points[i]->velocity;
	points[i]->a1 = points[i]->getForce() / points[i]->mass;
	points[i]->v2 = points[i]->v1 + dt / 2.0*points[i]->a1;
	points[i]->xt = points[i]->position;
	}
	for (unsigned int i = 0; i < points.size(); i++)
	{
	if (points[i]->is_clamped) continue;
	points[i]->velocity = points[i]->v1+ dt / 2.0*points[i]->a1;
	points[i]->position = points[i]->xt+ dt / 2.0*points[i]->v1;
	}
	for (unsigned int i = 0; i < points.size(); i++)
	{
	if (points[i]->is_clamped) continue;
	points[i]->a2 = points[i]->getForce() / points[i]->mass;
	points[i]->v3 = points[i]->v1 + dt / 2.0*points[i]->a2;
	}
	for (unsigned int i = 0; i < points.size(); i++)
	{
	if (points[i]->is_clamped) continue;
	points[i]->velocity = points[i]->v1+ dt / 2.0*points[i]->a2;
	points[i]->position = points[i]->xt+ dt / 2.0*points[i]->v2;
	}
	for (unsigned int i = 0; i < points.size(); i++)
	{
	if (points[i]->is_clamped) continue;
	points[i]->a3 = points[i]->getForce() / points[i]->mass;
	points[i]->v4 = points[i]->v1 + dt * points[i]->a3;
	}
	for (unsigned int i = 0; i < points.size(); i++)
	{
	if (points[i]->is_clamped) continue;
	points[i]->velocity = points[i]->v1 + dt * points[i]->a3;
	points[i]->position = points[i]->xt + points[i]->v3*dt;
	}
	for (unsigned int i = 0; i < points.size(); i++)
	{
	if (points[i]->is_clamped) continue;
	points[i]->a4 = points[i]->getForce() / points[i]->mass;
	}
	for (unsigned int i = 0; i < points.size(); i++)
	{
	if (points[i]->is_clamped) continue;
	points[i]->velocity = points[i]->v1 + dt / 6.0*(points[i]->a1 + 2 * points[i]->a2 + 2 * points[i]->a3 + points[i]->a4);
	points[i]->position = points[i]->xt + dt / 6.0*(points[i]->v1 + 2 * points[i]->v2 + 2 * points[i]->v3 + points[i]->v4);
	}
	counter++;
}

void MassSpringSystem::update(double dt)
{	
	for (unsigned int i = 0; i < points.size(); i++)
	{
		if (points[i]->is_clamped) continue;
		points[i]->acceleration= points[i]->getForce() / points[i]->mass;
		points[i]->velocity += points[i]->acceleration * dt;
	}

	for (unsigned int i = 0; i < points.size(); i++)
	{
		if (points[i]->is_clamped) continue;
		points[i]->position += points[i]->velocity * dt;
	}
	counter++;
}

void MassSpringSystem::draw()
{
	for (unsigned int i = 0; i < springs.size(); i++)
	{
		springs[i]->draw();
	}	
}

void MassSpringSystem::calculateEnergy()
{
	tot_energy = 0;
	for (unsigned int i = 0; i < springs.size(); i++)
	{
		Vector3d dir = springs[i]->point1->position - springs[i]->point2->position;
		double distance = dir.norm();
		double temp = 0.5 * springs[i]->stiffness*(distance - springs[i]->rest_length)*(distance - springs[i]->rest_length);
		tot_energy += temp;
	}
	for (unsigned int i = 0; i < points.size(); i++)
	{
		tot_energy += points[i]->mass*(9.8)*(points[i]->position(2));
	}
	std::cout << tot_energy <<"  "<<counter <<std::endl;
}


void MassSpringSystem::getQuasiStaticGD()
{
	double old_energy;
	calculateEnergy();

	do
	{
		old_energy = tot_energy;

		for (int i = 0; i < points.size(); i++)
		{
			if (points[i]->is_clamped) continue;
			points[i]->position = points[i]->position + points[i]->getForce()*0.0015;//ÌÝ¶ÈÏÂ½µ
		}

		calculateEnergy();
		counter++;

	} while (abs(tot_energy - old_energy) > 0.005);
	
}

void MassSpringSystem::getQuasiStaticDlib()
{
	Evaluate _evaluate(this);
	EvaluateGradient _evaluate_gradient(this);

	int clamped_count = 0;
	for (unsigned int i = 0; i < points.size(); i++)
	{
		if (points[i]->is_clamped) clamped_count++;
	}
	col_vector x((points.size() - clamped_count) * 3);
	int j = 0;
	for (unsigned int i = 0; i < points.size(); i++)
	{
		if (points[i]->is_clamped) continue;
		x(j) = points[i]->position(0);
		x(j+1) = points[i]->position(1);
		x(j+2) = points[i]->position(2);
		j += 3;
	}

	double min_energy = dlib::find_min(dlib::bfgs_search_strategy(),
		dlib::objective_delta_stop_strategy(0.01, 1000),
		_evaluate,
		_evaluate_gradient,
		x, 
		0.0);

	j = 0;
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i]->is_clamped) continue;
		points[i]->position = Vector3d(x(j), x(j + 1), x(j + 2));
		j += 3;
	}
}