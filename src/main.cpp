//std
#include <cstdio>
#include <cstdlib>

//Dome
#include "Dome/inc/Engine.hpp"

//FEA
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

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
	FILE* file = fopen("plot.txt", "w");
	//save
	for(uint32_t i = 0; i < dome.analysis()->solver_static_nonlinear()->step(); i++)
	{
		const double ps = dome.analysis()->solver_static_nonlinear()->load_data(i);
		const double u1 = dome.mesh()->node(2 * ns)->state(fea::mesh::nodes::DOF::Translation_1, i);
		const double u2 = dome.mesh()->node(2 * ns)->state(fea::mesh::nodes::DOF::Translation_2, i);
		const double u3 = dome.mesh()->node(2 * ns)->state(fea::mesh::nodes::DOF::Translation_3, i);
		fprintf(file, "%+.6e %+.6e %+.6e %+.6e\n", u1, u2, u3, ps);
	}
	//close
	fclose(file);
}

//data
void print_limit_points(Dome& dome)
{
	//data
	const double* p = dome.analysis()->solver_static_nonlinear()->load_data();
	for(uint32_t i = 1; i + 1 < dome.analysis()->solver_static_nonlinear()->step(); i++)
	{
		//minimum
		if(p[i] < p[i - 1] && p[i] < p[i + 1])
		{
			printf("Limit(-) Step: %4d Load: %+.6e\n", i, p[i]);
		}
		//maximum
		if(p[i] > p[i - 1] && p[i] > p[i + 1])
		{
			printf("Limit(+) Step: %4d Load: %+.6e\n", i, p[i]);
		}
	}
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
	dome.load(0, 2, -1.00e+06);
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