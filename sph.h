#include<list>
using namespace std;

class SphSolver;
struct Particle;

#ifndef _SPH_H_
#define _SPH_H_

#include "Vector.h"
#include "grid.h"

struct Particle
{
	int id;
	int style; //fluid particle/ boundary particle
	int flag; //out of domain or not
	float mass;
	float density;
	Vector2f position;
	Vector2f velocity;
	Vector2f force;
	Vector2f viscosity;

	Particle() :
		flag(0);
		mass(0.625e-6),
		density(1000.0),
		pressure(0.1)
	{
	}
};

class SphSolver{
	private:
		const float hsml;
		const float timestep;
		const float vsp; //vel of sound speed
		GridCell *grid_cells;

	public:
		SphSolver(float hsml, float timestep)
			:hsml(hsml),
			 timestep(timestep),
			 vsp(10.0)
	{
	}
// domain is divided into cells of side 2h
	void step();
	void init_particles();

/*	template <typename Function>
		void foreach_particle(Function function){
			for(int j=0; j<grid_height; j++){
				for(int i=0; i<grid_width;i++){
					GridCell &grid_cell = grid_cells[ grid_width*j + i];
					list<Partcile> &plist = grid_cell.ParticlesInCell;
					for(list<Particle>::iterator piter = plist.begin(); piter != plist.end(); piter++)
					{
						function(*piter);
					}
				}
			}
		}
*/
	private:	
	float kernel(const Vector2f &r, const float hsml);
	Vector2f gradient_kernel(const Vector2f &r, const float hmsl);
	void add_density(Particle &particle, Particle &neighbour);
	void sum_density(GridCell &grid_cell, Particle &particle);
	void sum_all_density(int i, int j, Particle &particle);
	void update_densities(int i, int j);
	void add_forces(Particle &particle, Particle &neighbour);
	void sum_forces(GridElement &grid_element, Particle &particle);
	void sum_all_forces(int i, int j, Particle &particle);
	void update_forces(int i, int j);
	void update_particle(Particle &particle);
	void update_particles(int i, int j);
	void insert_into_grid(int i, int j);
	void update_densities();
	void update_forces();
	void update_particles();
	
	void update_EoS(); //pressure
	void update_BC();//DBC, RBC, POBC

};

#endif 

