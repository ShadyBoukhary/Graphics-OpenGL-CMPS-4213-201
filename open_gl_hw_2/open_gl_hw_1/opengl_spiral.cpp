// Shady Boukhary
// Intro to Graphics
// January 30, 2018
// Dr. Simpson
// OpenGL Spiral

#include "Angel.h"

const int NumSpirals = 50; // change this to add more spirals
const double HopSize = 1.8 / (NumSpirals * 2);	
const int NumPoints = NumSpirals * 4 + 1;
//----------------------------------------------------------------------------

void init();
void display();
void keyboard(unsigned char, int, int);
//----------------------------------------------------------------------------

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
	vec2 points[NumPoints];
	double hop = HopSize;
	int pointIdx = 1;

	// start from center
	points[0] = vec2(0.0, 0.0);


	// compute all points per spiral
	for (int i = 0; i < NumSpirals; i++) {

		// first half of the spiral
		points[pointIdx] = vec2(points[pointIdx - 1].x + hop, points[pointIdx - 1].y);
		points[pointIdx + 1] = vec2(points[pointIdx].x, points[pointIdx].y - hop);
		hop = hop + HopSize;

		// second half of the spiral
		points[pointIdx + 2] = vec2(points[pointIdx + 1].x - hop, points[pointIdx + 1].y);
		points[pointIdx + 3] = vec2(points[pointIdx + 2].x, points[pointIdx + 2].y + hop);

		pointIdx += 4;
		hop = hop + HopSize;
	}

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
	glDrawArrays(GL_LINE_STRIP, 0, NumPoints);    // draw the points
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
