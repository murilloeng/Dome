//std
#include <cmath>

//Domes
#include "Domes/inc/Dome.hpp"

//static
static double default_twist(double Hk, double Ht)
{
	return 0;
}
static double default_shape(double Hk, double Ht, double Rt)
{
	return Rt * sqrt(1 - pow(Hk / Ht, 2));
}

//constructor
Dome::Dome(void) : 
	m_height{1.00e+00}, m_radius{1.00e+00}, m_sides{3}, m_layers{2}, 
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

//kinematics
void Dome::setup_nodes(void)
{
	// //data
	// const double Ht = m_height;
	// const double Rt = m_radius;
	// const uint32_t ns = m_sides;
	// const uint32_t nl = m_layers;
	// const uint32_t nn = (nl - 1) * ns + 1;
	// //setup
	// delete[] m_nodes;
	// m_nodes = new Node[nn];
	// //positions
	// for(uint32_t i = 0; i < nn; i++)
	// {
	// 	/* code */
	// }
	// const uint32_t index_t = index ? (index - 1) % m_sides : 0;
	// const uint32_t index_H = index ? (index - 1) / m_sides + 1 : 0;
	// //geometry
	// const double Hk = Ht - index_H * Ht / m_layers;
	// const double tk = 2 * M_PI * index_t / m_sides + m_twist(Hk, Ht);
	// //return
	// const double Rk = m_shape(Hk, Ht, Rt);
	// return {Rk * cos(tk), Rk * sin(tk), m_height};
}
void Dome::setup_elements(void)
{
	return;
}