

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

	m_particle = CAParticle(1.2, math::Vec3f(0, 70, 0));
}



void CAViewer::draw()
{
    glPushMatrix();
    glColor3f( 1, 1, 0);
    glTranslatef(m_particle.getPosition().x, m_particle.getPosition().y,
    	m_particle.getPosition().z);
    draw_cube();
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

void CAViewer::animate() 
{
	// Pour que la particule tombe
	m_particle.addForce(math::Vec3f(0, -2, 0));
	m_particle.computeOneStep(0.1);
}
