#include "sph.h"

#define PI_FLOAT  3.141592653589793f
#define square(x)	  ((x)*(x))

inline float SphSolver :: kernel(const Vector2f &r, const float h)
{
	const float alpha = 2/PI_FLOAT/square(h);
	const float q = length(r)/h;
	if(0<= q && q <= 2)
		return alpha * (3./16 * square(q) - 3./4 * q + 3./4);
	 else
		return 0;
}

inline Vector2f SphSolver:: gradient_kernel(const Vector2f &r, const float h)
{
	const float alpha = 2/PI_FLOAT/square(h);
	const float q = length(r)/h;
	if(0<=q && q<= 2)
		return alpha*(3./8*q - 3./4)* r/ length(r);
	 else
		return Vector2f(0.0f);
}

inline void SphSolver :: add_density(Particle &particle, Particle &neighbour)
{
	if(particle.id > neighbour.id)	return ;
	Vector2f r=particle.position - neighbour.position;
	if(dot(r,r) > square(hsml)) return ;
	float common = kernel(r, hsml);
	particle.density += neighbour.mass*common;
	neighbour.density += particle.mass*common;
}

inline void SphSolver::add_force(Particle &particle, Particle &neighbour)
{
	if(particle.id >= neighbour.id) return;
	Vector2f r = particle.position - neighbour.position;
	Vector2f v = particle.velocity - neighbour.velocity;
	if(dot(r,r)> square(hsml)) return;
v/* compute pressrue force */
	Vector2f common = (neighbour.pressure / square(neighbour.density) + particle.pressure / square(particle.density)) * gradient_kernel(r, hsml);
	particle.force += -neighbour.mass*common;
	neighbour.force -= -particle.mass*common;

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

inline void SphSolver::add_pressure(Particle& particle, Particle& neighbour)
{
}

void SphSolver::update(Particle& particle, Particle& neighbour)
{
	add_density(particle, neighbour);
	add_force(particle, neighbour);
	add_pressure(particle, neighbour);
}

