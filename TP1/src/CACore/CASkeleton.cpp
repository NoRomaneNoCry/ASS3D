
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

void CASkeleton::setPose(const BVH& bvh, const int & frameNumber) {

	m_joint[0].m_transLocal2world = math::Vec3f();
	m_joint[0].m_rotLocal2world = math::Quaternion();

	for(int jointNb = 1; jointNb < m_joint.size(); jointNb++) {

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
			translation = m_joint[fatherId].m_rotLocal2world * translation 
				+ m_joint[fatherId].m_transLocal2world;
		}

		m_joint[jointNb].m_transLocal2world = translation;
		m_joint[jointNb].m_rotLocal2world = rotation;
	}
}

void CASkeleton::drawGL() const {

	math::Vec3f translation;
	math::Vec3f axeRotation;
	float angleRotation;
	int fatherId;

	for(int i = 0; i < m_joint.size(); i++) {

		translation = m_joint[i].m_transLocal2world;
		if((fatherId = m_joint[i].m_fatherId) >= 0) {

			math::Vec3f coordsFather(m_joint[fatherId].m_transLocal2world);

			glBegin(GL_LINES);
			glVertex3f(coordsFather.x, coordsFather.y, coordsFather.z);
			glVertex3f(translation.x, translation.y, translation.z);
			glEnd();
		}

		glPushMatrix();
		glTranslatef(translation.x, translation.y, translation.z);

		draw_cube();

		glPopMatrix();
	}
}

float CASkeleton::distance(const CASkeleton& skel) const {

	float distance = 0.f;

	for(int i = 1; i < m_joint.size(); i++) {

		math::Vec3f difference = skel.m_joint[i].m_transLocal2world 
			- m_joint[i].m_transLocal2world;
		distance += difference.norm();
	}

	return distance;
}
