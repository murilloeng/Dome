//std
#include <cmath>
#include <cstdio>
#include <cstring>

//Dome
#include "Dome/inc/Dome.hpp"

//Math
#include "Math/inc/Linear/Eigen.hpp"
#include "Math/inc/Miscellaneous/util.hpp"
#include "Math/inc/Solvers/NewtonRaphson.hpp"

//shapes
[[maybe_unused]] static double shape_cone(double Hi, double Ht, double Rt)
{
	return Rt * (1 - Hi / Ht);
}
[[maybe_unused]] static double shape_ellipsoid(double Hi, double Ht, double Rt)
{
	return Rt * sqrt(1 - pow(Hi / Ht, 2));
}
[[maybe_unused]] static double shape_paraboloid(double Hi, double Ht, double Rt)
{
	return Rt * sqrt(1 - Hi / Ht);
}
[[maybe_unused]] static double shape_paraboloid_inversed(double Hi, double Ht, double Rt)
{
	return Rt * (1 - sqrt(Hi / Ht));
}

//twist
static double default_twist(double Hi, double Ht)
{
	return 0;
}

//constructor
Dome::Dome(void) : 
	m_flip{false}, m_solved{false}, m_height{1.00e+00}, m_radius{1.00e+00}, 
	m_sides{3}, m_layers{1}, m_twist{default_twist}, m_shape{shape_paraboloid}
{
	Node::m_dome = this;
	Loads::m_dome = this;
	Element::m_dome = this;
}

//destructor
Dome::~Dome(void)
{
	return;
}

//data
bool Dome::flip(bool flip)
{
	return m_flip = flip;
}
bool Dome::flip(void) const
{
	return m_flip;
}

double Dome::height(void) const
{
	return m_height;
}
double Dome::height(double height)
{
	return m_height = height;
}

double Dome::radius(void) const
{
	return m_radius;
}
double Dome::radius(double radius)
{
	return m_radius = radius;
}

uint32_t Dome::sides(void) const
{
	return m_sides;
}
uint32_t Dome::sides(uint32_t order)
{
	return m_sides = order;
}

uint32_t Dome::layers(void) const
{
	return m_layers;
}
uint32_t Dome::layers(uint32_t layers)
{
	return m_layers = layers;
}

uint32_t Dome::dof_unkown(void) const
{
	//data
	const uint32_t ns = m_sides;
	const uint32_t nl = m_layers;
	//return
	return 3 * (ns * (nl - 1) + 1);
}

Loads& Dome::loads(void)
{
	return m_loads;
}
const Loads& Dome::loads(void) const
{
	return m_loads;
}

Node& Dome::node(uint32_t index)
{
	return m_nodes[index];
}
const Node& Dome::node(uint32_t index) const
{
	return m_nodes[index];
}

Section& Dome::section(void)
{
	return m_section;
}
const Section& Dome::section(void) const
{
	return m_section;
}

Material& Dome::material(void)
{
	return m_material;
}
const Material& Dome::material(void) const
{
	return m_material;
}

Element& Dome::element(uint32_t index)
{
	return m_elements[index];
}
const Element& Dome::element(uint32_t index) const
{
	return m_elements[index];
}

std::vector<Node>& Dome::nodes(void)
{
	return m_nodes;
}
const std::vector<Node>& Dome::nodes(void) const
{
	return m_nodes;
}

std::vector<Element>& Dome::elements(void)
{
	return m_elements;
}
const std::vector<Element>& Dome::elements(void) const
{
	return m_elements;
}

math::solvers::NewtonRaphson& Dome::solver_static(void)
{
	return m_solver_static;
}


//info
double Dome::surface(void) const
{
	//data
	double A = 0;
	math::Vec3 x[4];
	const uint32_t ns = m_sides;
	const uint32_t nl = m_layers;
	//apply
	for(uint32_t i = 0; i < nl; i++)
	{
		for(uint32_t j = 0; j < ns; j++)
		{
			if(i + 1 != nl)
			{
				x[0] = m_nodes[(i + 0) * ns + (j + 0) % ns].m_position;
				x[1] = m_nodes[(i + 0) * ns + (j + 1) % ns].m_position;
				x[2] = m_nodes[(i + 1) * ns + (j + 1) % ns].m_position;
				x[3] = m_nodes[(i + 1) * ns + (j + 0) % ns].m_position;
				A += (x[1] - x[0]).cross(x[2] - x[0]).norm() / 2;
				A += (x[2] - x[0]).cross(x[3] - x[0]).norm() / 2;
			}
			else
			{
				x[2] = m_nodes[nl * ns].m_position;
				x[0] = m_nodes[(i + 0) * ns + (j + 0) % ns].m_position;
				x[1] = m_nodes[(i + 0) * ns + (j + 1) % ns].m_position;
				A += (x[1] - x[0]).cross(x[2] - x[0]).norm() / 2;
			}
		}
	}
	//return
	return A;
}

//save
void Dome::save(const char* path) const
{
	//open
	FILE* file = fopen(path, "w");
	//nodes
	fprintf(file, "%zd\n", m_nodes.size());
	for(const Node& node : m_nodes)
	{
		for(uint32_t i = 0; i < 3; i++)
		{
			fprintf(file, "%+.6e ", node.position(i));
		}
		fprintf(file, "\n");
	}
	//elements
	fprintf(file, "%zd\n", m_elements.size());
	for(const Element& element : m_elements)
	{
		for(uint32_t i = 0; i < 2; i++)
		{
			fprintf(file, "%d ", element.node(i));
		}
		fprintf(file, "\n");
	}
	//close
	fclose(file);
}

