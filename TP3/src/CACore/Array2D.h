#ifndef __ARRAY2D_H__
#define __ARRAY2D_H__

#include <vector>

class Array2D
{
public:
   Array2D();
   void init(const int DIMX, const int DIMY);
   const int getDimX() const;
   const int getDimY() const;
   float& operator()(const int x, const int y);
   float operator()(const int x, const int y) const;
   float interpolate(const float x, const float y);
   
protected:
   std::vector<float> m_data;
   int m_dimX, m_dimY;
};

#endif