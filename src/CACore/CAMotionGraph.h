#ifndef __CAMotionGraph_H__
#define __CAMotionGraph_H__

#include <vector>
#include <BVH.h>

#define SEUIL_COMPATIBILITE 100.f

class CAGraphNode
{
public:
	//! Un noeud du graphe d'animation est repéré par un identifiant
	typedef int GraphNodeID;

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
	std::vector<GraphNodeID> ids_next;
};

// TODO : créer structure arc pour gestion du centre de gravité

class CAMotionGraph
{
public:
	CAMotionGraph();
	void addBVH(chara::BVH * bvh);
	int getNumBVH() const;
	const chara::BVH * getBVH(int index) const;

	void addNode(const CAGraphNode & node);
	int getNumGraphNode() const;

	void showMotionGraph() const;

private:
	//! L'ensemble des BVH du graphe d'animation
	std::vector<chara::BVH *> m_BVH;

	//! Tous les noeuds du graphe d'animation
	std::vector<CAGraphNode> m_graphNode;
};

#endif
