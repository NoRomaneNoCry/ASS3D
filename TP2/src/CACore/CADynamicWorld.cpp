#include "CADynamicWorld.h"

#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>

const math::Vec3f CADynamicWorld::m_gravity = math::Vec3f(0,-2,0);

float distance3D(math::Vec3f a, math::Vec3f b) {
	return sqrt(pow(b.x - a.x,2) + pow(b.y - a.y,2) + pow(b.z - a.z,2));
}

CADynamicWorld::CADynamicWorld() {

	m_p = std::vector<CAParticle>();
	m_s = std::vector<CASpring>();
}

void CADynamicWorld::init(const int dimX, const int dimY) {

	m_p.reserve(dimX * dimY);
	for(int i = 0; i < dimX; i++) {
		for(int j = 0; j < dimY; j++) {
			m_p.push_back(CAParticle(1, math::Vec3f(i,10,j)));
		}
	}
	m_p[dimX/2].modifyMass(-1);
	m_p[(dimY-2)*dimX + dimX/2].modifyMass(-1);

	for(int i = 0; i < dimX; i++) {
		for(int j = 0; j < dimY; j++) {
			if(i+1 < dimX) {
				m_s.push_back(CASpring(&m_p[(i*dimY)+j], &m_p[((i+1)*dimY)+j], 
					0.5, 10, 2.5));
			}

			if(j+1 < dimY) {
				m_s.push_back(CASpring(&m_p[(i*dimY)+j], &m_p[(i*dimY)+j+1],
					0.5, 10, 2.5));
			}
		}
	}

	m_centre_sphere = math::Vec3f(3.0,6.0,6.0);
	m_rayon_sphere = 3.5;
	m_dimX = dimX;
	m_dimY = dimY;
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

	manageGroundCollisions(0.2);
	manageSphereCollisions();	
}

void CADynamicWorld::manageGroundCollisions(float coef_rebond) {

	std::vector<CAParticle>::iterator it_p;
	for(it_p = m_p.begin(); it_p != m_p.end(); ++it_p) {
		if((*it_p).getPosition().y < 0) {

			math::Vec3f deltaP = math::Vec3f(0.0, -2*(*it_p).getPosition().y, 0.0);

			(*it_p).setPosition() += coef_rebond * deltaP;
			(*it_p).setSpeed() = math::Vec3f((*it_p).getSpeed().x, 
				-coef_rebond*(*it_p).getSpeed().y, (*it_p).getSpeed().z);
		}
	}
}

void CADynamicWorld::manageSphereCollisions() {

	std::vector<CAParticle>::iterator it_p;
	for(it_p = m_p.begin(); it_p != m_p.end(); ++it_p) {

		float dist = distance3D(m_centre_sphere, (*it_p).getPosition());
		if(dist < m_rayon_sphere) {

			math::Vec3f normale((*it_p).getPosition() - m_centre_sphere);
			normale.normalize();
			math::Vec3f normaleOrtho(normale.y, -normale.x, normale.z);

			(*it_p).setSpeed() =  normaleOrtho - 0.5*((*it_p).getSpeed()*normale)*normale;
		}
	}
}

void CADynamicWorld::draw() const {

	glBegin(GL_QUADS);
	for(int i = 0; i < m_dimX-1; i++) {
		for(int j = 0; j < m_dimY-1; j++) {
			glVertex3fv(m_p[i * m_dimY + j].getPosition());
			glVertex3fv(m_p[(i+1) * m_dimY + j].getPosition());
			glVertex3fv(m_p[(i+1) * m_dimY + j+1].getPosition());
			glVertex3fv(m_p[i * m_dimY + j+1].getPosition());
		}
	}
	glEnd();

	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glTranslatef(m_centre_sphere.x, m_centre_sphere.y, m_centre_sphere.z);
	glutSolidSphere(m_rayon_sphere, 10, 10);
	glPopMatrix();
}

