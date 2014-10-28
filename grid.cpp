#include "sph.h"


void GridClass::check_limit();
	{	
		for(list<Particle>::iterator fpiter=fplist.begin(); fpiter!=fplist.end(); fpiter++)
		{
			if( (fpiter->position.x < min_box.x) || (fpiter->position.x > max_box.x) || (fpiter->position.z < min_box.z) || (fpiter->position.z > max_box.z))
			{	piter->flag = 1; // the next step will not update
				std::cout << "Fluid Particle " << piter->id << " is out of domain" << endl;	
			}
		}

		for(list<Particle>::iterator mbpiter = mbplist.begin(); mbpiter != mbplist.end(); mbpiter++)
		{
			if( (mbpiter->position.x < min_box.x) || (mbpiter->position.x > max_box.x) || (mbpiter->position.z < min_box.z) || (mbpiter->position.z > max_box.z))
		      {
			mbpiter->flag = 1;
			std::cout << "Moving B.C. Particle " << mbpiter->id << " is out of domain" << endl;	
		       }
	        }
		
void GridClass::init_grid();
	{
		grid_cells = new GridCell[grid_width * grid_height];
		tmp_grid_cells = new GridCell[grid_width * grid_height];

		for(int x=0; x<fp_count; x++){
			FP[x].id = x;
			fp_add_to_grid(grid_cells,FP[x]);
		}
//only do one time
		for(int x=0; x<fbp_count; x++){
			FBP[x].id = x;
			fbp_add_to_grid(grid_cells, FBP[x]);
		}

		for(int x=0; x<mbp_count; x++){
			MBP[x].id = x;
			mbp_add_to_grid(grid_cells, MBP[x]);
		}
	}

void GridClass::insert_into_grid(int i, int j);
	{
		GridCell &grid_cell = grid(i,j);
		list<Particle>& fplist = grid_cell.FP;
		list<Particle>& mbplist = grid_cell.MBP;
		for(list<Particle>::iterator fpiter=fplist.begin(); fpiter!=fplist.end(); fpiter++)
		{
			fp_add_to_grid(tmp_grid_elements, *piter);
		}

		for(list<Particle>::iterator mbpiter = mbplist.begin(); mbpiter != mbplist.end(); mbpiter++){
			mbp_add_to_grid(tmp_grid_elements, *mbpiter);
		}
	}

void GridClass::update_grid();
	{
		//update divide for every timestep
		for(int j=0; j<grid_height; j++)
			for(int i=0; i<grid_width; i++)
			{
				insert_into_grid(i,j);
			}
		swap(grid_cells, tmp_grid_cells);
	}

GridCell& GridClass::grid(int i, int j);
	{
		return grid_elements[grid_index(i,j)];
	}

GridCell& GridClass::tmp_grid(int i, int j);
	{
		return tmp_grid_elements[grid_index(i,j)];
	}

int GridClass::grid_index(int i, int j);
	{
		return grid_width*j + i;
	}

void GridClass::fp_add_to_grid(GridCell *target_grid, Particle &fparticle);
	{
		int i = (int) (fparticle.position.x/core_radius);
		int j = (int) (fparticle.position.z/core_radius);
			
		target_grid[grid_index(i,j)].FP.push_back(fparticle);
	}
	
void GridClass::mbp_add_to_grid(GridCell *target_grid, Particle &mbparticle);
	{
		int i = (int) (mbparticle.position.x/core_radius);
		int j = (int) (mbparticle.position.z/core_radius);
			
		target_grid[grid_index(i,j)].MBP.push_back(mbparticle);
	}
	
void GridClass::fbp_add_to_grid(GridCell *target_grid, Particle &fbparticle);
	{
		int i = (int) (fbparticle.position.x/core_radius);
		int j = (int) (fbparticle.position.z/core_radius);
			
		target_grid[grid_index(i,j)].FBP.push_back(fbparticle);
	}


