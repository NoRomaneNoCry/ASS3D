
#include <Viewer.h>

#include <Vec3.h>

#ifndef _CAVIEWER_H
#define _CAVIEWER_H

class CAViewer : public Viewer
{
public:
	CAViewer() : Viewer(), isPhysics(false)
	{}

	virtual ~CAViewer ();
	virtual void draw();
	virtual void init();
	virtual void keyPressed(unsigned char key, int x, int y);

protected :

	bool isPhysics;
};

#endif
