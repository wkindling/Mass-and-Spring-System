#ifndef _SPRING_H_
#define _SPRING_H_

#include <Eigen/Dense>
#include "MassPoint.h"

using namespace Eigen;

class MassPoint;

class Spring
{
public:
    Spring(double _stiffness,double _rest_length,MassPoint* _point1,MassPoint* _point2);
    ~Spring();

    Vector3d getForce(MassPoint* orient);
	void draw();
	void initialize(double _stiffness, double _rest_length, MassPoint* _point1, MassPoint* _point2);

public:
    double stiffness;
    double rest_length;

    MassPoint* point1;
    MassPoint* point2;
};

#endif