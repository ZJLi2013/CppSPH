#include "sph.h"
#include "object.h"

struct GridCell
{
	list<Particle> FP;  // fluid particles
	list<Particle> MBP; // moving boundary particles
	list<Particle> FBP; //fixed boundary particles
};

GridClass {

	const int grid_width;
	const int grid_height;

	GridCell* grid_cells;
	GridCell* tmp_grid_cells;

	SphSolver sphsolver;
	object	floatingbody;


	public:
	GridClass(	float hsml,
			float domain_width,
			float domain_height,
			float fp_count,
			float bp_count,
			float mbp_count)
		: grid_width( (int) (domain_width/core_radius) + 1),
		  grid_height( (int) (domain_height/core_radius) + 1),
		  core_radius (2*hsml),
		  fp_count(fp_count),
		  bp_count(bc_count),
		  mbp_count(mbp_count)
	{
	}

	void check_limit();
	void init_grid();
	void update_grid();
	void GlobalUpdate();
	
	void FixWallVector(); //I am not sure this is a good try
	

	private:
	void insert_into_grid(int i, int j);	
	GridCell &grid(int i, int j);
	GridCell &tmp_grid(int i, int j);
	int grid_index(int i, int j);
	void fp_add_to_grid(GridCell *target_grid, Particle &fparticle);
	void mbp_add_to_grid(GridCell *target_grid, Particle &mbparticle);
	void fbp_add_to_grid(GridCell *target_grid, Particle &fbparticle);
	void NeighbourCells(int i, int j);
	void Celij(int i, int j, Particle& particle);
	void UpdateParticleInCell(int i, int j, GridCell& grid_cell, Particle& particle);

};
