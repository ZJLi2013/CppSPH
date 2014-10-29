#include "sph.h"

#define root2 1.4142135623730951

void SphSolver::BoundaryForce(int i, int j, Particle& particle, const list<Particle>& bcplist)
{

	Vector2f wall_N, wall_T;
	if(i==0 && j==0) //bottom_left CP
	{
		wall_N.x = 1.0/root2; wall_N.z = 1.0/root2;
		wall_T.x = 1.0/root2; wall_T.z = -1.0/root2;
	}
	else if( i==0 && j != 0 && j != grid_width-1) //bottom
	{
		wall_N.x = 0.0; wall_N.z = 1.0;
		wall_T.x = 1.0; wall_T.z = 0.0;
	}
	else if(i==0 && j==grid_width-1) //bottom_right Cross Point
	{
		wall_N.x = -1.0/root2; wall_N.z = 1.0/root2;
		wall_T.x =  1.0/root2; wall_T.z = 1.0/root2;
	}
	else if(j==grid_width-1 && i != grid_height-1) //right 
	{
		wall_N.x = -1.0; wall_N.z = 0.0;
		wall_T.x = 0.0; wall_T.z = 1.0;
	}
	else if(j==grid_width-1 && i==grid_height-1) //top_right CP
	{
		wall_N.x = -1.0/root2; wall_N.z = -1.0/root2;
		wall_T.x = -1.0/root2; wall_T.z = 1.0/root2;
	}
	else if(i==grid_height-1 && j !=0) // top
	{
		wall_N.x = 0.0; wall_N.z = -1.0;
		wall_T.x = -1.0; wall_T.z = 0.0;
	}
	else if(i==grid_height-1 && j==0) //top_left CP
	{
		wall_N.x = 1.0/root2; wall_N.z = -1.0/root2;
		wall_T.x = -1.0/root2; wall_T.z = -1.0/root2;
	}
	else (j==0 && i != 0) //left
	{
		wall_N.x = 1.0; wall_N.z = 0.0;
		wall_T.x = 0.0; wall_T.z = -1.0;
	}

for(list<Particle>::iterator bcpiter= bcpist.begin(); bcpiter != bcplist.end(); bcpiter++)
{
		Vector2f bc_force(0.0f);
		Vector2f dr= particle.position - bcpiter->position;
//normal projection length(\psi) 
	float psi = dot(dr, wall_N); 
//tangential vector
	Vector2f tang_dr = dr - psi*wall_N;
//tangent projection length(\xi)
	float xi = dot(tang_dr, wall_T);

	float deltapp= hsml;
	float q = 0.0;

	if(psi>0.0)
		 q = psi/hsml/2; //paramter in R
	else if(psi<0.0)
		q = 0.2*fabs(psi)/hsml/2;
	else
		q = 0.5e-3; //correcting for zero distance from wall

	if(q <1.0 && psi > 0.0 && fabs(xi) < deltapp)
{   	
		Vector2f dv = bcpiter->velocity - particle.velocity;
		float u_normal = dot(dv, wall_N);
/* u_normal < 0 , particles are approaching
 * u_normal > 0 , particles are moving apart
 */	
		float App = 0.0;

		if(u_normal > 0.0)
			 App =( 0.01*vsp*vsp - vsp*u_normal)/hsml;
		else
			App = 0.01*vsp*vsp/hsml;

		float magnitude_force = 0.5*(1.0+ cos(2*PI*fabs(xi)/deltapp))*(1.0 - q)/sqrt(q)*App;

		 bc_force = wall_N*magnitude_force;
}       else
		 bc_force = Vector2f(0.0f);
	
	particle.force -=bc_force* bcpiter->mass; 
	index++;
	}
}

	         	


