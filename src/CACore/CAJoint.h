
#ifndef __CAJoint_H__
#define __CAJoint_H__

#include <math.h>
#include <Quaternion.h>

class CAJoint
{
public:
   int m_fatherId;

   math::Quaternion m_rotLocal2world;
   math::Vec3f m_transLocal2world;

   CAJoint(const int & id);
};

#endif