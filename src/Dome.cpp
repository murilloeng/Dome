//std
#include <cmath>
#include <cstdio>

//Domes
#include "Domes/inc/Dome.hpp"

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
	m_height{1.00e+00}, m_radius{1.00e+00}, m_sides{3}, m_layers{1}, 
	m_nodes{nullptr}, m_elements{nullptr}, m_twist{default_twist}, m_shape{default_shape}
{
	return;
}

//destructor
Dome::~Dome(void)
{
	delete[] m_nodes;
	delete[] m_elements;
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

//save
void Dome::save(const char* path) const
{
	//data
	const uint32_t ns = m_sides;
	const uint32_t nl = m_layers;
	const uint32_t nn = nl * ns + 1;
	//save
	FILE* file = fopen(path, "w");
	fprintf(file, "Nodes: %d\n", nn);
	for(size_t i = 0; i < nn; i++)
	{
		for(uint32_t j = 0; j < 3; j++)
		{
			fprintf(file, "%+.6e ", m_nodes[i].position(j));
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
	return;
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
	//setup
	delete[] m_nodes;
	m_nodes = new Node[nn];
	for(uint32_t i = 0; i < nl; i++)
	{
		const double Hi = i * Ht / nl;
		const double ti = m_twist(Hi, Ht);
		const double Ri = m_shape(Hi, Ht, Rt);
		for(uint32_t j = 0; j < ns; j++)
		{
			m_nodes[i * ns + j].position(2, Hi);
			m_nodes[i * ns + j].position(0, Ri * cos(2 * M_PI * j / ns + ti));
			m_nodes[i * ns + j].position(1, Ri * sin(2 * M_PI * j / ns + ti));
		}
	}
	m_nodes[nl * ns].position(0, 0);
	m_nodes[nl * ns].position(1, 0);
	m_nodes[nl * ns].position(2, Ht);
}
void Dome::setup_elements(void)
{
	return;
}