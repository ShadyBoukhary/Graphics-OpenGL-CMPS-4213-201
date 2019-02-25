#include "Cube.h"

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

GLuint  theta;  // The location of the "theta" shader uniform variable

// generate 12 triangles: 36 vertices and 36 colors
void Cube::colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

Cube::Cube()
{
	Index = 0;
	colorcube();
}

void Cube::quad(int a, int b, int c, int d)
{
	static const point4 vertices[8] = {
		point4(-0.5, -0.5,  0.5, 1.0),
		point4(-0.5,  0.5,  0.5, 1.0),
		point4(0.5,  0.5,  0.5, 1.0),
		point4(0.5, -0.5,  0.5, 1.0),
		point4(-0.5, -0.5, -0.5, 1.0),
		point4(-0.5,  0.5, -0.5, 1.0),
		point4(0.5,  0.5, -0.5, 1.0),
		point4(0.5, -0.5, -0.5, 1.0)
	};

	// RGBA olors
	static const color4 vertex_colors[8] = {
		color4(0.0, 0.0, 0.0, 1.0),  // black
		color4(1.0, 0.0, 0.0, 1.0),  // red
		color4(1.0, 1.0, 0.0, 1.0),  // yellow
		color4(0.0, 1.0, 0.0, 1.0),  // green
		color4(0.0, 0.0, 1.0, 1.0),  // blue
		color4(1.0, 0.0, 1.0, 1.0),  // magenta
		color4(1.0, 1.0, 1.0, 1.0),  // white
		color4(0.0, 1.0, 1.0, 1.0)   // cyan
	};
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

// Load into GPU
void Cube::load(GLuint& program)
{
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	theta = glGetUniformLocation(program, "theta");
}

// Draw to display
void Cube::draw()
{
	glUniform3fv(theta, 1, Theta);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

//----------------------------------------------------------------------------

void Cube::keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

void Cube::mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
		case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
		case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		}
	}
}

//----------------------------------------------------------------------------

void Cube::idle()
{
	Theta[Axis] += 0.01;

	if (Theta[Axis] > 360.0) {
		Theta[Axis] -= 360.0;
	}

	glutPostRedisplay();
}

Cube::~Cube()
{
}
