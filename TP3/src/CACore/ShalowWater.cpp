#include "ShalowWater.h"

#include <iostream>
#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

ShallowWater::ShallowWater() {

	m_g = Array2D();
	m_h = Array2D();
	m_n = Array2D();
	m_vX = Array2D();
	m_vY = Array2D();
}

void ShallowWater::init(const int DIMX, const int DIMY, const float _dt) {

	m_dt = _dt;
	m_g.init(DIMX, DIMY);
	m_n.init(DIMX, DIMY);
	m_vX.init(DIMX, DIMY);
	m_vY.init(DIMX, DIMY);
	m_h.init(DIMX, DIMY);
	m_g.setAll(0);
	m_n.setAll(20);
	m_n.setValue(5,5,25); m_n.setValue(5,6,25);
	plus(m_g, m_n, m_h);
	m_vX.setAll(0);
	m_vY.setAll(0);
}

void ShallowWater::draw() const {

	glBegin(GL_POINTS);
	for(int i = 0; i < m_g.getDimX(); i++) {
		for(int j = 0; j < m_g.getDimY(); j++) {
			glVertex3f(i, m_h(i, j), j);
		}
	}
	glEnd();
}

void ShallowWater::computeOneStep() {

	Array2D tempvX;
	Array2D tempvY;

	advect(m_n, m_n);
	advect(tempvX, m_vX);
	advect(tempvY, m_vY);
	m_vX = tempvX; m_vY = tempvY;
	updateHeight();
	plus(m_n, m_g, m_h);
	updateVelocities();
}


void ShallowWater::advect(Array2D & dest, const Array2D & src) {

	float x, y;
	float dt = 0.1;
	dest.init(src.getDimX(), src.getDimY());

	for(int j = 1; j < src.getDimY()-1; j++) {
		for(int i = 1; i < src.getDimX()-1; i++) {
			x = i - m_dt * m_vX(i, j);
			y = j - m_dt * m_vY(i, j);
			dest.setValue(i, j, src.interpolate(x, y));
		}
	}
}

void ShallowWater::updateHeight() {

	Array2D tempN;
	tempN.init(m_n.getDimX(), m_n.getDimY());
	tempN = m_n;

	for(int j = 1; j < m_n.getDimY()-1; j++) {
		for(int i = 1; i < m_n.getDimX()-1; i++) {
			// les 1.f correspondent respectivement à Dx et Dy, le rapport de la taille 
			// réelle sur dimX et dimY
			tempN.setValue(i, j, m_n(i, j) - m_n(i, j) * m_dt * 
				((m_vX(i+1, j) - m_vX(i, j)) / 1.f + (m_vY(i, j+1) - m_vY(i, j)) / 1.f));
		}
	}
	m_n = tempN;
}

void ShallowWater::updateVelocities() {

	float a = 9.81;

	for(int j = 1; j < m_h.getDimY(); j++) {
		for(int i = 2; i < m_h.getDimX(); i++) {
			// divisé par Dx
			m_vX.addValue(i, j, a * m_dt * ((m_h(i, j-1)-m_h(i, j)) / 1.f));
		}
	}
	for(int j = 2; j < m_h.getDimY(); j++) {
		for(int i = 1; i < m_h.getDimX(); i++) {
			// divisé par Dy
			m_vY.addValue(i, j, a * m_dt * ((m_h(i, j-1)-m_h(i, j)) / 1.f));
		}
	}
}
