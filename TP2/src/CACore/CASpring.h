#ifndef _CASPRING_H
#define _CASPRING_H

#include <iostream>
#include <Vec3.h>

#include "CAParticle.h"

/*
class CAParticle
{
public:
	CAParticle(const float _mass, const math::Vec3f _p) : mass(_mass), p(_p), v(0,0,0), f(0,0,0) {}
	~CAParticle();
	void addForce(const math::Vec3f& af){}
	void computeOneStep(const float dt){}
	math::Vec3f getPosition() {return p;}
protected:
   float mass;
   math::Vec3f p;
   math::Vec3f v;
   math::Vec3f f;
};*/

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