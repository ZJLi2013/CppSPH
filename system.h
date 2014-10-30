#include "sph.h"
#include "grid.h"
#include "object.h"

#ifndef _system_h_
#define _system_h_


Class SphSystem
{
	public:
	GridClass  gridclass;
	SphSolver sphsolver;
	FloatingObject floatingbody;

	SphSystem();
	void steps();
	void single_step();
};


#endif
