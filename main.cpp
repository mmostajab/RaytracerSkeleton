#include <assert.h>
#include <GL/glut.h>
#include <stdio.h>
#include <time.h>

#include "raytracer.h"


RayTracer::CRayTracer g_World;
bool g_UpdateFlag = true;
GLuint texture;

void drawBitmapText(char *string,float x,float y) 
{  
	char *c;
	glRasterPos2f(x, y);
	glDisable(GL_TEXTURE_2D);
	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10/* GLUT_BITMAP_HELVETICA_12*/, *c);
	}

	glEnable(GL_TEXTURE_2D);
}

void mouseHandler(int button, int state, int x, int y)
{
	static int a = 1;
	switch(button)
	{
	case 0:
		if(state)
		{
			// LBUTTON_UP
		}
		else
		{
			// LBUTTON_DOWN
		}
		break;
	case 1:
		if(state)
		{
			// MBUTTON_UP
		}
		else
		{
			// MBUTTON_DOWN
		}
		break;
	case 2:
		if(state)
		{
			// RBUTTON_UP
		}
		else
		{
			// RBUTTON_DOWN
		}
		break;
	}
}

void keyboardHandler(unsigned char key, int x, int y)
{
	float RoatationVal(10);
	float TranslationVal(100);

	switch(key)
	{
	case 27:
		//RAYTRACER_LOGCLOSE
		exit(0);
		break;

	case 'w':
	case 'W':
	    // Move camera
		g_UpdateFlag = true;
		break;

	case 's':
	case 'S':
		// Move camera
		g_UpdateFlag = true;
		break;

	case 'a':
	case 'A':
		// Move camera
		g_UpdateFlag = true;
		break;

	case 'd':
	case 'D':
		// Move camera
		g_UpdateFlag = true;
		break;

	case 'e':
	case 'E':
		// Move camera
		g_UpdateFlag = true;
		break;

	case 'q':
	case 'Q':
		// Move camera
		g_UpdateFlag = true;
		break;
	}
}



void display(void)
{
	//Clear all pixels
	glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    

	//draw white polygon (rectangle) with corners at
	// (0.0, 0.0, 0.0) and (1.0, 1.0, 0.0)
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2d(1.0, 0.0); glVertex3f(1.0, 0.0, 0.0);
		glTexCoord2d(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
		glTexCoord2d(0.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glEnd();
	
	glColor3f(1.0f, 1.0f, 1.0f);

	char msg[] = "Demo Scene";
	drawBitmapText(msg, 0.01f, 0.97f);
	
	glFlush();
}

void init(void)
{
	//select clearing (background) color
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//initialize viewing values 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void idle(void)
{
	if(g_UpdateFlag)
	{
		g_World.renderScene();
		g_World.saveBitmapToFile( "Result.ppm" );
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, IMG_WIDTH, IMG_HEIGHT, 0, GL_RGB, GL_FLOAT, g_World.getColorBuffer() );

		g_UpdateFlag = false;
	
		display();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(IMG_WIDTH,IMG_HEIGHT);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Demo Raytracer");

	// Type the code for world initialization

	init();

	srand(clock());

	glutDisplayFunc(display);
	glutMouseFunc(mouseHandler);
	glutKeyboardFunc(keyboardHandler);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}