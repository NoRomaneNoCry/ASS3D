#ifndef __ARRAY2D_H__
#define __ARRAY2D_H__

#include <vector>

template <typename T>
class Array2D
{
public:
   Array2D();
   void init(const int DIMX, const int DIMY);
   void setAll(const T v);
   const int getDimX() const;
   const int getDimY() const;
   void setValue(int x, int y, T value);
   void addValue(int x, int y, T value);
   T operator()(const int x, const int y) const;
   T interpolate(const float x, const float y) const;
   
protected:
   std::vector<T> m_data;
   int m_dimX, m_dimY;
};

template <typename T>
void plus(const Array2D<T> & op1, const Array2D<T> & op2, Array2D<T> & res);

#endif
