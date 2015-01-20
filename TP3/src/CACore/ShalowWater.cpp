#include "ShalowWater.h"

#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

ShallowWater::ShallowWater() {

	m_g = Array2D<float>();
	m_h = Array2D<float>();
	m_n = Array2D<float>();
	m_vX = Array2D<float>();
	m_vY = Array2D<float>();
	m_g_normals = Array2D<math::Vec3f>();
}

void ShallowWater::init(const int DIMX, const int DIMY, const float _dt, 
	const float _dx, const float _dy) {

	m_dt = _dt;
	m_dx = _dx;
	m_dy = _dy;

	m_g.init(DIMX, DIMY);
	m_n.init(DIMX, DIMY);
	m_vX.init(DIMX, DIMY);
	m_vY.init(DIMX, DIMY);
	m_h.init(DIMX, DIMY);

	m_n.setAll(1);
	m_n.setValue(4,4,4); m_n.setValue(4,5,6); m_n.setValue(4,6,4);
	m_n.setValue(5,4,4); m_n.setValue(5,5,6); m_n.setValue(5,6,4);
	m_n.setValue(6,4,4); m_n.setValue(6,5,6); m_n.setValue(6,6,4);

	plus(m_g, m_n, m_h);
	m_vX.setAll(0);
	m_vY.setAll(0);

	m_g_normals = computeNormals(m_g);
}

void ShallowWater::draw() const {

	Array2D<math::Vec3f> normals;
	normals = computeNormals(m_h);

	glBegin(GL_QUADS);
	glColor4f(0.0,0.4,0.8,0.5);
	for(int i = 0; i < m_h.getDimX()-1; i++) {
		for(int j = 0; j < m_h.getDimY()-1; j++) {
			glNormal3fv(normals(i, j));
			glVertex3f(i*m_dx, m_h(i, j), j*m_dy);
			glNormal3fv(normals(i+1,j));
			glVertex3f((i+1)*m_dx, m_h(i+1, j), j*m_dy);
			glNormal3fv(normals(i+1,j+1));
			glVertex3f((i+1)*m_dx, m_h(i+1, j+1), (j+1)*m_dy);
			glNormal3fv(normals(i,j+1));
			glVertex3f(i*m_dx, m_h(i, j+1), (j+1)*m_dy);
		}
	}
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.4,0.2,0.0);
	for(int i = 0; i < m_g.getDimX()-1; i++) {
		for(int j = 0; j < m_g.getDimY()-1; j++) {
			glNormal3fv(m_g_normals(i,j));
			glVertex3f(i*m_dx, m_g(i, j), j*m_dy);
			glNormal3fv(m_g_normals(i+1,j));
			glVertex3f((i+1)*m_dx, m_g(i+1, j), j*m_dy);
			glNormal3fv(m_g_normals(i+1,j+1));
			glVertex3f((i+1)*m_dx, m_g(i+1, j+1), (j+1)*m_dy);
			glNormal3fv(m_g_normals(i,j+1));
			glVertex3f(i*m_dx, m_g(i, j+1), (j+1)*m_dy);
		}
	}
	glEnd();
}

void ShallowWater::computeOneStep() {

	Array2D<float> tempvX;
	Array2D<float> tempvY;

	m_n = advect(m_n);
	tempvX = advect(m_vX);
	tempvY = advect(m_vY);
	m_vX = tempvX; 
	m_vY = tempvY;
	updateHeight();
	plus(m_n, m_g, m_h);
	updateVelocities();
	reflectingBoundaries();
}


Array2D<float> ShallowWater::advect(const Array2D<float> & src) {

	Array2D<float> temp;
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

	Array2D<float> tempN;
	tempN.init(m_n.getDimX(), m_n.getDimY());
	tempN = m_n;

	for(int j = 1; j < m_n.getDimY()-1; j++) {
		for(int i = 1; i < m_n.getDimX()-1; i++) {
			tempN.addValue(i, j, - m_n(i, j) * m_dt * ( (m_vX(i+1, j) - m_vX(i, j))
				/ m_dx + (m_vY(i, j+1) - m_vY(i, j)) / m_dy ) );
		}
	}
	m_n = tempN;
}

