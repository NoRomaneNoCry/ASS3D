#include "ShalowWater.h"

#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <Vec3.h>

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
	m_n.setAll(1);
	m_n.setValue(4,4,2); m_n.setValue(4,5,3); m_n.setValue(4,6,2);
	m_n.setValue(5,4,2); m_n.setValue(5,5,3); m_n.setValue(5,6,2);
	m_n.setValue(6,4,2); m_n.setValue(6,5,3); m_n.setValue(6,6,2);
	plus(m_g, m_n, m_h);
	m_vX.setAll(0);
	m_vY.setAll(0);
}

void ShallowWater::draw() const {

	/*std::vector<math::Vec3f> normals;
	normals.resize(m_g.getDimX() * m_g.getDimY());*/

	glBegin(GL_QUADS);
	for(int i = 0; i < m_g.getDimX()-1; i++) {
		for(int j = 0; j < m_g.getDimY()-1; j++) {
			glVertex3f(i, m_h(i, j), j);
			glVertex3f(i+1, m_h(i+1, j), j);
			glVertex3f(i+1, m_h(i+1, j+1), j+1);
			glVertex3f(i, m_h(i, j+1), j+1);
		}
	}
	glEnd();
}

void ShallowWater::computeOneStep() {

	Array2D tempvX;
	Array2D tempvY;

	m_n = advect(m_n);
	tempvX = advect(m_vX);
	tempvY = advect(m_vY);
	m_vX = tempvX; 
	m_vY = tempvY;
	updateHeight();
	plus(m_n, m_g, m_h);
	updateVelocities();
}


Array2D ShallowWater::advect(const Array2D & src) {

	Array2D temp;
	temp.init(src.getDimX(), src.getDimY());

	float x, y;
	temp = src;

	for(int j = 1; j < src.getDimY()-1; j++) {
		for(int i = 1; i < src.getDimX()-1; i++) {
			x = i - m_dt * m_vX(i, j);
			y = j - m_dt * m_vY(i, j);
			temp.setValue(i, j, src.interpolate(x, y));
		}
	}
	return temp;
}

void ShallowWater::updateHeight() {

	Array2D tempN;
	tempN.init(m_n.getDimX(), m_n.getDimY());
	tempN = m_n;

	for(int j = 1; j < m_n.getDimY()-1; j++) {
		for(int i = 1; i < m_n.getDimX()-1; i++) {
			tempN.addValue(i, j, - m_n(i, j) * m_dt * ( (m_vX(i+1, j) - m_vX(i, j)) 
				+ (m_vY(i, j+1) - m_vY(i, j)) ) );
		}
	}
	m_n = tempN;
}

void ShallowWater::updateVelocities() {

	float a = 9.81;

	for(int j = 1; j < m_h.getDimY(); j++) {
		for(int i = 2; i < m_h.getDimX(); i++) {
			m_vX.addValue(i, j, a * m_dt * (m_h(i-1, j) - m_h(i, j)) );
		}
	}
	for(int j = 2; j < m_h.getDimY(); j++) {
		for(int i = 1; i < m_h.getDimX(); i++) {
			m_vY.addValue(i, j, a * m_dt * (m_h(i, j-1) - m_h(i, j)) );
		}
	}
}
