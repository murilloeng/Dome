//Domes
#include "Domes/inc/Dome.hpp"

//constructor
Dome::Dome(void)
{
	return;
}

//destructor
Dome::~Dome(void)
{
	return;
}

//data
double Dome::height(void) const
{
	return m_height;
}
double Dome::height(double height)
{
	return m_height = height;
}

double Dome::radius(void) const
{
	return m_radius;
}
double Dome::radius(double radius)
{
	return m_radius = radius;
}

uint32_t Dome::order(void) const
{
	return m_order;
}
uint32_t Dome::order(uint32_t order)
{
	return m_order = order;
}

uint32_t Dome::layers(void) const
{
	return m_layers;
}
uint32_t Dome::layers(uint32_t layers)
{
	return m_layers = layers;
}

Section& Dome::section(void)
{
	return m_section;
}
const Section& Dome::section(void) const
{
	return m_section;
}

Material& Dome::material(void)
{
	return m_material;
}
const Material& Dome::material(void) const
{
	return m_material;
}