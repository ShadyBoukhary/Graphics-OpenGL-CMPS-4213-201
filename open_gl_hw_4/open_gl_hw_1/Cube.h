#pragma once
#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

class Cube
{
private:
	static const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
	int Index;
	point4 points[NumVertices];
	color4 colors[NumVertices];
public:
	Cube();
	void quad(int, int, int, int);
	void colorcube();
	void load(GLuint&);
	void keyboard(unsigned char key, int x, int y);
	void mouse(int button, int state, int x, int y);
	void idle();
	void draw();
	~Cube();
};

