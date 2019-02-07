 // Shady Boukhary
// Intro to Graphics
// January 30, 2018
// Dr. Simpson
// OpenGL Spiral

#include "Angel.h"

const int MatrixDims = 33; // change this to add more spirals
const double CellDim = 2.0 / (MatrixDims);
const int NumPoints = MatrixDims * MatrixDims * 4;
const double CenterOffset = 0.5 * CellDim;
//----------------------------------------------------------------------------

void init();
void display();
void keyboard(unsigned char, int, int);
//----------------------------------------------------------------------------


struct Cell {
	bool isVisited;
	bool southWall;
	bool eastWall;
	Cell() : isVisited(false), southWall(false), eastWall(false) {}
};

int main(int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
	glewExperimental = GL_TRUE;
    glutCreateWindow( "Sierpinski Gasket" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}

void init()
{
	Cell cells[MatrixDims][MatrixDims];

	for (int i = 0; i < MatrixDims; i++)
		for (int j = 0; j < MatrixDims; j++) {
			if ((i == 0 || i == MatrixDims - 1) && j != 0) {
				cells[i][j].southWall = true;
				cells[i][j].isVisited = true;
			}
			if ((j == 0 || j == MatrixDims - 1) && i != 0) {
				cells[i][j].eastWall = true;
				cells[i][j].isVisited = true;
			}

			if (i != 0 && j != 0) {
				cells[i][j].southWall = true;
				cells[i][j].eastWall = true;
			}
		}


	vec2 points[NumPoints];
	int currentPoint = 0;

	// start from the top right
	vec2 startingPoint = vec2(-1.0 - CenterOffset, 1.0 + CenterOffset);

	for (int i = 0; i < MatrixDims; i++) {
		for (int j = 0; j < MatrixDims; j++) {
			if (cells[i][j].southWall) {
				vec2 point1 = vec2(startingPoint.x + (CellDim * j), startingPoint.y - (CellDim * (i + 1)));
				vec2 point2 = vec2(startingPoint.x + (CellDim * (j + 1)), startingPoint.y - (CellDim * (i + 1)));
				points[currentPoint] = point1;
				points[currentPoint + 1] = point2;
				currentPoint += 2;
			}
			if (cells[i][j].eastWall) {
				vec2 point1 = vec2(startingPoint.x + (CellDim * (j + 1)), startingPoint.y - (CellDim * (i)));
				vec2 point2 = vec2(startingPoint.x + (CellDim * (j + 1)), startingPoint.y - (CellDim * (i + 1)));
				points[currentPoint] = point1;
				points[currentPoint + 1] = point2;
				currentPoint += 2;
			}
		}
	}

	// compute all points per spiral
	//for (int i = 0; i < NumSpirals; i++) {

	//	// first half of the spiral
	//	points[pointIdx] = vec2(points[pointIdx - 1].x + hop, points[pointIdx - 1].y);
	//	points[pointIdx + 1] = vec2(points[pointIdx].x, points[pointIdx].y - hop);
	//	hop = hop + HopSize;

	//	// second half of the spiral
	//	points[pointIdx + 2] = vec2(points[pointIdx + 1].x - hop, points[pointIdx + 1].y);
	//	points[pointIdx + 3] = vec2(points[pointIdx + 2].x, points[pointIdx + 2].y + hop);

	//	pointIdx += 4;
	//	hop = hop + HopSize;
	//}

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader_a2.glsl", "fshader_a2.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

//----------------------------------------------------------------------------

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);     // clear the window
	glDrawArrays(GL_LINES, 0, NumPoints);    // draw the points
	glFlush();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------
