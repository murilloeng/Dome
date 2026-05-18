#pragma once

//std
#include <cstdint>

class Node
{
public:
	//constructor
	Node(void);

	//destructor
	~Node(void);

	//data
	uint32_t dof(uint32_t) const;

	double state(uint32_t) const;
	double state(uint32_t, double);

	double position(uint32_t) const;
	double position(uint32_t, double);

	double velocity(uint32_t) const;
	double velocity(uint32_t, double);

	double acceleration(uint32_t) const;
	double acceleration(uint32_t, double);

private:
	//data
	uint32_t m_dof[3];
	double m_state[3];
	double m_position[3];
	double m_velocity[3];
	double m_acceleration[3];
};