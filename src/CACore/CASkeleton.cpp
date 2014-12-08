
#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "CASkeleton.h"

#include <CAViewer.h>

using namespace chara;

CASkeleton::CASkeleton(const BVH& bvh)
{
	initialiseMJointRec(*bvh.getRoot(), -1);
}

void CASkeleton::initialiseMJointRec(const BVHJoint& bvhj, const int & fatherId)
{
	m_joint.push_back(CAJoint(fatherId));
	
	int id = m_joint.size() - 1;
	for(int i = 0; i < bvhj.getNumChild(); i++) {
		initialiseMJointRec(*(bvhj.getChild(i)), id);
	}
}

void CASkeleton::setPose(const BVH& bvh, const int frameNumber) {

	for(int jointNb = 0; jointNb < m_joint.size(); jointNb++) {

		math::Quaternion rotation;
		math::Vec3f translation;

		const BVHJoint & bvhj = *(bvh.getJoint(jointNb));
		bvhj.getOffset(translation.x, translation.y, translation.z);

		for(int i =0; i < bvhj.getNumChannel(); i++)
		{
			BVHChannel * bvhc = bvhj.getChannel(i);
			if(bvhc->isRotation()) {
				switch(bvhc->getAxis()) {
					case chara::AXIS_X :
						rotation *= math::Quaternion(math::Vec3f(1.f, 0.f, 0.f), 
							bvhc->getData(frameNumber) * M_PI / 180);
						break;
					case chara::AXIS_Y :
						rotation *= math::Quaternion(math::Vec3f(0.f, 1.f, 0.f), 
							bvhc->getData(frameNumber) * M_PI / 180);
						break;
					case chara::AXIS_Z :
						rotation *= math::Quaternion(math::Vec3f(0.f, 0.f, 1.f), 
							bvhc->getData(frameNumber) * M_PI / 180);
						break;
					default : break;
				}
			}
			else {
				switch(bvhc->getAxis()) {
					case chara::AXIS_X :
						translation.x += bvhc->getData(frameNumber);
						break;
					case chara::AXIS_Y :
						translation.y += bvhc->getData(frameNumber);
						break;
					case chara::AXIS_Z :
						translation.z += bvhc->getData(frameNumber);
						break;
					default : break;
				}
			}
		}

		int fatherId = m_joint[jointNb].m_fatherId;
		if(fatherId >= 0) {
			rotation = m_joint[fatherId].m_rotLocal2world * rotation;
			translation = m_joint[fatherId].m_rotLocal2world * translation + m_joint[fatherId].m_transLocal2world;
		}

		m_joint[jointNb].m_transLocal2world = translation;
		m_joint[jointNb].m_rotLocal2world = rotation;
	}
}

void CASkeleton::drawGL() const {

	math::Vec3f translation;
	math::Vec3f axeRotation;
	float angleRotation;

	for(int i = 0; i < m_joint.size(); i++) {

		glPushMatrix();

		translation = m_joint[i].m_transLocal2world;
		glTranslatef(translation.x, translation.y, translation.z);

		m_joint[i].m_rotLocal2world.getAxisAngle(axeRotation, angleRotation);
		glRotatef(angleRotation * 180 / M_PI, axeRotation.x, axeRotation.y,
			axeRotation.z);

		draw_cube();

		glPopMatrix();
	}
}
