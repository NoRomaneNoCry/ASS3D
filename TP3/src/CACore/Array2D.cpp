#include "Array2D.h"
#include <cassert>

Array2D::Array2D() { 
	m_dimX = m_dimY = 0;
}

void Array2D::init(const int DIMX, const int DIMY) { 

	m_dimX = DIMX; 
	m_dimY = DIMY; 
	m_data.resize(m_dimX*m_dimY); 
}

void Array2D::setAll(const float v) { 
	m_data.assign(m_dimX*m_dimY, v); 
}

const int Array2D::getDimX() const { 
	return m_dimX; 
}
 
const int Array2D::getDimY() const { 
	return m_dimY; 
}

void Array2D::setValue(int x, int y, float value) {

	assert( x>=0 && x<m_dimX && y>=0 && y<m_dimY);
	m_data[ y*m_dimX+x ] = value;
}

void Array2D::addValue(int x, int y, float value) {

	assert( x>=0 && x<m_dimX && y>=0 && y<m_dimY);
	m_data[ y*m_dimX+x ] += value;
}

float Array2D::operator()(const int x, const int y) const { 

	assert( x>=0 && x<m_dimX && y>=0 && y<m_dimY);
    return m_data[ y*m_dimX+x ]; 
}

float Array2D::interpolate(const float x, const float y) const {

	assert( (x>=0) && (x<=m_dimX-1) && (y>=0) && (y<=m_dimY-1));
	int X = int(x);
	int Y = int(y);
	if ((X+1>=m_dimX) || (Y+1>=m_dimY)) return operator()(X,Y);
	float IX_b = (x-X)*operator()(X+1,Y) + (X+1-x)*operator()(X,Y);
	float IX_h = (x-X)*operator()(X+1,Y+1) + (X+1-x)*operator()(X,Y+1);

	return (y-Y)*IX_h + (Y+1-y)*IX_b;
}

void plus(const Array2D & op1, const Array2D & op2, Array2D & res) {

	assert(op1.getDimX() == op2.getDimX() && op1.getDimY() == op2.getDimY());

	for(int i = 0; i < op1.getDimX(); i++) {
		for(int j = 0; j < op1.getDimY(); j++) {
			res.setValue(i, j, op1(i, j) + op2(i, j));
		}
	}
}