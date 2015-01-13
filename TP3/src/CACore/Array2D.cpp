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

const int Array2D::getDimX() const { 
	return m_dimX; 
}
 
const int Array2D::getDimY() const { 
	return m_dimY; 
}

float& Array2D::operator()(const int x, const int y) { 

	assert( x>=0 && x<m_dimX && y>=0 && y<m_dimY);
	return m_data[ y*m_dimX+x ]; 
}

float Array2D::operator()(const int x, const int y) const { 
	return operator()(x,y); 
}

float Array2D::interpolate(const float x, const float y) {

	assert( (x>=0) && (x<=m_dimX-1) && (y>=0) && (y<m_dimY-1));
	int X = int(x);
	int Y = int(y);
	if ((X+1>=m_dimX) || (Y+1>=m_dimY)) return operator()(X,Y);
	float IX_b = (x-X)*operator()(X+1,Y) + (X+1-x)*operator()(X,Y);
	float IX_h = (x-X)*operator()(X+1,Y+1) + (X+1-x)*operator()(X,Y+1);

	return (y-Y)*IX_h + (Y+1-y)*IX_b;
}