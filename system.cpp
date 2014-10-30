#include "system.h"

void SphSystem::steps()
{
	for(int t=1; t<=timesteps; t++)
	{
		if(t==1)
			gridclass.init_grid;
		else{
			single_step();
			if( (t/10)==0)
			gridclass.WriteResult(); 
//actually,all FP, BP are in GridCells, so the result should be through GridCells
		}
	}
}

void SphSystem::single_step()
{
	gridclass.update_grid();
	gridclass.check_limit();
	gridclass.GlobalUpdate(); // so GridClass.GlobalUpdate actually only update fluid particles
	gridclass.updateFP();
}

