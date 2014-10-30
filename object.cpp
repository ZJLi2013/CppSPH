#include "object.h"
#include <math.h>

void FloatingObject::RigidMoving(mbp)
	{
		RigidMoving( mbp, currenttime);
	}
	
void FloatingObject::RigidMoving(mbp,currenttime)
	{
		if( double currenttime >= starttime)
		{
			VelObject.x = 0.0;
			VelObject.z = sin(2*pi*currenttime)*HSML/TS;  // which should satisfy vel*ts <= hsml
			for(list<Particle>::iterator mbpiter = mbpartilces.begin(); mbpiter != mbparticles.end(); mbpiter++)
			{
				mbpiter->position += VelObject*TS;
				mbpiter->velocity = VelObject;
			}
		}
	}

void FloatingObject::FluidDrivenMoving()
	{
		//in this way, MBP treated same as a BP, and it will collect force from surround FP and MBP, and then act on mass center, then update vel and position
	}
			

