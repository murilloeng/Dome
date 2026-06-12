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
	//data
	const uint32_t ns = dome.sides();
	const uint32_t nl = dome.layers();
	FILE* file = fopen("data.txt", "w");
	const double W = dome.loads().weight();
	//save
	for(uint32_t i = 0; i < dome.solver_static().m_step; i++)
	{
		const double u = dome.node(ns * nl).state(i, 2);
		const double p = dome.solver_static().m_p_data[i];
		fprintf(file, "%+.6e %+.6e\n", u, p * W);
	}
	//close
	fclose(file);
}

//data
void print_limit_points(Dome& dome)
{
	//data
	const uint32_t ns = dome.sides();
	const uint32_t nl = dome.layers();
	const double W = dome.loads().weight();
	const double* u = dome.node(ns * nl).state();
	const double* p = dome.solver_static().m_p_data;
	for(uint32_t i = 1; i + 1 < dome.solver_static().m_step; i++)
	{
		//minimum
		if(p[i] < p[i - 1] && p[i] < p[i + 1])
		{
			printf("Limit(-) Step: %4d Load: %+.6e Displacement: %+.6e\n", i, W * p[i], u[3 * i + 2]);
		}
		//maximum
		if(p[i] > p[i - 1] && p[i] > p[i + 1])
		{
			printf("Limit(+) Step: %4d Load: %+.6e Displacement: %+.6e\n", i, W * p[i], u[3 * i + 2]);
		}
	}
}

//solve
void solve_static(Dome& dome)
{
	//data
	const uint32_t nu = dome.dof_unkown();
	math::solvers::NewtonRaphson& solver = dome.solver_static();
	//setup
	solver.m_dp0 = 5.00e+01;
	solver.m_step_max = 2000;
	solver.m_watch_dof = nu - 1;
	solver.m_stop_criteria.m_x_min = -2 * dome.height();
	solver.m_continuation.m_type = math::solvers::Continuation::Type::MinimalNorm;
	solver.m_stop_criteria.m_types |= uint32_t(math::solvers::StopCriteria::Type::StateLimitMinimum);
	//solve
	dome.solve_static();
}

int main(void)
{
	//data
	Dome dome;
	const uint32_t ns = 4;
	const uint32_t nl = 1;
	//setup
	dome.sides(ns);
	dome.layers(nl);
	//solve
	solve_static(dome);
	printf("Weight: %+.6e\n", dome.loads().weight());
	//plot
	plot_static(dome);
	print_limit_points(dome);
	//return
	return EXIT_SUCCESS;
}