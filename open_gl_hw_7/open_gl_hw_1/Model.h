#pragma once
#include <string>
#include <iostream>
#include "plyloader.h"
using namespace std;

class Model
{
	string name;
	GLuint tex;
	PLYModel model;
public:
	GLuint vao;
	Model();
	~Model();
	// Parameratized constructor that reads in the model rply file
	Model(string);
	// This method loads the model into OpenGL(ie onto the GPU)
	bool Load(GLuint);
	void Draw(GLuint);
};

