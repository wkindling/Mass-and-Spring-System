#ifndef _MASSPOINT_H_
#define _MASSPOINT_H_

#include <vector>
#include <Eigen/Dense>
#include "Spring.h"
#include <gl/glut.h>

using namespace Eigen;
using namespace std;

class Spring;

class MassPoint
{
public:
    MassPoint(double _mass,Vector3d _position,bool _is_clamped);
    ~MassPoint();

	void initialize(double _mass, Vector3d _position, bool _is_clamped);
    void draw();
    void addSpring(Spring* spring);
    Vector3d getForce();

public:
    double mass;
    Vector3d position;
    Vector3d velocity;
    Vector3d acceleration;
    bool is_clamped;
    vector<Spring*> connected_springs;

	Vector3d v1, v2, v3, v4;
	Vector3d a1, a2, a3, a4;
	Vector3d xt;
};

#endif