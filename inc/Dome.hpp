#pragma once

//std
#include <cstdint>

//Domes
#include "Domes/inc/Section.hpp"
#include "Domes/inc/Material.hpp"

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

	uint32_t order(uint32_t);
	uint32_t order(void) const;

	uint32_t layers(uint32_t);
	uint32_t layers(void) const;

	Section& section(void);
	const Section& section(void) const;

	Material& material(void);
	const Material& material(void) const;

private:
	//data
	double m_height;
	double m_radius;
	uint32_t m_order;
	uint32_t m_layers;
	Section m_section;
	Material m_material;
};