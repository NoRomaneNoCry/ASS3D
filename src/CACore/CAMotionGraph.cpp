
#include "CAMotionGraph.h"

#include <CASkeleton.h>

CAGraphNode::CAGraphNode(const BVH_ID & bvhId, const int & frame) :
	m_idBVH(bvhId), m_frame(frame) {

	m_idsNext = std::vector<GraphNodeID>();
	m_next = std::vector<CAGraphArc>();
}

int CAGraphNode::getIdBVH() const {
	return (int)m_idBVH;
}

int CAGraphNode::getFrame() const {
	return m_frame;
}

void CAGraphNode::addNext(const GraphNodeID & nextId) {
	m_idsNext.push_back(nextId);
}

int CAGraphNode::getNumNext() const {
	return m_idsNext.size();
}

int CAGraphNode::getNext(int index) const {
	return (int)m_idsNext[index];
}

CAMotionGraph::CAMotionGraph() : m_seuilCompatibilite(200.f) {

	m_BVH = std::vector<chara::BVH *>();
	m_graphNode = std::vector<CAGraphNode>();
}

void CAMotionGraph::addBVH(chara::BVH * bvh) {
	
	m_BVH.push_back(bvh);

	for(int frameNb = 0; frameNb < bvh->getNumFrame(); frameNb++) {
		
		CAGraphNode node = CAGraphNode(getNumBVH() - 1, frameNb);
		addNode(node);
		
		if(frameNb != 0)
			m_graphNode[frameNb-1].addNext(frameNb);

		// Calcul des distances aux autres noeuds 
		CASkeleton current_skel = CASkeleton(*bvh);
		current_skel.setPose(*bvh, frameNb);
		
		for(int i = 0; i < getNumGraphNode() - 1; i++) {

			CASkeleton other_skel = CASkeleton(*(m_BVH[m_graphNode[i].getIdBVH()]));
			other_skel.setPose(*(m_BVH[m_graphNode[i].getIdBVH()]), 
				m_graphNode[i].getFrame());


			if(current_skel.distance(other_skel) < m_seuilCompatibilite)
				m_graphNode[i].addNext(frameNb);
		}
	}
}

int CAMotionGraph::getNumBVH() const {
	return m_BVH.size();
}

const chara::BVH * CAMotionGraph::getBVH(int index) const {
	return m_BVH[index];
}

void CAMotionGraph::addNode(const CAGraphNode & node) {
	m_graphNode.push_back(node);
}

int CAMotionGraph::getNumGraphNode() const {
	return m_graphNode.size();
}

void CAMotionGraph::showMotionGraph() const {

	std::cout << "NodeID | BvhID | Next" << std::endl;
	for(int i =0; i < getNumGraphNode(); i++) {
		std::cout << i << " | " << m_graphNode[i].getIdBVH() << " | ";
		for(int j = 0; j < m_graphNode[i].getNumNext(); j++)
			std::cout << m_graphNode[i].getNext(j) << ", ";
		std::cout << std::endl;
	}
}