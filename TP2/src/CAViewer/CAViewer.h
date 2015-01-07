
#include <Viewer.h>

#include <Vec3.h>
#include <CADynamicWorld.h>

#ifndef _CAVIEWER_H
#define _CAVIEWER_H

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

	CADynamicWorld m_world;
};

#endif
