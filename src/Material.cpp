//std
#include <cmath>

//Math
#include "Math/inc/Miscellaneous/util.hpp"

//Dome
#include "Dome/inc/Material.hpp"
#include "Dome/inc/MaterialPoint.hpp"

//constructor
Material::Material(void) :
	m_inelastic{false}, m_density{7.85e+03}, m_yield_stress{4.40e+08}, m_elastic_modulus{2.10e+11}, m_plastic_modulus{8.00e+09}
{
	return;
}

//destructor
Material::~Material(void)
{
	return;
}

//data
bool Material::inelastic(void) const
{
	return m_inelastic;
}
bool Material::inelastic(bool inelastic)
{
	return m_inelastic = inelastic;
}

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

//return mapping
void Material::return_mapping(MaterialPoint& materialPoint) const
{
	//data
	const double sy = m_yield_stress;
	const double E = m_elastic_modulus;
	double& s = materialPoint.m_stress;
	double& K = materialPoint.m_stiffness;
	const double e = materialPoint.m_strain;
	double& ep_new = materialPoint.m_plastic_strain_new;
	const double ep_old = materialPoint.m_plastic_strain_old;
	//return mapping
	K = E;
	s = E * (e - ep_old);
	if(m_inelastic && fabs(s) > sy)
	{
		K = 0;
		s = sy * math::sign(s);
		ep_new = ep_old + (s - sy * math::sign(s)) / E;
	}
}