// Shady Boukhary
// Intro to Graphics
// Dr. Simpson
// February 25th, 2019
//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "Angel.h"
#include "Cube.h"

Cube cube;

// OpenGL initialization
void init()
{
	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader_a4.glsl", "fshader_a4.glsl");
	glUseProgram(program);
	cube.load(program);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

// Display Callback
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cube.draw();

	// Timing etc
	static int frame, fps, time, timebase = 0;
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	char display_string[100];
	if (time - timebase > 1000)
	{
		fps = frame * 1000.0 / (time - timebase);
		sprintf(display_string, "Shady’s rotating cube : FPS:%d ", fps);
		glutSetWindowTitle(display_string);
		timebase = time;
		frame = 0;
	}
	glutSwapBuffers();
}

// Keyboard Callback
void keyboard(unsigned char key, int x, int y)
{
	cube.keyboard(key, x, y);
}

// Mouse Callback
void mouse(int button, int state, int x, int y)
{
	cube.mouse(button, state, x, y);
}

// Idle Callback
void idle()
{
	cube.idle();
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
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
