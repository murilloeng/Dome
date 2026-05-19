#pragma once

//std
#include <cstdint>

//Dome
#include "Dome/inc/MaterialPoint.hpp"

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
	//strains
	double strain_hessian(double) const;
	double strain_measure(double) const;
	double strain_gradient(double) const;

	//data
	static Dome* m_dome;

	double m_f, m_K;
	uint32_t m_nodes[2];
	MaterialPoint m_material_point;

	//friends
	friend class Dome;
};