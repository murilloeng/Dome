//Dome
#include "Dome/inc/Dome.hpp"
#include "Dome/inc/Node.hpp"

//constructor
Node::Node(void) : m_dof{0, 0, 0}, m_position{0, 0, 0}
{
	return;
}

//destructor
Node::~Node(void)
{
	return;
}

//data
uint32_t Node::dof(uint32_t index) const
{
	return m_dof[index];
}
const uint32_t* Node::dof(void) const
{
	return m_dof;
}

double Node::position(uint32_t index) const
{
	return m_position[index];
}
double Node::position(uint32_t index, double position)
{
	return m_position[index] = position;
}
const double* Node::position(void) const
{
	return m_position;
}
math::vec3 Node::position(const double* x) const
{
	//data
	const uint32_t nu = m_dome->dof_unkown();
	//return
	return {
		m_position[0] + m_dof[0] < nu ? x[m_dof[0]] : 0,
		m_position[1] + m_dof[1] < nu ? x[m_dof[1]] : 0,
		m_position[2] + m_dof[2] < nu ? x[m_dof[2]] : 0
	};
}

//static data
Dome* Node::m_dome = nullptr;