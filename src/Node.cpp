//Dome
#include "Dome/inc/Dome.hpp"
#include "Dome/inc/Node.hpp"

//constructor
Node::Node(void) : m_dof{0, 0, 0}, m_position{0, 0, 0}, m_state{nullptr}, m_velocity{nullptr}, m_acceleration{nullptr}
{
	return;
}

//destructor
Node::~Node(void)
{
	delete[] m_state;
	delete[] m_velocity;
	delete[] m_acceleration;
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
math::Vec3 Node::position(const double* x) const
{
	//data
	const uint32_t nu = m_dome->dof_unkown();
	//return
	return {
		m_position[0] + (m_dof[0] < nu ? x[m_dof[0]] : 0),
		m_position[1] + (m_dof[1] < nu ? x[m_dof[1]] : 0),
		m_position[2] + (m_dof[2] < nu ? x[m_dof[2]] : 0)
	};
}

const double* Node::state(void) const
{
	return m_state;
}
double Node::state(uint32_t step, uint32_t dof) const
{
	return m_state[3 * step + dof];
}

const double* Node::velocity(void) const
{
	return m_velocity;
}
double Node::velocity(uint32_t step, uint32_t dof) const
{
	return m_velocity[3 * step + dof];
}

const double* Node::acceleration(void) const
{
	return m_acceleration;
}
double Node::acceleration(uint32_t step, uint32_t dof) const
{
	return m_acceleration[3 * step + dof];
}

//analysis
void Node::allocate(uint32_t steps, bool dynamics)
{
	//data
	double** data[] = {&m_state, &m_velocity, &m_acceleration};
	//allocate
	for(uint32_t i = 0; i < 3; i++)
	{
		if(i == 0 || dynamics)
		{
			delete[] *data[i];
			*data[i] = new double[3 * steps];
		}
	}
}
void Node::record(const double* x, uint32_t step)
{
	//data
	const uint32_t nu = m_dome->dof_unkown();
	//record
	for(uint32_t i = 0; i < 3; i++)
	{
		m_state[3 * step + i] = m_dof[i] < nu ? x[m_dof[i]] : 0;
	}
}
void Node::record(const double* x, const double* v, const double* a, uint32_t step)
{
	//data
	const uint32_t nu = m_dome->dof_unkown();
	//record
	for(uint32_t i = 0; i < 3; i++)
	{
		m_state[3 * step + i] = m_dof[i] < nu ? x[m_dof[i]] : 0;
		m_velocity[3 * step + i] = m_dof[i] < nu ? v[m_dof[i]] : 0;
		m_acceleration[3 * step + i] = m_dof[i] < nu ? a[m_dof[i]] : 0;
	}
}

//static data
Dome* Node::m_dome = nullptr;