void ShallowWater::updateVelocities() {

	float a = 9.81;

	for(int j = 1; j < m_h.getDimY(); j++) {
		for(int i = 2; i < m_h.getDimX(); i++) {
			m_vX.addValue(i, j, a * m_dt * ((m_h(i-1, j) - m_h(i, j)) / m_dx));
		}
	}
	for(int j = 2; j < m_h.getDimY(); j++) {
		for(int i = 1; i < m_h.getDimX(); i++) {
			m_vY.addValue(i, j, a * m_dt * ((m_h(i, j-1) - m_h(i, j)) / m_dy));
		}
	}
}

void ShallowWater::reflectingBoundaries() {

	int xMax = m_h.getDimX()-1;
	int yMax = m_h.getDimY()-1;
	for(int j = 0; j <= yMax; j++) {
		m_h.setValue(0, j, m_h(1, j));
		m_h.setValue(xMax, j, m_h(xMax-1, j));
		m_vX.setValue(1, j, 0.0);
		m_vX.setValue(xMax, j, 0.0);
		m_vY.setValue(0, j, 0.0);
		m_vY.setValue(xMax, j, 0.0);
	}
	for(int i = 0; i <= xMax; i++) {
		m_h.setValue(i, 0, m_h(i, 1));
		m_h.setValue(i, yMax, m_h(i, yMax-1));
		m_vX.setValue(i, 0, 0.0);
		m_vX.setValue(i, yMax, 0.0);
		m_vY.setValue(i, 1, 0.0);
		m_vY.setValue(i, yMax, 0.0);
	}

}

Array2D<math::Vec3f> ShallowWater::computeNormals(const Array2D<float> & src) const {

	math::Vec3f a, b, c, d;
	Array2D<math::Vec3f> upNorms;
	upNorms.init(src.getDimX()-1, src.getDimY()-1);
	Array2D<math::Vec3f> downNorms;
	downNorms.init(src.getDimX()-1, src.getDimY()-1);
	Array2D<math::Vec3f> temp;
	temp.init(src.getDimX(), src.getDimY());

	for(int i = 0; i < upNorms.getDimX(); i++) {
		for(int j = 0; j < upNorms.getDimY(); j++) {

			a = math::Vec3f(i*m_dx, src(i, j), j*m_dy);
			b = math::Vec3f((i+1)*m_dx, src(i+1, j), j*m_dy);
			c = math::Vec3f((i+1)*m_dx, src(i+1, j+1), (j+1)*m_dy);
			d = math::Vec3f(i*m_dx, src(i, j+1), (j+1)*m_dy);
			upNorms.setValue(i, j, d-a ^ b-a);
			downNorms.setValue(i, j, b-c ^ d-c);
		}
	}

	for(int i = 0; i < src.getDimX(); i++) {
		for(int j = 0; j < src.getDimY(); j++) {

			math::Vec3f tmp(0.0, 0.0, 0.0);
			int tmpNb = 0;
			if(i-1 >= 0 && j < src.getDimY()-1) {
				tmpNb += 2;
				tmp += upNorms(i-1, j) + downNorms(i-1, j);
			}
			if(j-1 >= 0 && i < src.getDimX()-1) {
				tmpNb += 2;
				tmp += upNorms(i, j-1) + downNorms(i, j-1);
			}
			if(j-1 >= 0 && i-1 >= 0) {
				tmpNb++;
				tmp += downNorms(i-1, j-1);
			}
			if(i < src.getDimX()-1 && j < src.getDimY()-1) {
				tmpNb++;
				tmp += upNorms(i, j);
			}

			temp.setValue(i, j, (tmp/tmpNb).normalize());
		}
	}
	return temp;
}
