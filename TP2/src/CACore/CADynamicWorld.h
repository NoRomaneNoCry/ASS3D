#ifndef __CADYNAMICWORLD_H__
#define __CADYNAMICWORLD_H__

#include <CASpring.h>
#include <vector>

class CADynamicWorld
{
public:
   CADynamicWorld();
   void init(const int dimX, const int dimY);
   void computeOneStep(const float dt);                 
   void draw() const;

   static const math::Vec3f m_gravity;

protected:
   std::vector<CAParticle> m_p;
   std::vector<CASpring> m_s;
   math::Vec3f m_centre_sphere;
   float m_rayon_sphere;
   int m_dimX;
   int m_dimY;

   void manageGroundCollisions(float coef_rebond);
   void manageSphereCollisions();
};

float distance3D(math::Vec3f a, math::Vec3f b);

#endif