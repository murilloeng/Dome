//Domes
#include "Domes/inc/Element.hpp"

//constructor
Element::Element(void) : m_nodes{0, 0}
{
	return;
}

//destructor
Element::~Element(void)
{
	return;
}

//data
uint32_t Element::node(uint32_t index) const
{
	return m_nodes[index];
}
uint32_t Element::node(uint32_t index, uint32_t node)
{
	return m_nodes[index] = node;
}

//formulation
void Element::inertia(double* M) const
{
	return;
}
void Element::stiffness(double* K) const
{
	return;
}
void Element::internal_force(double* f) const
{
	return;
}