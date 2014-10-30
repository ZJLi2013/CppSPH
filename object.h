#include "sph.h"
#include "grid.h"

class FloatingObject
{
	Vector2f VelObject;   // moving velocity
	double starttime; 
	double currenttime;

	public:
	FloatingObject(double TS, 
		   	double currenttime)
		: starttime(10*TS),
		  currenttime(currenttime)
	{
	}

	void RigidMoving(list<Particle>& mbp);
	void FluidDrivenMoving(list<Particle>& mbp);
	
	private:
	void RigidMoving(list<Particle>& mbp, double currenttime)
	void FluidDrivenMoving(list<Particle>& mbp, double currenttime)
		//in this fluid_driven_moving, MBP treated same as a BP, and it will collect force from surround FP and MBP, and then act on mass center, then update vel and position
};	
			

