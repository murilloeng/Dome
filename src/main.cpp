//std
#include <cstdlib>

//Dome
#include "Dome/inc/Engine.hpp"

//data
const double Fr = 1.00e+08;

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
	const uint32_t steps = dome.solver_static().m_step;
	//save
	for(uint32_t i = 0; i < steps; i++)
	{
		const double u = dome.node(ns * nl).state(i, 2);
		const double p = dome.solver_static().m_p_data[i];
		fprintf(file, "%+.6e %+.6e\n", u, Fr * p);
	}
	//close
	fclose(file);
}

//solve
void solve_static(Dome& dome)
{
	//data
	const uint32_t nu = dome.dof_unkown();
	math::solvers::NewtonRaphson& solver = dome.solver_static();
	//setup
	solver.m_dp0 = 1.00e-04;
	solver.m_step_max = 2000;
	solver.m_watch_dof = nu - 1;
	solver.m_continuation.m_type = math::solvers::Continuation::Type::StateControl;
	solver.m_stop_criteria.m_types |= uint32_t(math::solvers::StopCriteria::Type::LoadValueNegative);
	//solve
	dome.solve_static();
}

int main(void)
{
	//data
	Dome dome;
	const uint32_t ns = 6;
	const uint32_t nl = 3;
	//setup
	dome.sides(ns);
	dome.layers(nl);
	dome.loads().push_back(Load(ns * nl, 2, -Fr));
	//solve
	solve_static(dome);
	//plot
	plot_static(dome);
	//draw
	draw(dome);
	//return
	return EXIT_SUCCESS;
}