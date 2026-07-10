//std
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>

//Sections
#include "Sections/inc/CHS.hpp"

//Materials
#include "Materials/inc/Mechanic/Steel.hpp"

//Dome
#include "Dome/inc/Dome.hpp"

//constructor
Dome::Dome(void) : 
	m_sides{3}, m_loads{0, 0, 0, 0, 0, 0}, m_height{+5.00e-01, 1.00e+00}, m_radius{1.00e+00, +5.00e-01}, 
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

//solve
void Dome::solve_modal(void)
{
	return;
}
void Dome::solve_static(void)
{
	// //data
	// const uint32_t nu = dof_unkown();
	// const uint32_t ns = m_solver_static.step_max();
	// //setup
	// m_solver_static.cleanup();
	// m_solver_static.allocate(nu);
	// //solver
	// memset(m_solver_static.m_fe, 0, nu * sizeof(double));
	// memset(m_solver_static.state_new(), 0, nu * sizeof(double));
	// //system
	// for(Node& node : m_nodes) node.allocate(ns + 1, false);
	// for(const Node& node : m_nodes) node.assemble(m_solver_static.m_fe);
	// m_solver_static.m_system_1 = [this, nu](double* f, double* K, const double* x)
	// {
	// 	//setup
	// 	memset(f, 0, nu * sizeof(double));
	// 	memset(K, 0, nu * nu * sizeof(double));
	// 	//assemble
	// 	for(Element& element : m_elements)
	// 	{
	// 		element.apply(x);
	// 		element.stiffness(K, x);
	// 		element.internal_force(f, x);
	// 	}
	// };
	// m_solver_static.m_update = [this](void){
	// 	for(Element& element : m_elements) element.m_material_point.update();
	// };
	// m_solver_static.m_restore = [this](void){
	// 	for(Element& element : m_elements) element.m_material_point.restore();
	// };
	// m_solver_static.m_record = [this](void){
	// 	for(Node& node : m_nodes) node.record(m_solver_static.m_x_new, m_solver_static.m_step);
	// };
	// //solve
	// m_solver_static.solve();
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
	setup_nodes();
	setup_elements();
}
void Dome::setup_nodes(void)
{
	// //data
	// const double Ht = m_height;
	// const double Rt = m_radius;
	// const uint32_t ns = m_sides;
	// const uint32_t nl = m_layers;
	// const uint32_t nn = nl * ns + 1;
	// const uint32_t nu = dof_unkown();
	// //setup
	// uint32_t du = 0;
	// uint32_t dk = 0;
	// m_nodes.resize(nn);
	// for(uint32_t i = 0; i < nl; i++)
	// {
	// 	const double Hi = i * Ht / nl;
	// 	const double ti = m_twist(Hi, Ht);
	// 	const double Ri = m_shape(Hi, Ht, Rt);
	// 	for(uint32_t j = 0; j < ns; j++)
	// 	{
	// 		m_nodes[i * ns + j].m_position[2] = Hi;
	// 		m_nodes[i * ns + j].m_position[0] = Ri * cos(2 * M_PI * j / ns + ti);
	// 		m_nodes[i * ns + j].m_position[1] = Ri * sin(2 * M_PI * j / ns + ti);
	// 		for(uint32_t k = 0; k < 3; k++) m_nodes[i * ns + j].m_dof[k] = i == 0 ? nu + dk++ : du++;
	// 	}
	// }
	// m_nodes[nl * ns].m_position[0] = 0;
	// m_nodes[nl * ns].m_position[1] = 0;
	// m_nodes[nl * ns].m_position[2] = Ht;
	// for(uint32_t k = 0; k < 3; k++) m_nodes[nl * ns].m_dof[k] = du++;
}
void Dome::setup_elements(void)
{
	// //data
	// uint32_t counter = -1;
	// const uint32_t ns = m_sides;
	// const uint32_t nl = m_layers;
	// const uint32_t ne = (3 * nl - 1) * ns;
	// const double A = m_section.area();
	// const double I = m_section.inertia();
	// const double E = m_material.elastic_modulus();
	// //setup
	// m_elements.resize(ne);
	// for(uint32_t i = 0; i < nl; i++)
	// {
	// 	for(uint32_t j = 0; j < ns; j++)
	// 	{
	// 		//borttom
	// 		counter++;
	// 		m_elements[counter].m_nodes[0] = (i + 0) * ns + (j + 0) % ns;
	// 		m_elements[counter].m_nodes[1] = (i + 0) * ns + (j + 1) % ns;
	// 		//vertical
	// 		counter++;
	// 		m_elements[counter].m_nodes[0] = (i + 0) * ns + (j + 0) % ns;
	// 		m_elements[counter].m_nodes[1] = i + 1 != nl ? (i + 1) * ns + j : nl * ns;
	// 		//diagonal
	// 		if(i + 1 != nl)
	// 		{
	// 			counter++;
	// 			const uint32_t isw = (i + 0) * ns + (j + 0) % ns;
	// 			const uint32_t ise = (i + 0) * ns + (j + 1) % ns;
	// 			const uint32_t inw = (i + 1) * ns + (j + 0) % ns;
	// 			const uint32_t ine = (i + 1) * ns + (j + 1) % ns;
	// 			m_elements[counter].m_nodes[0] = (!m_flip || i % 2 == 0) ? isw : ise;
	// 			m_elements[counter].m_nodes[1] = (!m_flip || i % 2 == 0) ? ine : inw;
	// 		}
	// 	}
	// }
	// //buckling
	// const double r = sqrt(I / A);
	// for(Element& element : m_elements)
	// {
	// 	//data
	// 	const math::Vec3 x1 = m_nodes[element.m_nodes[0]].m_position;
	// 	const math::Vec3 x2 = m_nodes[element.m_nodes[1]].m_position;
	// 	//buckling
	// 	const double L = (x2 - x1).norm();
	// 	element.m_material_point.m_buckling_stress = pow(M_PI * r / L, 2) * E;
	// }
}