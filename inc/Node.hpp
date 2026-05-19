#pragma once

//std
#include <cstdint>

//Math
#include "Math/inc/linear/vec3.hpp"

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
	const uint32_t* dof(void) const;

	double position(uint32_t) const;
	double position(uint32_t, double);
	const double* position(void) const;
	math::vec3 position(const double*) const;

private:
	//data
	uint32_t m_dof[3];
	double m_position[3];

	friend class Dome;
	static Dome* m_dome;
};