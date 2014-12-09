#ifndef __CAMotionGraph_H__
#define __CAMotionGraph_H__

#include <vector>

#include <BVH.h>

class CAGrapheNode
{
public:
	CAGrapheNode(const BVH_ID & gnId, const int & frame);
	void addNext(const GrapheNodeID & nextId);

private:

	//! Un noeud du graphe d'animation est repéré par un identifiant
	typedef int GrapheNodeID;

	//! Une animation BVH est repérée par identifiant
	typedef int BVH_ID;

	//! Identifiant de l'animation
	BVH_ID m_idBVH;
	//! Numéro de la frame
	int m_frame;
	//! Liste des noeuds successeurs
	std::vector<GrapheNodeID> ids_next;
};

// TODO : créer structure arc pour gestion du centre de gravité
class CAMotionGraph
{

private:

	//! L'ensemble des BVH du graphe d'animation
	std::vector<BVH> m_BVH;

	//! Tous les noeuds du graphe d'animation
	std::vector<CAGrapheNode> m_grapheNode;
};

#endif
