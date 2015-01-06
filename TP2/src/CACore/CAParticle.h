#ifndef __CAPARTICLE_H__
#define __CAPARTICLE_H__

#include <Vec3.h>

class CAParticle
{
public:
   CAParticle(const float _mass, const math::Vec3f _p);
   void addForce(const math::Vec3f& af);
   void computeOneStep(const float dt);        // mise à jour de la position 
   // et de la vitesse en fonction de f et de dt

protected:
   float mass;
   math::Vec3f p;
   math::Vec3f v;
   math::Vec3f f;
};

#endif