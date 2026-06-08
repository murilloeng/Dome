#pragma once

//std
#include <vector>
#include <cstdint>

//Dome
#include "Dome/inc/Node.hpp"
#include "Dome/inc/Loads.hpp"
#include "Dome/inc/Element.hpp"
#include "Dome/inc/Section.hpp"
#include "Dome/inc/Material.hpp"

//Math
#include "Math/inc/Solvers/Newmark.hpp"
#include "Math/inc/Solvers/NewtonRaphson.hpp"

class Dome
{
public:
	//constructor
	Dome(void);

	//destructor
	~Dome(void);

	//data
	bool flip(bool);
	bool flip(void) const;

	double height(double);
	double height(void) const;
	
	double radius(double);
	double radius(void) const;
	
	uint32_t sides(uint32_t);
	uint32_t sides(void) const;
	
	uint32_t layers(uint32_t);
	uint32_t layers(void) const;

	uint32_t dof_unkown(void) const;

	Loads& loads(void);
	const Loads& loads(void) const;

	Node& node(uint32_t);
	const Node& node(uint32_t) const;

	Section& section(void);
	const Section& section(void) const;

	Material& material(void);
	const Material& material(void) const;

	Element& element(uint32_t);
	const Element& element(uint32_t) const;

	std::vector<Node>& nodes(void);
	const std::vector<Node>& nodes(void) const;

	std::vector<Element>& elements(void);
	const std::vector<Element>& elements(void) const;

	math::solvers::NewtonRaphson& solver_static(void);

	//save
	void save(const char*) const;

	//loads
	void apply_loads_vertical(void);

	//analysis
	void setup(void);
	void solve_modal(void);
	void solve_static(void);
	void solve_dynamic(void);
	void solve_buckling(void);
	void solve_harmonic(void);

private:
	//analysis
	void setup_nodes(void);
	void setup_elements(void);

	//data
	bool m_flip;
	bool m_solved;
	double m_height;
	double m_radius;
	uint32_t m_sides;
	uint32_t m_layers;

	Loads m_loads;
	Section m_section;
	Material m_material;
	std::vector<Node> m_nodes;
	std::vector<Element> m_elements;

	double(*m_twist)(double, double);
	double(*m_shape)(double, double, double);

	math::solvers::Newmark m_solver_dynamic;
	math::solvers::NewtonRaphson m_solver_static;
};