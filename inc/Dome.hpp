#pragma once

//std
#include <cstdint>

//Dome
#include "Dome/inc/Node.hpp"
#include "Dome/inc/Element.hpp"
#include "Dome/inc/Section.hpp"
#include "Dome/inc/Material.hpp"

class Dome
{
public:
	//constructor
	Dome(void);

	//destructor
	~Dome(void);

	//data
	double height(double);
	double height(void) const;
	
	double radius(double);
	double radius(void) const;
	
	uint32_t sides(uint32_t);
	uint32_t sides(void) const;
	
	uint32_t layers(uint32_t);
	uint32_t layers(void) const;

	uint32_t nodes(void) const;
	uint32_t elements(void) const;

	Node& node(uint32_t);
	const Node& node(uint32_t) const;

	Section& section(void);
	const Section& section(void) const;

	Material& material(void);
	const Material& material(void) const;

	Element& element(uint32_t);
	const Element& element(uint32_t) const;

	//save
	void save(const char*) const;

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
	bool m_solved;
	double m_height;
	double m_radius;
	uint32_t m_sides;
	uint32_t m_layers;

	Node* m_nodes;
	Section m_section;
	Element* m_elements;
	Material m_material;

	double(*m_twist)(double, double);
	double(*m_shape)(double, double, double);
};