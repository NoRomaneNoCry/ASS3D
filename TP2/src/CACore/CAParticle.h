#ifndef __CAPARTICLE_H__
#define __CAPARTICLE_H__

#include <Vec3.h>

class CAParticle
{
public:
	/*! Default constructor */
	CAParticle();
	/*! Constructor with a mass and an initial position */
	CAParticle(const float _mass, const math::Vec3f _p);
	
	void addForce(const math::Vec3f& af);

	/*! Update the speed and the postion
	  !param dt : the interval between the last position and the one calculated
	  */
	void computeOneStep(const float dt);        

	math::Vec3f getPosition() const;
	math::Vec3f getSpeed() const;

protected:
	float mass;
	math::Vec3f p;
	math::Vec3f v;
	math::Vec3f f;
};

#endif