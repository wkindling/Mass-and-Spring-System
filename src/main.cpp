#include "render.h"
#include "interactive.h"
#include <iostream>

class A
{
	int a;
};


int main(int argc, const char* argv[])
{
	MassPoint* Grid[20][20];
	double z = 10.0;
	double x = 0.0;

	int width=15;
	int height=15;

	for (int i = 0; i < width; i++)
	{
		double y = 0.0;
		for (int j = 0; j < height; j++)
		{
			Grid[i][j]=simulator.addMassPoint(0.15, Vector3d(x, y, z), false);
			y += 0.1;
		}
		x += 0.1;
	}
	//Grid[0][0]->is_clamped = true;
	//for (int j = 0; j < height; j++) Grid[15][j]->is_clamped = true;
	//for (int j = 0; j < height; j++) Grid[5][j]->is_clamped = true;
	//for (int j = 0; j < height; j++) Grid[0][j]->is_clamped = true;
	//for (int j = 0; j < height; j++) Grid[width-1][j]->is_clamped = true;
	//for (int j = 0; j < height; j++) Grid[j][0]->is_clamped = true;
	//for (int j = 0; j < height; j++) Grid[j][height-1]->is_clamped = true;
	for (int i=4;i<11;i++)
		for (int j = 4; j < 11; j++)
		{
			Grid[i][j]->is_clamped = true;
		}
	
	double stiffness = 250.0;
	double rest_length = 0.1;
	double long_rest_length = 0.1*1.414;

	for (int i=0;i<width-1;i++)
		for (int j = 0; j < height-1; j++)
		{
			MassPoint* p = Grid[i][j];
			MassPoint* right = Grid[i][j + 1];
			MassPoint* forward = Grid[i + 1][j];
			MassPoint* slope = Grid[i + 1][j + 1];
			Spring* s1 = simulator.addSpring(stiffness, rest_length, p, right);
			Spring* s2 = simulator.addSpring(stiffness, rest_length, p, forward);
			Spring* s3 = simulator.addSpring(stiffness, long_rest_length, p, slope);
			Spring* s4 = simulator.addSpring(stiffness, long_rest_length, right, forward);
		}
	for (int j = 0; j < height - 1; j++)
	{
		int i = width - 1;
		MassPoint* p = Grid[i][j];
		MassPoint* p1 = Grid[i][j+1];
		Spring* s = simulator.addSpring(stiffness, rest_length, p, p1);
	}
	for (int i = 0; i < width - 1; i++)
	{
		int j = height - 1;
		MassPoint*p = Grid[i][j];
		MassPoint*p1 = Grid[i+1][j];
		Spring* s = simulator.addSpring(stiffness, rest_length, p, p1);
	}

	//simulator.getQuasiStaticDlib();
	//simulator.getQuasiStaticGD();
	//cout << "Size of A" << sizeof(A) << std::endl;
	glutInit(&argc, (char**)argv);
	init();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(mykeyboard);
	glutMouseFunc(mymouse);
	glutMotionFunc(mouseMove);
	glutDisplayFunc(display);
	glutMainLoop();	
	return 0;

}