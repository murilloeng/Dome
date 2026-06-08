#pragma once

//std
#include <cstdint>

//Dome
#include "Dome/inc/MaterialPoint.hpp"
#include "Dome/inc/StrainMeasure.hpp"

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
	static double strain_hessian(double);
	static double strain_measure(double);
	static double strain_gradient(double);

	static StrainMeasure strain_measure(void);
	static StrainMeasure strain_measure(StrainMeasure);

	//data
	double m_f, m_K;
	uint32_t m_nodes[2];
	MaterialPoint m_material_point;

	static Dome* m_dome;
	static StrainMeasure m_strain_measure;

	//friends
	friend class Dome;
};