//Dome
#include "Dome/inc/Dome.hpp"
#include "Dome/inc/Load.hpp"
#include "Dome/inc/Node.hpp"

//constructor
Load::Load(uint32_t node, uint32_t dof, double value, time_function function) : m_value{value}, m_dof{dof}, m_node{node}, m_function{function}
{
	return;
}

//destructor
Load::~Load(void)
{
	return;
}

//data
double Load::value(void) const
{
	return m_value;
}
double Load::value(double value)
{
	return m_value = value;
}

uint32_t Load::dof(void) const
{
	return m_dof;
}
uint32_t Load::dof(uint32_t dof)
{
	return m_dof = dof;
}

uint32_t Load::node(void) const
{
	return m_node;
}
uint32_t Load::node(uint32_t node)
{
	return m_node = node;
}

time_function Load::function(void) const
{
	return m_function;
}
time_function Load::function(time_function function)
{
	return m_function = function;
}

//analysis
void Load::external_force(double* f) const
{
	//data
	const uint32_t nu = m_dome->dof_unkown();
	const uint32_t dof = m_dome->node(m_node).dof(m_dof);
	//assemble
	if(dof < nu) f[dof] += m_value;
}

//static data
Dome* Load::m_dome = nullptr;