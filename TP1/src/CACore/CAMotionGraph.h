#ifndef __CAMotionGraph_H__
#define __CAMotionGraph_H__

#include <vector>
#include <BVH.h>
#include <Quaternion.h>
#include <Vec3.h>

//! Un noeud du graphe d'animation est repéré par un identifiant
typedef int GraphNodeID;

// TODO : créer structure arc pour gestion du centre de gravité
class CAGraphArc
{
public:
	
private:

	//! Identifiant du noeud du graphe vers lequel on se dirige
	GraphNodeID m_idDest;

	math::Quaternion m_gravityCenterRot;
	math::Vec3f m_gravityCenterTrans;
};

class CAGraphNode
{
public:

	//! Une animation BVH est repérée par identifiant
	typedef int BVH_ID;

	//! Constructeur comprenant l'identifiant du BVH et le numéro de la frame
	CAGraphNode(const BVH_ID & bvhId, const int & frame);

	int getIdBVH() const;
	int getFrame() const;

	//! Ajoute l'identifiant d'un noeud à la liste des successeurs
	void addNext(const GraphNodeID & nextId);
	int getNumNext() const;
	int getNext(int index) const;

private:
	//! Identifiant de l'animation
	BVH_ID m_idBVH;
	//! Numéro de la frame
	int m_frame;
	//! Liste des noeuds successeurs
	std::vector<GraphNodeID> m_idsNext;

	//! Liste des arcs qui permettent d'aller aux noeuds successeurs en gérant
	// la rotation et la translation du centre de gravité
	std::vector<CAGraphArc> m_next;
};

class CAMotionGraph
{
public:
	CAMotionGraph();
	void addBVH(chara::BVH * bvh);
	int getNumBVH() const;
	const chara::BVH * getBVH(int index) const;

	void addNode(const CAGraphNode & node);
	CAGraphNode & getNode(GraphNodeID index);
	int getNumGraphNode() const;

	void showMotionGraph() const;

private:
	//! L'ensemble des BVH du graphe d'animation
	std::vector<chara::BVH *> m_BVH;

	//! Tous les noeuds du graphe d'animation
	std::vector<CAGraphNode> m_graphNode;

	float m_seuilCompatibilite;
};

#endif
