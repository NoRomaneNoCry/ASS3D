#include "CAJoint.h"

CAJoint::CAJoint(const int & id, const math::Mat4f & matrice): m_fatherId(id), m_local2world(matrice)
{
}