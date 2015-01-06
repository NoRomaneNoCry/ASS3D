#ifndef _CASPRING_H
#define _CASPRING_H

#include <iostream>
#include <Vec3.h>

#include "CAParticle.h"

class CASpring
{
public:
   CASpring();
   CASpring(CAParticle* _p1, CAParticle* _p2, const float _l0, const float _k);
   void set(CAParticle* _p1, CAParticle* _p2, const float _l0, const float _k);
   void applyForceToParticles();
protected:
   CAParticle* p1;
   CAParticle* p2;
   float l0;
   float k;
};

#endif