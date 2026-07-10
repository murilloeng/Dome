//std
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>

//FEA
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/Node.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Sections
#include "Sections/inc/CHS.hpp"

//Materials
#include "Materials/inc/Mechanic/Steel.hpp"

//Dome
#include "Dome/inc/Dome.hpp"

//constructor
Dome::Dome(void) : 
	m_sides{3}, m_twist{0}, m_loads{0, 0, 0, 0, 0, 0},
	m_height{+5.00e-01, 1.00e+00}, m_radius{1.00e+00, +5.00e-01},
	m_section{new sections::CHS}, m_material{new materials::Steel}
{
	return;
}

//destructor
Dome::~Dome(void)
{
	delete m_section;
	delete m_material;
}

//data
uint32_t Dome::sides(void) const
{
	return m_sides;
}
uint32_t Dome::sides(uint32_t sides)
{
	return m_sides = sides;
}

double Dome::twist(void) const
{
	return m_twist;
}
double Dome::twist(double twist)
{
	return m_twist = twist;
}

double Dome::height(uint32_t index) const
{
	return m_height[index];
}
double Dome::height(uint32_t index, double height)
{
	return m_height[index] = height;
}

double Dome::radius(uint32_t index) const
{
	return m_radius[index];
}
double Dome::radius(uint32_t index, double radius)
{
	return m_radius[index] = radius;
}

double Dome::load(uint32_t layer, uint32_t index) const
{
	return m_loads[3 * layer + index];
}
double Dome::load(uint32_t layer, uint32_t index, double load)
{
	return m_loads[3 * layer + index] = load;
}

sections::Section* Dome::section(void) const
{
	return m_section;
}

materials::Mechanic* Dome::material(void) const
{
	return m_material;
}

//print
void Dome::print(void) const
{
	print_loads();
	print_nodes();
	print_elements();
	print_supports();
}

//solve
void Dome::solve_modal(void)
{
	return;
}
void Dome::solve_static(void)
{
	//setup
	setup_model();
	m_section->compute();
	analysis()->create_solver(fea::analysis::Type::StaticNonlinear);
	//solver
	analysis()->solver()->load_combination(0);
	analysis()->solver()->watch_dof().node(2 * m_sides);
	analysis()->solver()->watch_dof().dof(fea::mesh::nodes::DOF::Translation_3);
	dynamic_cast<fea::analysis::StaticNonlinear*>(analysis()->solver())->step_max(1000);
	//solve
	solve();
	//save
	analysis()->solver()->save("Static Nonlinear.txt");
}
void Dome::solve_dynamic(void)
{
	return;
}
void Dome::solve_buckling(void)
{
	return;
}
void Dome::solve_harmonic(void)
{
	return;
}

//setup
void Dome::setup_model(void)
{
	setup_loads();
	setup_nodes();
	setup_elements();
	setup_supports();
}
void Dome::setup_loads(void)
{
	//setup
	boundary()->create_load_case();
	boundary()->create_load_combination(0, false, 1);
	//loads
	for(uint32_t i = 0; i < m_sides; i++)
	{
		boundary()->load_case(0)->create_load_node(2 * i + 1, fea::mesh::nodes::DOF::Translation_1, m_loads[0] / m_sides);
		boundary()->load_case(0)->create_load_node(2 * i + 1, fea::mesh::nodes::DOF::Translation_2, m_loads[1] / m_sides);
		boundary()->load_case(0)->create_load_node(2 * i + 1, fea::mesh::nodes::DOF::Translation_3, m_loads[2] / m_sides);
	}
	boundary()->load_case(0)->create_load_node(2 * m_sides, fea::mesh::nodes::DOF::Translation_1, m_loads[3]);
	boundary()->load_case(0)->create_load_node(2 * m_sides, fea::mesh::nodes::DOF::Translation_2, m_loads[4]);
	boundary()->load_case(0)->create_load_node(2 * m_sides, fea::mesh::nodes::DOF::Translation_3, m_loads[5]);
}
void Dome::setup_nodes(void)
{
	for(uint32_t i = 0; i < m_sides; i++)
	{
		const double a = m_twist;
		const double H = m_height[0];
		const double R0 = m_radius[0];
		const double R1 = m_radius[1];
		const double t = 2 * M_PI * i / m_sides;
		mesh()->create_node(R0 * cos(t), R0 * sin(t), 0);
		mesh()->create_node(R1 * cos(t + a), R1 * sin(t + a), H);
	}
	mesh()->create_node(0, 0, m_height[1]);
}
void Dome::setup_elements(void)
{
	for(uint32_t i = 0; i < m_sides; i++)
	{
		const uint32_t a = (i + 0) % m_sides;
		const uint32_t b = (i + 1) % m_sides;
		mesh()->create_element(fea::mesh::elements::Type::Truss3D, {2 * a + 1, 2 * b + 1});
		mesh()->create_element(fea::mesh::elements::Type::Truss3D, {2 * a + 0, 2 * a + 1});
		mesh()->create_element(fea::mesh::elements::Type::Truss3D, {2 * a + 0, 2 * b + 1});
		mesh()->create_element(fea::mesh::elements::Type::Truss3D, {2 * a + 1, 2 * m_sides});
	}
	for(fea::mesh::elements::Element* element : mesh()->elements())
	{
		((fea::mesh::elements::Truss3D*) element)->section(m_section);
		((fea::mesh::elements::Truss3D*) element)->material(m_material);
	}
}
void Dome::setup_supports(void)
{
	for(uint32_t i = 0; i < m_sides; i++)
	{
		boundary()->create_support(2 * i + 0, fea::mesh::nodes::DOF::Translation_1);
		boundary()->create_support(2 * i + 0, fea::mesh::nodes::DOF::Translation_2);
		boundary()->create_support(2 * i + 0, fea::mesh::nodes::DOF::Translation_3);
	}
}

//print
void Dome::print_loads(void) const
{
	printf("Loads:\n");
	for(const fea::boundary::loads::Node* load : boundary()->load_case(0)->loads_nodes())
	{
		printf("Node: %2d Dof: %d Value: %+.2e\n", load->node(), (uint32_t) load->dof(), load->value());
	}
}
void Dome::print_nodes(void) const
{
	printf("Nodes:\n");
	for(const fea::mesh::nodes::Node* node : mesh()->nodes())
	{
		const double* x = node->position_ref();
		printf("Index: %2d x1: %+.2e x2: %+.2e x3: %+.2e\n", node->index(), x[0], x[1], x[2]);
	}
}
void Dome::print_elements(void) const
{
	printf("Elements:\n");
	for(const fea::mesh::elements::Element* element : mesh()->elements())
	{
		printf("Index: %2d Nodes: %2d %2d\n", element->index(), element->index_node(0), element->index_node(1));
	}
}
void Dome::print_supports(void) const
{
	printf("Supports:\n");
	for(const fea::boundary::Support* support : boundary()->supports())
	{
		printf("Node: %2d Dof: %d\n", support->index_node(), (uint32_t) support->dof());
	}
}