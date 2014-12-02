#include "CAJoint.h"

CAJoint::CAJoint(const int & id): m_fatherId(id)
{
	m_local2world = math::Mat4f();
}
