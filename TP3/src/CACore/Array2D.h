#ifndef __ARRAY2D_H__
#define __ARRAY2D_H__

class Array2D
{
public:
   Array2D() { m_dimX=m_dimY=0; }
   void init(const int DIMX, const int DIMY);
   const int getDimX() const { return m_dimX; }
   const int getDimY() const { return m_dimY; }
   float& operator()(const int x, const int y);
   float operator()(const int x, const int y);
   float interpolate(const float x, const float y);
   
protected:
   vector<float> m_data;
   int m_dimX, m_dimY;
};

#endif