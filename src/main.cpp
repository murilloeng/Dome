//std
#include <cstdlib>

//Dome
#include "Dome/inc/Engine.hpp"

void draw(Dome& dome)
{
	//data
	Engine engine;
	engine.dome(&dome);
	engine.show_fps(false);
	//start
	engine.start();
}
void solve_static(Dome& dome)
{
	//data
	const uint32_t nu = dome.dof_unkown();
	math::solvers::newton_raphson& solver = dome.solver_static();
	//setup
	solver.m_dp0 = 1.00e-04;
	solver.m_step_max = 1000;
	solver.m_watch_dof = nu - 1;
	solver.m_continuation.m_type = math::solvers::continuation::type::control_state;
	solver.m_stop_criteria.m_types |= uint32_t(math::solvers::stop_criteria::type::load_local_maximum);
	//solve
	dome.solve_static();
}

int main(void)
{
	//data
	Dome dome;
	const uint32_t ns = 20;
	const uint32_t nl = 20;
	//setup
	dome.sides(ns);
	dome.layers(nl);
	dome.loads().push_back(Load(ns * nl, 2, -1e8));
	//solve
	solve_static(dome);
	//draw
	draw(dome);
	//return
	return EXIT_SUCCESS;
}