#include "CASpring.h"

CASpring::CASpring() : p1(NULL), p2(NULL), l0(0.f), k(0.f) {}

CASpring::CASpring(CAParticle* _p1, CAParticle* _p2, const float _l0, const float _k, const float _v) 
{ 
	set(_p1,_p2,_l0,_k, _v); 
}

void CASpring::set(CAParticle* _p1, CAParticle* _p2, const float _l0, const float _k, const float _v)
{
	p1 = _p1;
	p2 = _p2;
	l0 = _l0;
	k = _k;
	v = _v;
}
 
void CASpring::applyForceToParticles()
{
	float l( (p2->getPosition() - p1->getPosition()).norm());
	math::Vec3f n12( (p2->getPosition() - p1->getPosition()) / l);
	math::Vec3f n21( (p1->getPosition() - p2->getPosition()) / l);


	p1->addForce( (k * (l - l0)/l0 + v * (p2->getSpeed() - p1->getSpeed()) * n12 )* n12);
	p2->addForce( (k * (l - l0)/l0 + v * (p1->getSpeed() - p2->getSpeed()) * n21 )* n21);
}

CAParticle * CASpring::getParticle1() const {
	return p1;
}

CAParticle * CASpring::getParticle2() const {
	return p2;
}
