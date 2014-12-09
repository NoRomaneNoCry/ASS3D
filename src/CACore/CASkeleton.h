
#ifndef __CASkeleton_H__
#define __CASkeleton_H__

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <BVH.h>
#include <BVHJoint.h>
#include <CAJoint.h>

using namespace chara;

class CASkeleton
{
public:
   //! Cr�er un squelette ayant la m�me structure que d�finit dans le BVH
   CASkeleton(const BVH& bvh);

   void initialiseMJointRec(const BVHJoint& bvhj, const int & fatherId);

   //! Positionne ce squelette dans la position n du BVH
   void setPose(const BVH& bvh, const int & frameNumber);

   //! Calcule la distance entre deux postures
   //! precond: les deux squelettes doivent avoir le 
   //! m�me nombre d'articulations (m�me structure d'arbre)
   float distance(const CASkeleton& skel) const;

   //! Calcule la distance entre deux postures en prenant en compte le sens du
   // mouvement
   //! precond: les deux squelettes doivent avoir la m�me structure d'arbre
   float temporalDistance(const CASkeleton& skel) const;

   //! Affiche en OpenGL le squelette, les liens entre les articulations
   //! sont donn�s par le champ m_fatherId de CAJoint
   void drawGL() const;

private:
   //! L'ensemble des articulations.
   //! Remarque : la notion de hi�rarchie (arbre) n'est plus n�cessaire ici,
   //! pour tracer les os on utilise l'information "fatherID" de la class CAJoint
   std::vector<CAJoint> m_joint;
};

#endif //
