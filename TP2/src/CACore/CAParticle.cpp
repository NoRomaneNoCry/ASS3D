
#include "CAParticle.h"

CAParticle::CAParticle(const float _mass, const math::Vec3f _p) : 
	mass(_mass), p(_p), v(0,0,0), f(0,0,0) 
{}

void CAParticle::addForce(const math::Vec3f & af) {
	f = af;
}

void CAParticle::computeOneStep(const float dt) {

	/* Mise à jour de la vitesse */
	v = v + dt * (f/mass);

	/* Mise à jour de la position */
	p = p + dt * v;
}