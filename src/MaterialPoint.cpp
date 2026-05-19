//Dome
#include "Dome/inc/MaterialPoint.hpp"

//constructor
MaterialPoint::MaterialPoint(void) : m_strain{0}, m_stress{0}, m_stiffness{0}, m_plastic_strain_old{0}, m_plastic_strain_new{0}
{
	return;
}

//destructor
MaterialPoint::~MaterialPoint(void)
{
	return;
}