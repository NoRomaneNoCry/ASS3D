#ifndef _CASPRING_H
#define _CASPRING_H

#include <iostream>
#include <Vec3.h>

#include "CAParticle.h"

class CASpring
{
public:
   CASpring();
   CASpring(CAParticle* _p1, CAParticle* _p2, const float _l0, const float _k, const float _v);
   void set(CAParticle* _p1, CAParticle* _p2, const float _l0, const float _k, const float _v);
   void applyForceToParticles();

   CAParticle * getParticle1() const;
   CAParticle * getParticle2() const;

protected:
   CAParticle* p1;
   CAParticle* p2;
   float l0;
   float k;
   float v;
};

#endif