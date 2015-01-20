#include "Array2D.h"
#include <cassert>
#include <iostream>

template <typename T>
Array2D<T>::Array2D() { 
	m_dimX = m_dimY = 0;
	m_data = std::vector<T>();
}

template <typename T>
void Array2D<T>::init(const int DIMX, const int DIMY) { 

	m_dimX = DIMX; 
	m_dimY = DIMY; 
	m_data.resize(m_dimX*m_dimY); 
}

template <typename T>
void Array2D<T>::setAll(const T v) { 
	m_data.assign(m_dimX*m_dimY, v); 
}

template <typename T>
const int Array2D<T>::getDimX() const { 
	return m_dimX; 
}
 
template <typename T>
const int Array2D<T>::getDimY() const { 
	return m_dimY; 
}

template <typename T>
void Array2D<T>::setValue(int x, int y, T value) {

	assert( x>=0 && x<m_dimX && y>=0 && y<m_dimY);
	m_data[ y*m_dimX+x ] = value;
}

template <typename T>
void Array2D<T>::addValue(int x, int y, T value) {

	assert( x>=0 && x<m_dimX && y>=0 && y<m_dimY);
	m_data[ y*m_dimX+x ] += value;
}

template <typename T>
T Array2D<T>::operator()(const int x, const int y) const { 

	assert( x>=0 && x<m_dimX && y>=0 && y<m_dimY);
    return m_data[ y*m_dimX+x ]; 
}

template <typename T>
T Array2D<T>::interpolate(const float x, const float y) const {

	if ((x < 0) || (x > m_dimX) || (y < 0) || (y > m_dimY)) 
		return 0.f;
		
	int X = int(x);
	int Y = int(y);
	if ((X+1>=m_dimX) || (Y+1>=m_dimY)) {
		return operator()(X,Y);
	}

	T IX_b = (x-X)*operator()(X+1,Y) + (X+1-x)*operator()(X,Y);
	T IX_h = (x-X)*operator()(X+1,Y+1) + (X+1-x)*operator()(X,Y+1);

	return (y-Y) * IX_h + (Y+1-y) * IX_b;
}

template <typename T>
void plus(const Array2D<T> & op1, const Array2D<T> & op2, Array2D<T> & res) {

	assert(op1.getDimX() == op2.getDimX() && op1.getDimY() == op2.getDimY());

	for(int i = 0; i < op1.getDimX(); i++) {
		for(int j = 0; j < op1.getDimY(); j++) {
			res.setValue(i, j, op1(i, j) + op2(i, j));
		}
	}
}