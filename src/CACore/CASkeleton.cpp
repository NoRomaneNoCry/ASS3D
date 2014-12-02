
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

	math::Mat4f local2father;
	math::Vec3f translation;

	for(int jointNb = 0; jointNb < m_joint.size(); jointNb++) {

		const BVHJoint & bvhj = *(bvh.getJoint(jointNb));
		bvhj.getOffset(translation.x, translation.y, translation.z);

		for(int i =0; i < bvhj.getNumChannel(); i++)
		{
			BVHChannel * bvhc = bvhj.getChannel(i);
			if(bvhc->isRotation()) {
				switch(bvhc->getAxis()) {
					case chara::AXIS_X :
						local2father.addRotationX(bvhc->getData(frameNumber));
						break;
					case chara::AXIS_Y :
						local2father.addRotationY(bvhc->getData(frameNumber));
						break;
					case chara::AXIS_Z :
						local2father.addRotationZ(bvhc->getData(frameNumber));
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

		local2father += translation;
		int fatherId = m_joint[jointNb].m_fatherId;
		if(fatherId >= 0)
			local2father *= m_joint[fatherId].m_local2world;

		m_joint[jointNb].m_local2world = local2father;
	}
}

void CASkeleton::drawGL() const {

	math::Vec3f translation;
	for(int i = 0; i < m_joint.size(); i++) {
		glPushMatrix();
		translation = m_joint[i].m_local2world.getTranslation();
		glTranslatef(translation.x, translation.y, translation.z);
		draw_cube();
		glPopMatrix();
	}
}
