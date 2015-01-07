#ifndef __CADYNAMICWORLD_H__
#define __CADYNAMICWORLD_H__

#include <CASpring.h>
#include <vector>

class CADynamicWorld
{
public:
   CADynamicWorld();
   void init(const int dimX, const int dimY);
   // à appeler depus la fonction animate de la class Viewer
   void computeOneStep(const float dt);                 
    // à appeler depuis la fonction draw de la class Viewer
   void draw() const;

   static const math::Vec3f m_gravity;

protected:
   std::vector<CAParticle> m_p;
   std::vector<CASpring> m_s;
};

#endif