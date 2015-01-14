#ifndef __ARRAY2D_H__
#define __ARRAY2D_H__

#include <vector>

class Array2D
{
public:
   Array2D();
   void init(const int DIMX, const int DIMY);
   void setAll(const float v);
   const int getDimX() const;
   const int getDimY() const;
   void setValue(int x, int y, float value);
   void addValue(int x, int y, float value);
   float operator()(const int x, const int y) const;
   float interpolate(const float x, const float y) const;
   
protected:
   std::vector<float> m_data;
   int m_dimX, m_dimY;
};

void plus(const Array2D & op1, const Array2D & op2, Array2D & res);

#endif
