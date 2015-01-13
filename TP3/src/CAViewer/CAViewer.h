#ifndef _CAVIEWER_H
#define _CAVIEWER_H

#include <Viewer.h>
#include "ShalowWater.h"

class CAViewer : public Viewer
{
public:
	CAViewer() : Viewer()
	{}

	virtual ~CAViewer ();
	virtual void animate();
	virtual void draw();
	virtual void init();
	virtual void keyPressed(unsigned char key, int x, int y);

protected :
	
	ShallowWater m_shallow;
};

#endif
