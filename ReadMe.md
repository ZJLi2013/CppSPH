This is a sph solver derived from SPHysics, but implemented by Cpp.

The reason of this project is focusing on computation efficiency, basically including two things:

1 space dividing algorithm with link list
2 limit check algorithm

Others will be similar as SPHFEM, but with more implements on  Sympletic time-stepping scheme, Dynamic B.C., Periodic B.C., Moving Objects<both pre-imposed and fluid driven)

There are three classes: Particle, Object, SphSystem 

Oct 27

there are three kinds of particles in moving objects application(fluid, movingBC, fixedBC)
so the first thing, is to dividing space with cells and adding all three kinds of particles into cells. the fixedBC only need adding once, the other two needs update every time-step. This is done by GridClass. Interfaces of GridClass includes "generate grid, adding_to_grid, return grid_number, return grid_cell_reference/pointer"

in each time-step, call SphSolver to update fluid particles(FP), and call ObjectSolver to update FBC

there are some global variables for both SphSolver and ObjectSolver;

Oct 28
Adding fixed B.C. Wall normal/tang vectors. The vectors can be easily obtained from Celij. 

The problem is about moving objects wall normal vectors, which should be updaded every timestep
