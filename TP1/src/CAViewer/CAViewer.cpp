

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>

#include "CAViewer.h"
#include <BVH.h>
#include <BVHChannel.h>
#include <Mat4.h>
#include <Quaternion.h>
#include <CASkeleton.h>

#include "BVHJoint.h"

using namespace chara;
using namespace std;


void CAViewer::help()
{
	printf("Animation:\n");
	printf("   n: Next character pose\n");
	printf("   b: Back(Previous) character pose\n");
	printf("   6: Next node in motion graph\n");
	printf("   shift+arrows: Move the target point");
	Viewer::help();
}


CAViewer::~CAViewer()
{
}



void CAViewer::init()
{
	Viewer::init();

	std::string fn_front = "../data/danse.bvh";
	std::string test2 = "../data/Robot2.bvh";

	if (fn_front!="")
	{
		std::string current_file( fn_front );
		printf("%s\n", current_file.c_str());
		m_bvh = new BVH(current_file.c_str(), true );
		BVH * bvh2 = new BVH(test2.c_str(), true);

		cout<<"BVH"<<endl;
		cout<<*m_bvh<<endl;
		cout<<"------------"<<endl;

		m_motionGraph = new CAMotionGraph();
		m_motionGraph->addBVH(m_bvh);
		m_motionGraph->addBVH(bvh2);
		m_motionGraph->showMotionGraph();
	}
	else cout<<"No BVH\n";

    m_target.set( 10, 10, 0);
    m_nodeID = 0;
}



void CAViewer::draw()
{
    glColor3f(0, 0, 1);

    CAGraphNode node = m_motionGraph->getNode(m_nodeID);
    m_skel = new CASkeleton(*(m_motionGraph->getBVH(node.getIdBVH())));
    m_skel->setPose(*(m_motionGraph->getBVH(node.getIdBVH())), node.getFrame());
    m_skel->drawGL();
}



