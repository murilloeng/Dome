#pragma once

//std
#include <cstdint>

class Dome;

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
	void apply(const double*);
	void inertia(double*, const double*) const;
	void stiffness(double*, const double*) const;
	void internal_force(double*, const double*) const;

private:
	//data
	uint32_t m_nodes[2];
	double m_C, m_U, m_f, m_K;

	friend class Dome;
	static Dome* m_dome;
};