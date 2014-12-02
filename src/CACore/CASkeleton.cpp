#include "CASkeleton.h"

using namespace chara;
using namespace std;

CASkeleton::CASkeleton(const BVH& bvh)
{
	//BVHJoint& bvhj = *bvh.getRoot();

	initialiseMJointRec(*bvh.getRoot(), -1);
}

void CASkeleton::initialiseMJointRec(const BVHJoint& bvhj, const int & fatherId)
{
	math::Mat4f local2father;
	
	for(int i = 0; i < bvhj.getNumChild(); i++) {
		initialiseMJointRec(*(bvhj.getChild(i)), fatherId+1);
	}
}