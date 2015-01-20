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

void ShallowWater::init(const int DIMX, const int DIMY, const float _dt) {

	m_dt = _dt;
	m_g.init(DIMX, DIMY);
	m_n.init(DIMX, DIMY);
	m_vX.init(DIMX, DIMY);
	m_vY.init(DIMX, DIMY);
	m_h.init(DIMX, DIMY);

	m_g.setAll(0);
	m_g.setValue(7,15,1.5);
	m_n.setAll(1);
	m_n.setValue(4,4,2); m_n.setValue(4,5,3); m_n.setValue(4,6,2);
	m_n.setValue(5,4,2); m_n.setValue(5,5,3); m_n.setValue(5,6,2);
	m_n.setValue(6,4,2); m_n.setValue(6,5,3); m_n.setValue(6,6,2);
	plus(m_g, m_n, m_h);
	m_vX.setAll(0);
	m_vY.setAll(0);

	m_g_normals = computeNormals(m_g);
}

void ShallowWater::draw() const {

	Array2D<math::Vec3f> normals;
	normals = computeNormals(m_n);

	glBegin(GL_QUADS);
	glColor4f(0.0,0.0,1.0, 0.5);
	for(int i = 0; i < m_n.getDimX()-1; i++) {
		for(int j = 0; j < m_n.getDimY()-1; j++) {
			glNormal3f(normals(i,j).x, normals(i,j).y, normals(i,j).z);
			glVertex3f(i, m_n(i, j), j);
			glNormal3f(normals(i+1,j).x, normals(i+1,j).y, normals(i+1,j).z);
			glVertex3f(i+1, m_n(i+1, j), j);
			glNormal3f(normals(i+1,j+1).x, normals(i+1,j+1).y, normals(i+1,j+1).z);
			glVertex3f(i+1, m_n(i+1, j+1), j+1);
			glNormal3f(normals(i,j+1).x, normals(i,j+1).y, normals(i,j+1).z);
			glVertex3f(i, m_n(i, j+1), j+1);
		}
	}
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.5,0.5,0.5);
	for(int i = 0; i < m_g.getDimX()-1; i++) {
		for(int j = 0; j < m_g.getDimY()-1; j++) {
			glNormal3f(m_g_normals(i,j).x, m_g_normals(i,j).y, m_g_normals(i,j).z);
			glVertex3f(i, m_g(i, j), j);
			glNormal3f(m_g_normals(i+1,j).x, m_g_normals(i+1,j).y, m_g_normals(i+1,j).z);
			glVertex3f(i+1, m_g(i+1, j), j);
			glNormal3f(m_g_normals(i+1,j+1).x, m_g_normals(i+1,j+1).y, m_g_normals(i+1,j+1).z);
			glVertex3f(i+1, m_g(i+1, j+1), j+1);
			glNormal3f(m_g_normals(i,j+1).x, m_g_normals(i,j+1).y, m_g_normals(i,j+1).z);
			glVertex3f(i, m_g(i, j+1), j+1);
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

			a = math::Vec3f(i, src(i, j), j);
			b = math::Vec3f(i+1, src(i+1, j), j);
			c = math::Vec3f(i+1, src(i+1, j+1), j+1);
			d = math::Vec3f(i, src(i, j+1), j+1);
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