void CAViewer::keyPressed(unsigned char key, int x, int y)
{
	bool handled = false;
	if ((key=='n'))
	{
		++m_bvhFrame;
		//m_skel->setPostureFromBVH( *m_bvh, m_bvhFrame);
		handled = true;
	}
	else
	if ((key=='b'))
	{
		--m_bvhFrame;
		//m_skel->setPostureFromBVH( *m_bvh, m_bvhFrame);
		handled = true;
	}
	else
	if(key == '6')
	{
		std::cout << "Node ID " << m_nodeID << std::endl;
		if(m_nodeID < m_motionGraph->getNumGraphNode() - 1
			&& (m_motionGraph->getNode(m_nodeID)).getNumNext() > 0)
			m_nodeID = (m_motionGraph->getNode(m_nodeID)).getNext(
				rand() % (m_motionGraph->getNode(m_nodeID)).getNumNext());
		else
			std::cout << "Plus de nodes dans le graphe" << std::endl;
		handled = true;
	}
	if (key=='w')
	{
		bWireframe = !bWireframe;
		if (bWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		handled = true;
	}

	if (!handled)
	{
		Viewer::keyPressed(key,x,y);
	}
	updateGL();
}


void CAViewer::specialKeyPressed(int key, int x, int y)
{
	bool handled = false;

	if (glutGetModifiers()==GLUT_ACTIVE_SHIFT)
	{
		switch (key)
		{
		case GLUT_KEY_UP:
			m_target.y += 1;
			handled=true;
			break;
		case GLUT_KEY_DOWN:
			m_target.y -= 1;
			handled=true;
			break;
		case GLUT_KEY_LEFT:
			m_target.x -= 1;
			handled=true;
			break;
		case GLUT_KEY_RIGHT:
			m_target.x += 1;
			handled=true;
			break;
		case GLUT_KEY_PAGE_UP:
			m_target.z += 1;
			handled=true;
			break;
		case GLUT_KEY_PAGE_DOWN:
			m_target.z -= 1;
			handled=true;
			break;
		}
	}


	if (!handled)
	{
        Viewer::specialKeyPressed(key,x,y);
	}
	updateGL();
}



void CAViewer::animate()
{
	if (m_bvh)
	{
		++m_bvhFrame;
		if (m_bvhFrame>=m_bvh->getNumFrame()) m_bvhFrame=0;
		//m_skel->setPostureFromBVH( *m_bvh, m_bvhFrame);
	}
}

void CAViewer::bvhDrawGL(const chara::BVH& bvh, int frameNumber)
{
	bvhDrawGLRec(*(bvh.getRoot()), frameNumber);
}

void CAViewer::bvhDrawGLRec(const chara::BVHJoint& bvhj, int frameNumber)
{
	glPushMatrix();
	float offsetx, offsety, offsetz;
	bvhj.getOffset(offsetx, offsety, offsetz);

	// Dessin de l'os
	glBegin(GL_LINES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(offsetx, offsety, offsetz);
	glEnd();

	// Dessin de l'articulation
	glTranslatef(offsetx, offsety, offsetz);

	for(int i =0; i < bvhj.getNumChannel(); i++)
	{
		chara::BVHChannel * bvhc = bvhj.getChannel(i);
		if(bvhc->isRotation()) {
			switch(bvhc->getAxis()) {
				case chara::AXIS_X :
					glRotatef(bvhc->getData(frameNumber), 1, 0, 0);
					break;
				case chara::AXIS_Y :
					glRotatef(bvhc->getData(frameNumber), 0, 1, 0);
					break;
				case chara::AXIS_Z :
					glRotatef(bvhc->getData(frameNumber), 0, 0, 1);
					break;
				default : break;
			}
		}
		else {
			switch(bvhc->getAxis()) {
				case chara::AXIS_X :
					glTranslatef(bvhc->getData(frameNumber), 0, 0);
					break;
				case chara::AXIS_Y :
					glTranslatef(0, bvhc->getData(frameNumber), 0);
					break;
				case chara::AXIS_Z :
					glTranslatef(0, 0, bvhc->getData(frameNumber));
					break;
				default : break;
			}
		}
	}
	draw_cube();

	for(int i = 0; i < bvhj.getNumChild(); i++)
		bvhDrawGLRec(*(bvhj.getChild(i)), frameNumber);

	glPopMatrix();
}

void CAViewer::bvhTransitionDrawGL(const chara::BVH& bvhSRC, int frameNumberSRC,
		const chara::BVH& bvhDST, int frameNumberDST, const float interpolationValue)
{	
	bvhTransitionDrawGLRec(*(bvhSRC.getRoot()), frameNumberSRC, *(bvhDST.getRoot()), 
		frameNumberDST, interpolationValue);
}

void CAViewer::bvhTransitionDrawGLRec(const chara::BVHJoint& bvhjSRC, int frameNumberSRC, 
		const chara::BVHJoint& bvhjDST, int frameNumberDST, 
		const float interpolationValue)
{
	glPushMatrix();

	math::Quaternion Qsrc;
	math::Quaternion Qdst;
	math::Quaternion Qinterpol;

	math::Vec3f translationSRC;
	math::Vec3f translationDST;
	math::Vec3f axeRotation;
	float offsetx, offsety, offsetz, angle;
	chara::BVHChannel * bvhcSRC;
	chara::BVHChannel * bvhcDST;

	bvhjSRC.getOffset(offsetx, offsety, offsetz);

	// Dessin de l'os
	glBegin(GL_LINES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(offsetx, offsety, offsetz);
	glEnd();

	// Dessin de l'articulation
	glTranslatef(offsetx, offsety, offsetz);

	for(int i =0; i < bvhjSRC.getNumChannel(); i++)
	{
		bvhcSRC = bvhjSRC.getChannel(i);
		bvhcDST = bvhjDST.getChannel(i);
		if(bvhcSRC->isRotation()) {
			switch(bvhcSRC->getAxis()) {
				case chara::AXIS_X :
					Qsrc *= math::Quaternion(math::Vec3f(1.f, 0.f, 0.f), bvhcSRC->getData(frameNumberSRC) * M_PI / 180);
					Qdst *= math::Quaternion(math::Vec3f(1.f, 0.f, 0.f), bvhcDST->getData(frameNumberDST) * M_PI / 180);
					break;
				case chara::AXIS_Y :
					Qsrc *= math::Quaternion(math::Vec3f(0.f, 1.f, 0.f), bvhcSRC->getData(frameNumberSRC) * M_PI / 180);
					Qdst *= math::Quaternion(math::Vec3f(0.f, 1.f, 0.f), bvhcDST->getData(frameNumberDST) * M_PI / 180);
					break;
				case chara::AXIS_Z :
					Qsrc *= math::Quaternion(math::Vec3f(0.f, 0.f, 1.f), bvhcSRC->getData(frameNumberSRC) * M_PI / 180);
					Qdst *= math::Quaternion(math::Vec3f(0.f, 0.f, 1.f), bvhcDST->getData(frameNumberDST) * M_PI / 180);
					break;
				default : break;
			}
		}
		else {
			switch(bvhcSRC->getAxis()) {
				case chara::AXIS_X :
					translationSRC.x = bvhcSRC->getData(frameNumberSRC);
					translationDST.x = bvhcDST->getData(frameNumberDST);
					break;
				case chara::AXIS_Y :
					translationSRC.y = bvhcSRC->getData(frameNumberSRC);
					translationDST.y = bvhcDST->getData(frameNumberDST);
					break;
				case chara::AXIS_Z :
					translationSRC.z = bvhcSRC->getData(frameNumberSRC);
					translationDST.z = bvhcDST->getData(frameNumberDST);
					break;
				default : break;
			}
		}
	}
	Qinterpol = math::Quaternion::slerp(Qsrc, Qdst, interpolationValue);
	Qinterpol.getAxisAngle(axeRotation, angle);
	glTranslatef(interpolationValue * translationSRC.x + (1 - interpolationValue) * translationDST.x,
		interpolationValue * translationSRC.y + (1 - interpolationValue) * translationDST.y,
		interpolationValue * translationSRC.z + (1 - interpolationValue) * translationDST.z);

	glRotatef(angle * 180 / M_PI, axeRotation.x, axeRotation.y, axeRotation.z);

	draw_cube();

	for(int i = 0; i < bvhjSRC.getNumChild(); i++)
		bvhTransitionDrawGLRec(*(bvhjSRC.getChild(i)), frameNumberSRC, *(bvhjDST.getChild(i)),
			frameNumberDST, interpolationValue);

	glPopMatrix();
}
