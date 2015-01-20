

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
	m_shallow = ShallowWater();
}

void CAViewer::init()
{
	Viewer::init();
	m_shallow.init(20, 20, 0.1);
}

void CAViewer::draw()
{
	m_shallow.draw();
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
	else if(key=='n')
	{
		m_shallow.computeOneStep();
		handled = true;
	}

	if (!handled)
	{
		Viewer::keyPressed(key,x,y);
	}
	updateGL();
}

void CAViewer::animate() 
{
	m_shallow.computeOneStep();
}
