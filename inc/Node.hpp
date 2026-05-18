#pragma once

//std
#include <cstdint>

class Dome;

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
	const double* state(void) const;

	double position(uint32_t) const;
	double position(uint32_t, double);
	const double* position(void) const;

	double velocity(uint32_t) const;
	double velocity(uint32_t, double);
	const double* velocity(void) const;

	double acceleration(uint32_t) const;
	double acceleration(uint32_t, double);
	const double* acceleration(void) const;

private:
	//data
	uint32_t m_dof[3];
	double m_state[3];
	double m_position[3];
	double m_velocity[3];
	double m_acceleration[3];

	//friends
	friend class Dome;
};