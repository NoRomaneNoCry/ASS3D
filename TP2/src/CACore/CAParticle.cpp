
#include "CAParticle.h"

CAParticle::CAParticle() : mass(0.0), p(0,0,0), v(0,0,0), f(0,0,0)
{}

CAParticle::CAParticle(const float _mass, const math::Vec3f _p) : 
	mass(_mass), p(_p), v(0,0,0), f(0,0,0) 
{}

void CAParticle::addForce(const math::Vec3f & af) {
	f += af;
}

void CAParticle::computeOneStep(const float dt) {

	/* Mise à jour de la vitesse */
	v = v + dt * (f/mass);

	/* Mise à jour de la position */
	p = p + dt * v;

	/* Force à 0 */
	f = math::Vec3f(0,0,0);
}

math::Vec3f CAParticle::getPosition() const {
	return p;
}

math::Vec3f CAParticle::getSpeed() const {
	return v;
}
