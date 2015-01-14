#ifndef __SHALOWWATER_H__
#define __SHALOWWATER_H__

#include "Array2D.h"

class ShallowWater 
{
public:
   ShallowWater();
   void init(const int DIMX, const int DIMY, const float _dt);
   void draw() const;
   void computeOneStep();
   Array2D advect(const Array2D & src);
   void updateHeight();
   void updateVelocities();
   
protected:
   Array2D m_g;     // height of the ground (0 if flat)
   Array2D m_h;     // height of the water : the thing to compute and to draw
   Array2D m_n;     // m_n = m_h - m_g : amount of water above the ground
   Array2D m_vX;    // velocity along X   
   Array2D m_vY;    // velocity along Y

   float m_dt;
};

#endif