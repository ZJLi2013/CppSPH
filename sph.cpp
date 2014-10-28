#include "sph.h"


#define PI_FLOAT  3.141592653589793f
#define square(x)	  ((x)*(x))

inline float SphFluidSolver :: kernel(const Vector2f &r, const float h)
{
	const float alpha = 2/PI_FLOAT/square(h);
	const float q = length(r)/h;
	if(0<= q && q <= 2)
		return alpha * (3./16 * square(q) - 3./4 * q + 3./4);
	 else
		return 0;
}

inline Vector2f SphFluidSolver:: gradient_kernel(const Vector2f &r, const float h)
{
	const float alpha = 2/PI_FLOAT/square(h);
	const float q = length(r)/h;
	if(0<=q && q<= 2)
		return alpha*(3./8*q - 3./4)* r/ length(r);
	 else
		return Vector2f(0.0f);
}

inline void SphFluidSolver :: add_density(Particle &particle, Particle &neighbour)
{
	if(particle.id > neighbour.id)	return ;
	Vector2f r=particle.position - neighbour.position;
	if(dot(r,r) > square(hsml)) return ;
	float common = kernel(r, hsml);
	particle.density += neighbour.mass*common;
	neighbour.density += particle.mass*common;
}

void SphSolver::sum_density(GridCell &grid_cell, Particle &particle)
{
	list<Particle> &plist = grid_cell.ParticlesInCell;
	for(list<Particle>::iterator piter = plist.begin(); piter != plist.end(); piter++){
		add_density(particle, *piter);
	}
}

inline void SphSolver::sum_all_density(int i, int j, Particle& particle)
{
	for(int y=j-1; y<=j+1; y++){
		for(int x=i-1; x<=i+1; x++){
			if( (x<0) || (x >= grid_width)
			   || (y<0) || (y >= grid_height)){
				continue;
			}
			sum_density(grid(x,y), particle);
		}
	}
}

void SphSolver::update_densities(int i, int j)
{
	// function grid(int i, int j)
	GridElement &grid_element = grid(i,j);
	list<Particle> &plist = grid_element.ParticlesInCell;
	for(list<Particle>::iterator piter=plist.begin(); piter != plist.end(); piter++)
	{
		sum_all_density(i,j,*piter);
	}
}

inline void SphFluidSolver::add_force(Particle &particle, Particle &neighbour)
{
	if(particle.id >= neighbour.id) return;
	Vector2f r = particle.position - neighbour.position;
	Vector2f v = particle.velocity - neighbour.velocity;
	if(dot(r,r)> square(hsml)) return;
v/* compute pressrue force */
	Vector2f common = (neighbour.pressure / square(neighbour.density) + particle.pressure / square(particle.density)) * gradient_kernel(r, hsml);
	particle.force += -neighbour.mass*common;
//	particle.pressure += -neighbour.mass*common.x; 
// wether pressure is scalar or vector?
	neighbour.force -= -particle.mass*common;
//	neighbour.pressure -= -particle.mass*common.x;

/*compute viscosity force by artificial viscosity given by Monaghan 1992 */
        float const alpha = 0.1 ;
    	float const eta2 = 0.01 * square(hsml);
 	float mu = hsml*dot(v,r)/(dot(r,r) + eta2);
	float rhobar =(particle.density + neighbour.density)/2;

	float pi;
	if (dot(v,r) < 0.0)
		 pi = - alpha* vsp * mu / rhobar;
	else
		 pi = 0.0;

	common = pi*gradient_kernel(r,hsml);
	particle.force += -neighbour.mass * common;
	particle.viscosity += -neighbour.mass*common;
	neighbour.force -= -particle.mass*common;
	neighbour.viscosity -= -particle.mass*common;
	
/*gravity is not based on particle collection */
}

void SphSolver::sum_forces(GridCell &grid_cell, Particle &particle)
{
	list<Particle> &plist = grid_cell.ParticlesInCell;
	for(list<Particle>::iterator piter=plist.begin(); piter != plist.end(); piter++)
	{
		add_force(particle, *piter);
	}
}

void SphSolver::sum_all_forces(int i, int j, Particle &particle)
{
	for(int y=j-1; y<=j+1; y++){
		for(int x=i-1; i<=i+1; x++){
			if( (x<0) || (x>grid_width) ||
			    (y<0) || (y>grid_height))
			{
				continue;
			}
			sum_forces(grid(x,y), particle);
		}
	}
}

void SphSolver::update_forces(int i, int j)
{
	GridElement &grid_element=grid(i,j);
	list<Particle>& plist=grid_element.ParticlesInCell;
       for(list<Particle>::iterator piter = plist.begin(); piter != plist.end(); piter++)
       {
		sum_all_forces(i,j,*piter);
       }
}

void SphFluidSolver::update_particle(Particle &particle)
{	
	
	particle.velocity += timestep * particle.force;
	particle.position += timestep * particle.velocity;
}

void SphSolver::update_particles(int i, int j)
{
	GridCell &grid_cell = grid(i,j);
	list<Particle>& plist = grid_cell.ParticlesInCell;
	for(list<Particle>::iterator piter = plist.begin(); piter != plist.end(); piter++)
	{
		update_particle(*piter);
	}
}

inline void SphSolver::insert_into_grid(int i, int j)
{
	GridCell &grid_cell = grid(i,j);
	list<Particle>& plist = grid_cell.ParticlesInCell;
	for(list<Particle>::iterator piter = plist.begin(); piter != plist.end(); piter++)
	{
		add_to_grid(grid_elements; *piter);
	}
}

void SphSolver::update_grid()
{
	for(int j=0; j<grid_height; j++){
		for(int i=0; i<grid_width; i++){
			insert_into_grid(i,j);
			grid(i,j).ParticlesInCell.clear(); //>>> what a fuck
		}
	}
}

//ALL THESE updates can be used in OPENMP
void SphSolver::update_densities()
{
	for(int j=0; j<grid_height; j++){
		for( int i=0; i<grid_width; i++){
			update_densities(i,j);
		}
	}
}

void SphSolver::update_forces()
{
	for(int j=0; j<grid_height; j++){
		for(int i=0; i<grid_width; i++){
			update_forces(i,j);
		}
	}
}

void SphSolver::update_particles()
{
	for(int j=0; j<grid_height; j++){
		for(int i=0; i<grid_width; i++)
		{
			update_particles(i,j):
		}
	}
}

void SphSolver::update()
{
	update_densities();
	update_forces();
	update_particles();
	update_grid();
}

void SphSolver::init_particles(Particle* particles, int count)
{
	grid_element = new GridCel[grid_width * grid_height];

	for(int x=0; x<count; x++)
	{
		particles[x].id = x;
		add_to_grid(grid_elements, particles[x]);
	}
}

inline GridCell& SphSolver::grid(int i, int j)
{
	return grid_cells[grid_index(i,j)];
}

inline SphSolver::grid_index(int i, int j){
	return grid_widht*j + i;
}

inline void SphSolver::add_to_grid(GridCell* target_grid, Particle &particle)
{
	int i= (int) (particle.position.x/hsml/2);
	int j= (int) (particle.position.z/hsml/2);
	target_grid[grid_index(i,j)].particles.push_back(particle);
}


