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

	void update();
// domain is divided into cells of side 2h

	private:	
	float kernel(const Vector2f &r, const float hsml);
	Vector2f gradient_kernel(const Vector2f &r, const float hmsl);
	void add_density(Particle &particle, Particle &neighbour);
	void add_forces(Particle &particle, Particle &neighbour);
	void add_pressure(Particle& particle, Particle& neighbour);
//	void update_BC();//DBC, RBC, POBC
	void BcForce(int i, int j, Particle& fparticle, const list<Particle>& bcplist);

#endif 

