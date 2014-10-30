#include "sph.h"
#include "grid.h"
#include "object.h"

int main()
{

	double hsml(2.5e-5);
	double domain_width(1.0e-3);
	double domain_height(0.5e-3);
	int fp_count;
	int bp_count;
	int mbp_count;
	double dt(1.0e-4);
	double simulationtime(0.1);
	int currentstep(0);

	GridClass gridclass(hsml, domain_width, domain_height, fp_count, bp_count, mbp_count);
	SphSolver sphsolver(hsml);
	FloatingObject floatingbody(dt,currentstep);

	for(int t=1; t<= (int) simulationtime/dt ; t++)
	{
		if(t==1)
			gridclass.init_grid;
		else{
			gridclass.update_grid();
			gridclass.check_limit();
			gridclass.GlobalUpdate(); // so GridClass.GlobalUpdate actually only update fluid particles
			gridclass.updateFP();
		     }
			if( (t/10)==0)
			gridclass.WriteResult(); 
//actually,all FP, BP are in GridCells, so the result should be through GridCells
	}
}


