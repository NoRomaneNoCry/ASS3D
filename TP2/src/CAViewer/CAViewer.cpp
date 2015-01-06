

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "CAViewer.h"
#include <Mat4.h>
#include <Quaternion.h>

using namespace std;

CAViewer::~CAViewer()
{
}



void CAViewer::init()
{
	Viewer::init();
}



void CAViewer::draw()
{
	glPushMatrix();

    glPushMatrix();
    glColor3f( 1, 1, 0);
    draw_cube();
    glPopMatrix();

	glPopMatrix();
}



void CAViewer::keyPressed(unsigned char key, int x, int y)
{
	bool handled = false;
	if (key=='w')
	{
		bWireframe = !bWireframe;
		if (bWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		handled = true;
	}

	if (!handled)
	{
		Viewer::keyPressed(key,x,y);
	}
	updateGL();
}
