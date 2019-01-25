#ifndef _MASSSPRINGSYSTEM_H_
#define _MASSSPRINGSYSTEM_H_

#include "Spring.h"
#include "MassPoint.h"
#include <dlib/matrix.h>
#include <dlib/optimization.h>

typedef dlib::matrix<double, 0, 1> col_vector;

class MassSpringSystem
{
public:
	MassSpringSystem();
	~MassSpringSystem();
    
	MassPoint* addMassPoint(double mass,Vector3d position,bool is_clamped);
    Spring* addSpring(double stiffness,double rest_length,MassPoint* point1,MassPoint* point2);
	void update(double dt);
	void draw();
	void calculateEnergy();
	void getQuasiStaticGD();
	void getQuasiStaticDlib();
	void updateRK4(double dt);

public:
    vector<MassPoint*> points;
    vector<Spring*> springs;
	int counter = 0;
	double tot_energy = 0;
};

struct Evaluate
{
	Evaluate(MassSpringSystem* _simulator) { simulator = _simulator; }
	MassSpringSystem* simulator;

	double operator() (const col_vector x)const
	{
		int j = 0;
		for (int i = 0; i < simulator->points.size(); i++)
		{
			if (simulator->points[i]->is_clamped) continue;
			simulator->points[i]->position = Vector3d(x(j), x(j + 1), x(j + 2));
			j += 3;
		}
		simulator->calculateEnergy();
		return simulator->tot_energy;
	}
};

struct EvaluateGradient
{
	EvaluateGradient(MassSpringSystem* _simulator) { simulator = _simulator; }
	MassSpringSystem* simulator;

	col_vector operator() (const col_vector x)const
	{
		int j = 0;
		for (int i = 0; i < simulator->points.size(); i++)
		{
			if (simulator->points[i]->is_clamped) continue;
			simulator->points[i]->position = Vector3d(x(j), x(j + 1), x(j + 2));
			j += 3;
		}
		col_vector g;
		g.set_size(x.size());
		
		j = 0;
		for (int i = 0; i < simulator->points.size(); i++)
		{
			if (simulator->points[i]->is_clamped) continue;
			Vector3d force=simulator->points[i]->getForce();
			g(j) = -force(0);
			g(j + 1) = -force(1);
			g(j + 2) = -force(2);
			j += 3;
		}
		return g;
	}
};

#endif