//Dome
#include "Dome/inc/Material.hpp"

//constructor
Material::Material(void) : m_density{7.85e+03}, m_yield_stress{4.40e+08}, m_elastic_modulus{2.10e+11}, m_plastic_modulus{8.00e+09}
{
	return;
}

//destructor
Material::~Material(void)
{
	return;
}

//data
double Material::density(void) const
{
	return m_density;
}
double Material::density(double density)
{
	return m_density = density;
}

double Material::yield_stress(void) const
{
	return m_yield_stress;
}
double Material::yield_stress(double yield_stress)
{
	return m_yield_stress = yield_stress;
}

double Material::elastic_modulus(void) const
{
	return m_elastic_modulus;
}
double Material::elastic_modulus(double elastic_modulus)
{
	return m_elastic_modulus = elastic_modulus;
}

double Material::plastic_modulus(void) const
{
	return m_plastic_modulus;
}
double Material::plastic_modulus(double plastic_modulus)
{
	return m_plastic_modulus = plastic_modulus;
}