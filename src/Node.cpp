//Domes
#include "Domes/inc/Node.hpp"

//constructor
Node::Node(void) : m_dof{0, 0, 0}, m_state{0, 0, 0}, m_position{0, 0, 0}, m_velocity{0, 0, 0}, m_acceleration{0, 0, 0}
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

double Node::state(uint32_t index) const
{
	return m_state[index];
}
double Node::state(uint32_t index, double state)
{
	return m_state[index] = state;
}

double Node::position(uint32_t index) const
{
	return m_position[index];
}
double Node::position(uint32_t index, double position)
{
	return m_position[index] = position;
}

double Node::velocity(uint32_t index) const
{
	return m_velocity[index];
}
double Node::velocity(uint32_t index, double velocity)
{
	return m_velocity[index] = velocity;
}

double Node::acceleration(uint32_t index) const
{
	return m_acceleration[index];
}
double Node::acceleration(uint32_t index, double acceleration)
{
	return m_acceleration[index] = acceleration;
}