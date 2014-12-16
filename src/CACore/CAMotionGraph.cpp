
#include "CAMotionGraph.h"

CAGraphNode::CAGraphNode(const BVH_ID & bvhId, const int & frame) :
	m_idBVH(bvhId), m_frame(frame) {

	ids_next = std::vector<GraphNodeID>();
}

const CAGraphNode::BVH_ID CAGraphNode::getIdBVH() const {
	return m_idBVH;
}

const int CAGraphNode::getFrame() const {
	return m_frame;
}

void CAGraphNode::addNext(const GraphNodeID & nextId) {
	ids_next.push_back(nextId);
}

int CAGraphNode::getNumNext() const {
	return ids_next.size();
}

const CAGraphNode::GraphNodeID CAGraphNode::getNext(int index) const {
	return ids_next[index];
}

CAMotionGraph::CAMotionGraph() {

	m_BVH = std::vector<chara::BVH *>();
	m_graphNode = std::vector<CAGraphNode>();
}

void CAMotionGraph::addBVH(chara::BVH * bvh) {
	m_BVH.push_back(bvh);
}

int CAMotionGraph::getNumBVH() const {
	return m_BVH.size();
}

chara::BVH * CAMotionGraph::getBVH(int index) const {
	return m_BVH[index];
}