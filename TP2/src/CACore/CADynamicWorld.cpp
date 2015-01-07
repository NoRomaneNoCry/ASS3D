#include "CADynamicWorld.h"

#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

const math::Vec3f CADynamicWorld::m_gravity = math::Vec3f(0,-2,0);

CADynamicWorld::CADynamicWorld() {

	m_p = std::vector<CAParticle>();
	m_s = std::vector<CASpring>();
}

void CADynamicWorld::init(const int dimX, const int dimY) {

	m_p.reserve(dimX * dimY);
	for(int i = 0; i < dimX; i++) {
		for(int j = 0; j < dimY; j++) {
			m_p.push_back(CAParticle(1.2, math::Vec3f(5*i,10,5*j)));
		}
	}
	m_p[dimY/2].modifyMass(-1);
	m_p[((dimX-1)*dimY)+(dimY/2)].modifyMass(-1);
	for(int i = 0; i < dimX; i++) {
		for(int j = 0; j < dimY; j++) {
			if(i+1 < dimX) {
				m_s.push_back(CASpring(&m_p[(i*dimY)+j], &m_p[((i+1)*dimY)+j], 
					5, 25, 0.5));
			}

			if(j+1 < dimY) {
				m_s.push_back(CASpring(&m_p[(i*dimY)+j], &m_p[(i*dimY)+j+1],
					5, 25, 0.5));
			}
		}
	}
}

void CADynamicWorld::computeOneStep(const float dt) {

	std::vector<CAParticle>::iterator it_p;
	std::vector<CASpring>::iterator it_s;

	for(it_s = m_s.begin(); it_s != m_s.end(); ++it_s)
		(*it_s).applyForceToParticles();

	for(it_p = m_p.begin(); it_p != m_p.end(); ++it_p) {
		(*it_p).addForce(CADynamicWorld::m_gravity) ;
		(*it_p).computeOneStep(dt);
	}

	//TODO : gestion des collisions !
	
}

void CADynamicWorld::draw() const {

	std::vector<CAParticle>::const_iterator it_p;
	std::vector<CASpring>::const_iterator it_s;
	math::Vec3f position;

	glBegin(GL_POINTS);
	for(it_p = m_p.begin(); it_p != m_p.end(); ++it_p) {
		
		position = (*it_p).getPosition();
		glVertex3f(position.x, position.y, position.z);
	}
	glEnd();

	glBegin(GL_LINES);
	for(it_s = m_s.begin(); it_s != m_s.end(); ++it_s) {

		position = (*it_s).getParticle1()->getPosition();
		glVertex3f(position.x, position.y, position.z);
		position = (*it_s).getParticle2()->getPosition();
		glVertex3f(position.x, position.y, position.z);
	}
	glEnd();
}

