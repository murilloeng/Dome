//std
#include <cmath>
#include <cstdio>
#include <cstring>

//Dome
#include "Dome/inc/Dome.hpp"

//Math
#include "Math/inc/solvers/newton_raphson.hpp"

//static
static double default_twist(double Hi, double Ht)
{
	return 0;
}
static double default_shape(double Hi, double Ht, double Rt)
{
	return Rt * sqrt(1 - pow(Hi / Ht, 2));
}

//constructor
Dome::Dome(void) : 
	m_solved{false}, m_height{1.00e+00}, m_radius{1.00e+00}, m_sides{3}, m_layers{1}, m_twist{default_twist}, m_shape{default_shape}
{
	Node::m_dome = this;
	Load::m_dome = this;
	Element::m_dome = this;
}

//destructor
Dome::~Dome(void)
{
	return;
}

//data
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
	math::solvers::newton_raphson solver;
	//setup
	solver.cleanup();
	solver.allocate(nu);
	//solver
	solver.m_p_new = 0;
	solver.m_dp0 = 0.01;
	solver.m_watch_dof = nu - 1;
	solver.m_stop_criteria.m_x_min = -m_height;
	memset(solver.m_fe, 0, nu * sizeof(double));
	memset(solver.m_x_new, 0, nu * sizeof(double));
	solver.m_continuation.m_type = math::solvers::continuation::type::control_state;
	//system
	solver.m_system_1 = [this, nu](double* f, double* K, const double* x)
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
	//solve
	solver.solve();
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
	const uint32_t ns = m_sides;
	const uint32_t nl = m_layers;
	const uint32_t ne = 2 * nl * ns;
	//setup
	m_elements.resize(ne);
	for(uint32_t i = 0; i < nl; i++)
	{
		for(uint32_t j = 0; j < ns; j++)
		{
			//horizontal
			m_elements[2 * ns * i + j].m_nodes[0] = i * ns + (j + 0) % ns;
			m_elements[2 * ns * i + j].m_nodes[1] = i * ns + (j + 1) % ns;
			//vertical
			m_elements[2 * ns * i + ns + j].m_nodes[0] = i * ns + j;
			m_elements[2 * ns * i + ns + j].m_nodes[1] = i + 1 != nl ? (i + 1) * ns + j : nl * ns;
		}
	}
}