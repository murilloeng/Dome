//std
#include <cstdio>
#include <cstdlib>

//Dome
#include "Dome/inc/Engine.hpp"

//draw
void draw(Dome& dome)
{
	//data
	Engine engine;
	engine.dome(&dome);
	engine.show_fps(false);
	//start
	engine.start();
}

//plot
void plot_static(Dome& dome)
{
	// //data
	// const uint32_t ns = dome.sides();
	// const uint32_t nl = dome.layers();
	// FILE* file = fopen("plot.txt", "w");
	// //save
	// for(uint32_t i = 0; i < dome.solver_static().m_step; i++)
	// {
	// 	const double u1 = dome.node(ns * nl).state(i, 0);
	// 	const double u2 = dome.node(ns * nl).state(i, 1);
	// 	const double u3 = dome.node(ns * nl).state(i, 2);
	// 	const double ps = dome.solver_static().m_p_data[i];
	// 	fprintf(file, "%+.6e %+.6e %+.6e %+.6e\n", u1, u2, u3, ps);
	// }
	// //close
	// fclose(file);
}

//data
void print_limit_points(Dome& dome)
{
	// //data
	// const uint32_t ns = dome.sides();
	// const uint32_t nl = dome.layers();
	// const double* u = dome.node(ns * nl).state();
	// const double* p = dome.solver_static().m_p_data;
	// for(uint32_t i = 1; i + 1 < dome.solver_static().m_step; i++)
	// {
	// 	//minimum
	// 	if(p[i] < p[i - 1] && p[i] < p[i + 1])
	// 	{
	// 		printf("Limit(-) Step: %4d Load: %+.6e Displacement: %+.6e\n", i, p[i], u[3 * i + 2]);
	// 	}
	// 	//maximum
	// 	if(p[i] > p[i - 1] && p[i] > p[i + 1])
	// 	{
	// 		printf("Limit(+) Step: %4d Load: %+.6e Displacement: %+.6e\n", i, p[i], u[3 * i + 2]);
	// 	}
	// }
}

//solve
void solve_static(Dome& dome)
{
	dome.solve_static();
}

int main(void)
{
	//data
	Dome dome;
	dome.sides(3);
	dome.height(0, 0.1);
	dome.height(1, 0.2);
	dome.load(0, 2, -1.00e+05);
	//solve
	solve_static(dome);
	//plot
	plot_static(dome);
	print_limit_points(dome);
	//draw
	draw(dome);
	//return
	return EXIT_SUCCESS;
}