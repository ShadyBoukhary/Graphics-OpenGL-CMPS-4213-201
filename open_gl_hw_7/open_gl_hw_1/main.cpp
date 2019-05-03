// Shady Boukhary
// Intro to Graphics
// Dr. Simpson
// April 1sth, 2019
//
// Renders 2 cans on a table. Uses textures.


#include "Angel.h"
#include "Model.h"
#include <time.h>
#include <stack>

GLuint program;
// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
double zoom = 15;
double xEye = 0;
int      Axis = Zaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
const int NUM_MODELS = 7;
Model model = Model("./models/can");
Model table = Model("./models/table");


int currentModel = 0;
GLint modelViewLoc;
GLint projectionLoc;

// initialize OpenGL
void init();

// OpenGL display callback
void display();

// ModelView rotaion matrix
mat4 rotationMatrix();

// OpenGL reshape callback
void reshape(int, int);

// OpenGL keyboard callback
void keyboard(unsigned char, int, int);

// OpenGL mouse callback
void mouse(int, int, int, int);

// OpenGL idle callback
void idle();


// OpenGL initialization
void init()
{
	// Load shaders and use the resulting shader program
	program = InitShader("vshader_a6.glsl", "fshader_a6.glsl");
	glUseProgram(program);
	model.Load(program);
	table.Load(program);
	
	// get modelView and projection locations
	modelViewLoc = glGetUniformLocation(program, "ModelView");
	projectionLoc = glGetUniformLocation(program, "Projection");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

// Display Callback
void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	stack<mat4> mvs;
	

	// look at origin
	vec4 at(0.0, 0.0, 0.0, 1.0);
	// camera position 2 units in the positive z
	vec4 eye(0, 0, zoom, 1.0);
	// up vector
	vec4 up(0.0, 1.0, 0.0, 0.0);

	// modalView matrix for the sun
	mat4 modelView = LookAt(eye, at, up) * rotationMatrix();
	mvs.push(modelView);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, modelView * RotateX(-90) * Scale(3,3,3));
	table.Draw(program);

	modelView = modelView * Translate(-2, 1, 0) * Scale(0.5, 0.5, 0.5);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, modelView * RotateX(-90) * RotateZ(Theta[Zaxis]));
	model.Draw(program);

	modelView = modelView * Translate(vec3(3, -0.75 ,0)) * RotateY(60);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, modelView);
	model.Draw(program);


	// Calculate frame rate
	static int frame, fps, time, timebase = 0;
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	char display_string[100];
	if (time - timebase > 1000)
	{
		fps = frame * 1000.0 / (time - timebase);
		sprintf(display_string, "Shady's rotating models : FPS:%d ", fps);
		glutSetWindowTitle(display_string);
		timebase = time;
		frame = 0;
	}
	glutSwapBuffers();
}

mat4 rotationMatrix()
{
	return RotateX(Theta[Zaxis]);
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat aspect = GLfloat(width) / height;
	mat4  projection = Perspective(45.0, aspect, 0.1, 150);

	glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, projection);
}

// Keyboard Callback
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

// Mouse Callback
void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		//case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
		//case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
		//case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		case 3:					  zoom++;		 break;
		case 4:					  zoom--;		 break;
		}
	} 

}

// Idle Callback
void idle()
{
	Theta[Axis] += 0.01;

	if (Theta[Axis] > 360.0) {
		Theta[Axis] -= 360.0;
	}

	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	glewExperimental = GL_TRUE;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Color Cube");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
