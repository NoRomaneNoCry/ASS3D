
#ifndef __CAJoint_H__
#define __CAJoint_H__

#include <string>
#include <vector>
#include <cassert>
#include <fstream>
#include <iostream>

#include <BVH.h>
#include <Mat4.h>

class CAJoint
{
public:
   int m_fatherId;        // Le numéro du père dans le tableau de CAJoint de CASkeleton
   math::Mat4f m_local2world;   // La matrice passant du repère de l'articulation vers le monde. 
                          // On peut aussi stocker un quaternion + une translation
   //math::Mat4f m_world2local;   // Si besoin : La matrice passant du repère du monde vers l'articulation

   CAJoint(const int & id);
};

#endif