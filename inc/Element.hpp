#pragma once

//std
#include <cstdint>

class Element
{
public:
	//constructor
	Element(void);

	//destructor
	~Element(void);

	//data
	uint32_t node(uint32_t) const;
	uint32_t node(uint32_t, uint32_t);

	//formulation
	void inertia(double*) const;
	void stiffness(double*) const;
	void internal_force(double*) const;

private:
	//data
	uint32_t m_nodes[2];
};