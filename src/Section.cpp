//Dome
#include "Dome/inc/Section.hpp"

//constructor
Section::Section(void) : m_area{1.37e-04}, m_inertia{6.29e-09}
{
	return;
}

//destructor
Section::~Section(void)
{
	return;
}

//data
double Section::area(void) const
{
	return m_area;
}
double Section::area(double area)
{
	return m_area = area;
}

double Section::inertia(void) const
{
	return m_inertia;
}
double Section::inertia(double inertia)
{
	return m_inertia = inertia;
}