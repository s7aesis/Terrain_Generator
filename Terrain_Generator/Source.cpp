#include "Headers.h"



//Globals


float gCamPos[] = { 0, 50, 400 };	//where the camera is

float gSceneRotation[3] = { 0, 0, 0 }; //the rotation of the scene
float gMinSceneRotationX = 0, gMaxSceneRotationX = 90;
int gMinTerrainSize = 50, gMaxTerrainSize = 300;
unsigned int gDrawMode = 0;

int gWindowPositionX = 350, gWindowPositionY = 50;
int gWindowSizeX = 800, gWindowSizeY = 800;
int gWindow2SizeX = gMaxTerrainSize, gWindow2SizeY = gMaxTerrainSize;
int gWindow1 = 0, gWindow2 = 0; //specifies which window to work with

//lighting
float light_pos[] = { 0, 5, 0, 1.0 };

float amb0[4] = { 1, 1, 1, 1 };
float diff0[4] = { 1, 1, 1, 1 };
float spec0[4] = { 1, 1, 1, 1 };

float m_amb[] = { 0.1, 0.1, 0.1, 1.0 };
float m_diff[] = { 1, 1, 1, 1.0 };
float m_spec[] = { 1, 1, 1, 1.0 };
float shiny = 0.8f;

//Class instantiations
TerrainGenerator terrainGenerator;


void promptUser()
{
	int terrainSize = 0;
	cout << "Welcome to Terrain Generator\n\n";
	
	while (terrainSize < gMinTerrainSize || terrainSize > gMaxTerrainSize)
	{
		cout << "Please enter a valid terrain size \nto generate (50-300) -> ";
		cin >> terrainSize;
	}
	cout << "\nWorking...";
	terrainGenerator.setSize(terrainSize);
	terrainGenerator.setupTerrain();
	
}


//OpenGL functions
void keyboard(unsigned char key, int xIn, int yIn)
{
	switch (key)
	{
	case '_':
		gCamPos[2] += 10;
	case '-':
		gCamPos[2] += 10;
		break;
	case '=':
		gCamPos[2] -= 10;
	case '+':
		gCamPos[2] -= 10;
		break;
	case 'w':
		gDrawMode++;
		if (gDrawMode > 2)
			gDrawMode = 0;

		switch (gDrawMode)
		{
		case 0:
			cout << "solid";
			terrainGenerator.setFillMode(TerrainGenerator::SOLID);
			break;
		case 1:
			cout << "wireframe";
			terrainGenerator.setFillMode(TerrainGenerator::WIREFRAME);
			break;
		case 2:
			cout << "combo";
			terrainGenerator.setFillMode(TerrainGenerator::COMBINATION);
			break;
		}
		
		break;
	case 'r':
		terrainGenerator.reset();
		break;
	case 'q':
	case 27:	//27 is the esc key
		exit(0);
		break;
	}
}

void init(void)
{
	/* Setup GL features */
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE); //enable backface culling

	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 400);
}

/* callbacks start here */
void special(int key, int x, int y)
{
	switch (key)
	{

		/* Arrow keys change the rotation of the scene*/
	case GLUT_KEY_RIGHT:
		gSceneRotation[1] -= 1;
		break;

	case GLUT_KEY_LEFT:
		gSceneRotation[1] += 1;
		break;

	case GLUT_KEY_UP:
		if (gSceneRotation[0] < gMaxSceneRotationX)
			gSceneRotation[0] += 1;
		break;

	case GLUT_KEY_DOWN:
		if (gSceneRotation[0] > gMinSceneRotationX)
			gSceneRotation[0] -= 1;
		break;
	}
	glutPostRedisplay();
}

/* display function - GLUT display callback function
*		clears the screen, sets the camera position, draws the ground plane and movable box
*/
void display1(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	gluLookAt(gCamPos[0], gCamPos[1], gCamPos[2], 0, 0, 0, 0, 1, 0);
	glPushMatrix(); // push scene rotation
	//rotate the scene by the amount specified by the user
	

	glRotatef(gSceneRotation[0], 1, 0, 0);
	glRotatef(gSceneRotation[1], 0, 1, 0);

	if (terrainGenerator.getTerrainSize() > gMinTerrainSize && terrainGenerator.getTerrainSize() < gMaxTerrainSize)
		glTranslatef(-terrainGenerator.getTerrainSize() / 2, 0, -terrainGenerator.getTerrainSize() / 2);

	glColor3f(1, 1, 1);

	terrainGenerator.drawScene();

	
	glutSwapBuffers();
}

void display2(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	
	glScalef(-1, 1, -1);
	glTranslatef(1, -1, 0);

	gluLookAt(0, 0, -1, 0, 0, 0, 0, 1, 0);
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glColor3f(0, 1, 1);
	glVertex3f(0, 1, 0);
	glColor3f(1, 0, 1);
	glVertex3f(1, 1, 0);
	glColor3f(1, 1, 1);
	glVertex3f(1, 0, 0);
	glEnd();
	glPopMatrix();
	
	glutSwapBuffers();
}

void idle(void)
{
	glutSetWindow(gWindow1);
	glutPostRedisplay();
	glutSetWindow(gWindow2);
	glutPostRedisplay();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	promptUser();
	
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(gWindowSizeX, gWindowSizeY);
	glutInitWindowPosition(gWindowPositionX, gWindowPositionY);
	
	gWindow1 = glutCreateWindow("Terain Generator");	//creates the window
	
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

	glutDisplayFunc(display1);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutIdleFunc(idle);

	init();

	glutInitWindowSize(gWindow2SizeX, gWindow2SizeY);
	glutInitWindowPosition(gWindowSizeX + gWindowPositionX, gWindowPositionY);
	gWindow2 = glutCreateWindow("Height Map");
	glutDisplayFunc(display2);
	glutIdleFunc(idle);
	

	glutMainLoop();				//starts the event glutMainLoop

	
	return(0);					//return may not be necessary on all compilers
}
