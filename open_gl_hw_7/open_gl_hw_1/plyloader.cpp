//  The following software has been modified by R. P. Simpson to work with ASCII
//  blender files.  It originally read binary file.  The original's use of glm::
// was converted to use Angel:: on all the vec3,vec4 classes.  A new struct called ivec3
// was added to the code since Angel did not contain support for the integer vec.
// This software is a very simple loader for Blender file.  It does not have the 
// general flexibility of other loaders that you may download such as YaPly loader.
// It would be worth your while to make a study of the other loaders in order to 
// learn some c++11 tricks of the trade, if so inclined.
// I also ripped out code that delta with cases that did not include
// color and normals.

/**************************************
* Copyright (c) <2014> <Vishesh Gupta> *
**************************************

The MIT / X Window System License
=================================
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include "plyloader.h"
#pragma comment( lib, "glew32" )

using namespace std;
using namespace Angel;

PLYModel::PLYModel() {
}

struct data
{
	float _x, _y, _z;
    float _nx, _ny, _nz;
	float _s, _t;
    int _r, _g, _b, _a;
	data(){
		 _x= _y=_z=_nx=_ny=_nz=_r=_g=_b=_a=_s=_t=0;
	}
};


struct faceData
{
	int a,b,c;
	faceData()
	{
		a=b=c=0;
	}
};

struct colorData
{
	int r,g,b,a;
	colorData(){
		r=g=b=a=0;
	}
};

// This version will only work with ply files that have
// BOTH normals and colors.  Modified By Richard P. Simpson
PLYModel::PLYModel(const char* filename) {
    
	long nvertices;
	int position;
	struct faceData F;
	isMesh=0;

    string line;
    string s1, s2;

    ifstream inputPly;
	//inputPly.open(filename,ios::binary);
	inputPly.open(filename);
	if (!inputPly.is_open()) {
        cerr << "Couldn't open " << filename << '\n';
        exit(1);
    }
	cout << "Opened " << filename << endl;
	ifColor=0;
	ifNormal=0;

    getline(inputPly, line);
	while(line.compare("vertex")!=0)
	{
		getline(inputPly, line,' ');
		//cout<<line<<"\n";
	}

	inputPly>>vertexCount; // 3rd line
	cout << "Number of verts:" << vertexCount << endl;;
	
	//getline(inputPly,line);
	//bool visit=0;
	while(line.compare("face")!=0)
	{
		if (line.find("s") == 0) iftex = 1;
		if(line.find("red")==0) ifColor=1;
		if(line.find("nx")==0) ifNormal=1;
		getline(inputPly, line,' ');
		//cout<<line<<endl;		
	}

	inputPly>>faceCount;
	cout << "Number of faces:" << faceCount << endl;
	if(faceCount>0)
		isMesh=1;

	while(line.compare("end_header")!=0)
	{
		getline(inputPly, line);
	}
	
	// We will assume that our objects have both COLOR and NORMAL information

	{
		struct data Values;

		if(!ifColor)
		{
			cout<<"\nFile doesn't contain the Color Information, check the constructor call";
			exit(0);
		}
		if(!ifNormal)
		{
			cout<<"\nFile doesn't contain the Normal Information, check the constructor call";
			exit(0);
		}
		if (!iftex)
		{
			cout << "\nFile doesn't contain the Texture Information, check the constructor call";
			exit(0);
		}

		//inputPly.read((char *)&Values,sizeof(Values));
		inputPly >> Values._x >> Values._y >> Values._z;
		inputPly >> Values._nx >> Values._ny >> Values._nz;
		inputPly >> Values._s >> Values._t;
		inputPly >> Values._r >> Values._g >> Values._b;
		//cout<<"\n"<<Values._x <<"\t"<< Values._y <<"\t"<< Values._z <<"\t"<< Values._nx <<"\t"<< Values._ny <<"\t"<< Values._nz <<"\t"<< Values._r <<"\t"<< Values._g <<"\t"<< Values._b <<"\t"<< Values._a;
		min = max = vec3(Values._x, Values._y, Values._z);

		positions.push_back(vec3(Values._x, Values._y, Values._z));
		normals.push_back(vec3(Values._nx, Values._ny, Values._nz));
		uvs.push_back(vec2(Values._s, Values._t));
		colors.push_back(vec3(Values._r, Values._g, Values._b) / 255.0f);

		for (long int i = 1; i < vertexCount; i++) 
		{
			//inputPly.read((char *)&Values,sizeof(Values));
			inputPly >> Values._x >> Values._y >> Values._z;
			inputPly >> Values._nx >> Values._ny >> Values._nz;
			inputPly >> Values._s >> Values._t;
			inputPly >> Values._r >> Values._g >> Values._b;
			//cout<<"\n"<<Values._x <<"\t"<< Values._y <<"\t"<< Values._z <<"\t"<< Values._nx <<"\t"<< Values._ny <<"\t"<< Values._nz <<"\t"<< (int)Values._r <<"\t"<< (int)Values._g <<"\t"<< (int)Values._b <<"\t"<< (int)Values._a;
        
			if (Values._x < min.x) min.x = Values._x;
			if (Values._y < min.y) min.y = Values._y;
			if (Values._z < min.z) min.z = Values._z;

			if (Values._x > max.x) max.x = Values._x;
			if (Values._y > max.y) max.y = Values._y;
			if (Values._z > max.z) max.z = Values._z;

			positions.push_back(vec3(Values._x, Values._y, Values._z));	// -1 is been multiplied to set the correct orientation of this model....
			normals.push_back(vec3(Values._nx, Values._ny, Values._nz));
			uvs.push_back(vec2(Values._s, Values._t));
			colors.push_back(vec3((int)Values._r, (int)Values._g, (int)Values._b) / 255.0f);
		}
		center = (min + max) / 2.0f;

		//Bounding volume measurements
		bvWidth = max.x - min.x;
		bvHeight = max.y - min.y;
		bvDepth = max.z - min.z;
		bvAspectRatio = bvWidth / bvHeight;
	}

	
	if(isMesh)
	{
		unsigned int numEdges;
		for(int i=0;i<faceCount;i++)
		{
			//inputPly.read((char *)&numEdges,sizeof(numEdges));
			inputPly >> numEdges;
			if (numEdges != 3) {//ADDED: RP Simpson
				cerr << "Model is NOT triangulated!" << endl;
				cerr << "Triangulate model in Blender and resave" << endl;
				system("pause");
				exit(1);
			}
			//inputPly.read((char *)&F,sizeof(F));
			inputPly >> F.a >> F.b >> F.c;
			indices.push_back(F.a);
			indices.push_back(F.b);
			indices.push_back(F.c);
			faces.push_back(ivec3(F.a,F.b,F.c));// I really dont need this. Remove later
		}
	}
    inputPly.close();
}


void PLYModel:: FreeMemory()
{
	positions.~vector();
	normals.~vector();
	colors.~vector();
	if(isMesh)
	{
		faces.~vector();
	}

}
