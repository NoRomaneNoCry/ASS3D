#ifndef __SHALOWWATER_H__
#define __SHALOWWATER_H__

#include <Vec3.h>
#include "Array2D.h"
#include "Array2D.tcc"

class ShallowWater 
{
public:
   ShallowWater();
   void init(const int DIMX, const int DIMY, const float _dt);
   void draw() const;
   void computeOneStep();
   Array2D<float> advect(const Array2D<float> & src);
   void updateHeight();
   void updateVelocities();
   void reflectingBoundaries();
   Array2D<math::Vec3f> computeNormals(const Array2D<float> & src) const;
   
protected:
   Array2D<float> m_g;     // height of the ground (0 if flat)
   Array2D<float> m_h;     // height of the water : the thing to compute and to draw
   Array2D<float> m_n;     // m_n = m_h - m_g : amount of water above the ground
   Array2D<float> m_vX;    // velocity along X   
   Array2D<float> m_vY;    // velocity along Y
   Array2D<math::Vec3f> m_g_normals; // normals for the ground

   float m_dt;
};

#endif