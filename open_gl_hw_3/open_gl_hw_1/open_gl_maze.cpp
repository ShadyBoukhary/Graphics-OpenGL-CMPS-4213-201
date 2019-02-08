 // Shady Boukhary
// Intro to Graphics
// February 8, 2019
// Dr. Simpson
// Generates a random maze in OpenGL

#include "Angel.h"
#include <random>
#include <time.h>

const int MatrixDims = 15;	// size of maze, choose size + 1 since borders don't count
const double CellDim = 2.0 / (MatrixDims);	// size of cell
const int NumPoints = MatrixDims * MatrixDims * 4;	
const double CenterOffset = 0.5 * CellDim;
//----------------------------------------------------------------------------

void init();
void display();
void keyboard(unsigned char, int, int);
//----------------------------------------------------------------------------

/**
	Represents a cell in the maze. Each cell has a south and east wall.
*/
struct Cell {
	bool isVisited;
	bool southWall;
	bool eastWall;
	Cell() : isVisited(false), southWall(true), eastWall(true) {}
};

//----------------------------------------------------------------------------

/**
	Checks whether all neighbors of a cell have been visited

	@param i: row of the cell
	@param j: column of the cell
	@param cells: the 2D array of cells
	@return bool: whether all neighbors are visited
*/
bool allVisited(int i, int j, Cell cells[][MatrixDims]) {
	// whether all neighbors are visited or out of bounds
	return (i + 1 >= MatrixDims || cells[i + 1][j].isVisited) && 
		(i - 1 < 1 || cells[i - 1][j].isVisited) &&
		(j + 1 >= MatrixDims || cells[i][j + 1].isVisited) &&
		(j - 1 < 1 || cells[i][j - 1].isVisited);
}

//----------------------------------------------------------------------------

/**
	Find a random neighbor of the cell

	@param i: row of the cell
	@param j: column of the cell
	@param cells: the 2D array of cells
	@return vec2: coordinates of the chosen neighbor
*/
vec2 findNeighbor(int i, int j, Cell cells[][MatrixDims]) {

	if (!allVisited(i, j, cells)) {
		// while there are some unvisited cells and the random number is not valid, generate another one
		// and pick another cell
		while (true) {

			int num = rand() % 4 + 1;

			// the cell on the bottom of the current cell
			if (num == 1 && i + 1 < MatrixDims && !cells[i + 1][j].isVisited) {
				cells[i][j].southWall = false;	// open south gate of current cell
				return vec2(i + 1, j);
			}
			// the cell on top of the current cell
			else if (num == 2 && i - 1 >= 1 && !cells[i - 1][j].isVisited) {
				cells[i - 1][j].southWall = false;	// open south gate
				return vec2(i - 1, j);
			}
			// the cell on the right of the current cell
			else if (num == 3 && j + 1 < MatrixDims && !cells[i][j + 1].isVisited) {
				cells[i][j].eastWall = false;
				return vec2(i, j + 1);
			}
			// the cell on the left of the current cell
			else if (num == 4 && j - 1 >= 1 && !cells[i][j - 1].isVisited) {
				cells[i][j - 1].eastWall = false;	// open gate of neighbor behind current column
				return vec2(i, j - 1);
			}
		}
	}
	// if there are no unvisited neighbors
	return vec2(-1, -1);
}

//----------------------------------------------------------------------------

/**
	Visits a cell recursively until all cells have been visited

	@param i: row of the cell
	@param j: column of the cell
	@param cells: the 2D array of cells
*/
void visit(int i, int j, Cell cells[][MatrixDims]) {
	vec2 neighbor = findNeighbor(i, j, cells);
	cells[i][j].isVisited = true;

	// visit all neighbors recursively
	while (neighbor.x != -1) {
		visit(neighbor.x, neighbor.y, cells);
		neighbor = findNeighbor(i, j, cells);
	}
}

//----------------------------------------------------------------------------

/**
	Constructs a maze using the matrix of cells. Chooses start 
	and end points, then starts visiting each cell.

	@param cells: the 2D array of cells
*/
void constructMaze(Cell cells[][MatrixDims]) {
	srand(time(0));
	// pick a starting point to traverse from
	int i = rand() % (MatrixDims - 1) + 1;
	int j = rand() % (MatrixDims - 1) + 1;

	// open a starting point on western wall
	int row = rand() % (MatrixDims - 1) + 1;
	int column = 0;
	cells[row][column].eastWall = false;

	// open an end point on eastern wall
	row = rand() % (MatrixDims - 1) + 1;
	column = MatrixDims - 1;
	cells[row][column].eastWall = false;
	visit(i, j, cells);

}

//----------------------------------------------------------------------------

int main(int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );    
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
	glewExperimental = GL_TRUE;
    glutCreateWindow( "Maze Generator" );
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

	// Set initial `Cell` states. All border cells are marked already visited.
	for (int i = 0; i < MatrixDims; i++)
		for (int j = 0; j < MatrixDims; j++) {
			if (i == 0) {
				cells[i][j].eastWall = false;
				cells[i][j].isVisited = true;
			}
			if (j == 0) {
				cells[i][j].southWall = false;
				cells[i][j].isVisited = true;
			}
		}

	constructMaze(cells);
	vec2 points[NumPoints];
	int currentPoint = 0;

	// start from the top right
	vec2 startingPoint = vec2(-1.0 - CenterOffset, 1.0 + CenterOffset);

	// loop through all cells and generate points for OpenGL depending on whether the southern and eastern
	// walls of the cell are open or closed
	for (int i = 0; i < MatrixDims; i++) {
		for (int j = 0; j < MatrixDims; j++) {
			if (cells[i][j].southWall) {
				
				// calculate 2 points for the southern wall and add to the points array
				vec2 point1 = vec2(startingPoint.x + (CellDim * j), startingPoint.y - (CellDim * (i + 1)));
				vec2 point2 = vec2(startingPoint.x + (CellDim * (j + 1)), startingPoint.y - (CellDim * (i + 1)));
				points[currentPoint] = point1;
				points[currentPoint + 1] = point2;
				currentPoint += 2;
			}
			if (cells[i][j].eastWall) {

				// calculate 2 points for the eastern wall and add to the points array
				vec2 point1 = vec2(startingPoint.x + (CellDim * (j + 1)), startingPoint.y - (CellDim * (i)));
				vec2 point2 = vec2(startingPoint.x + (CellDim * (j + 1)), startingPoint.y - (CellDim * (i + 1)));
				points[currentPoint] = point1;
				points[currentPoint + 1] = point2;
				currentPoint += 2;
			}
		}
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