//loads
void Dome::apply_loads_vertical(void)
{
	
}

//analysis
void Dome::setup(void)
{
	setup_nodes();
	setup_elements();
}
void Dome::solve_modal(void)
{
	return;
}
void Dome::solve_static(void)
{
	//data
	const uint32_t nu = dof_unkown();
	const uint32_t ns = m_solver_static.m_step_max;
	//setup
	setup();
	m_solver_static.cleanup();
	m_solver_static.allocate(nu);
	//solver
	m_solver_static.m_p_new = 0;
	m_loads.apply(m_solver_static.m_fe);
	memset(m_solver_static.m_x_new, 0, nu * sizeof(double));
	//system
	for(Node& node : m_nodes) node.allocate(ns + 1, false);
	m_solver_static.m_system_1 = [this, nu](double* f, double* K, const double* x)
	{
		//setup
		memset(f, 0, nu * sizeof(double));
		memset(K, 0, nu * nu * sizeof(double));
		//assemble
		for(Element& element : m_elements)
		{
			element.apply(x);
			element.stiffness(K, x);
			element.internal_force(f, x);
		}
	};
	m_solver_static.m_update = [this](void){
		for(Element& element : m_elements) element.m_material_point.update();
	};
	m_solver_static.m_restore = [this](void){
		for(Element& element : m_elements) element.m_material_point.restore();
	};
	m_solver_static.m_record = [this](void){
		for(Node& node : m_nodes) node.record(m_solver_static.m_x_new, m_solver_static.m_step);
	};
	//solve
	m_solver_static.solve();
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

//kinematics
void Dome::setup_nodes(void)
{
	//data
	const double Ht = m_height;
	const double Rt = m_radius;
	const uint32_t ns = m_sides;
	const uint32_t nl = m_layers;
	const uint32_t nn = nl * ns + 1;
	const uint32_t nu = dof_unkown();
	//setup
	uint32_t du = 0;
	uint32_t dk = 0;
	m_nodes.resize(nn);
	for(uint32_t i = 0; i < nl; i++)
	{
		const double Hi = i * Ht / nl;
		const double ti = m_twist(Hi, Ht);
		const double Ri = m_shape(Hi, Ht, Rt);
		for(uint32_t j = 0; j < ns; j++)
		{
			m_nodes[i * ns + j].m_position[2] = Hi;
			m_nodes[i * ns + j].m_position[0] = Ri * cos(2 * M_PI * j / ns + ti);
			m_nodes[i * ns + j].m_position[1] = Ri * sin(2 * M_PI * j / ns + ti);
			for(uint32_t k = 0; k < 3; k++) m_nodes[i * ns + j].m_dof[k] = i == 0 ? nu + dk++ : du++;
		}
	}
	m_nodes[nl * ns].m_position[0] = 0;
	m_nodes[nl * ns].m_position[1] = 0;
	m_nodes[nl * ns].m_position[2] = Ht;
	for(uint32_t k = 0; k < 3; k++) m_nodes[nl * ns].m_dof[k] = du++;
}
void Dome::setup_elements(void)
{
	//data
	uint32_t counter = -1;
	const uint32_t ns = m_sides;
	const uint32_t nl = m_layers;
	const uint32_t ne = (3 * nl - 1) * ns;
	const double A = m_section.area();
	const double I = m_section.inertia();
	const double E = m_material.elastic_modulus();
	//setup
	m_elements.resize(ne);
	for(uint32_t i = 0; i < nl; i++)
	{
		for(uint32_t j = 0; j < ns; j++)
		{
			//borttom
			counter++;
			m_elements[counter].m_nodes[0] = (i + 0) * ns + (j + 0) % ns;
			m_elements[counter].m_nodes[1] = (i + 0) * ns + (j + 1) % ns;
			//vertical
			counter++;
			m_elements[counter].m_nodes[0] = (i + 0) * ns + (j + 0) % ns;
			m_elements[counter].m_nodes[1] = i + 1 != nl ? (i + 1) * ns + j : nl * ns;
			//diagonal
			if(i + 1 != nl)
			{
				counter++;
				const uint32_t isw = (i + 0) * ns + (j + 0) % ns;
				const uint32_t ise = (i + 0) * ns + (j + 1) % ns;
				const uint32_t inw = (i + 1) * ns + (j + 0) % ns;
				const uint32_t ine = (i + 1) * ns + (j + 1) % ns;
				m_elements[counter].m_nodes[0] = (!m_flip || i % 2 == 0) ? isw : ise;
				m_elements[counter].m_nodes[1] = (!m_flip || i % 2 == 0) ? ine : inw;
			}
		}
	}
	//buckling
	const double r = sqrt(I / A);
	for(Element& element : m_elements)
	{
		//data
		const math::Vec3 x1 = m_nodes[element.m_nodes[0]].m_position;
		const math::Vec3 x2 = m_nodes[element.m_nodes[1]].m_position;
		//buckling
		const double L = (x2 - x1).norm();
		element.m_material_point.m_buckling_stress = pow(M_PI * r / L, 2) * E;
	}
}