//std
#include <cmath>

//Math
#include "Math/inc/Miscellaneous/util.hpp"

//Dome
#include "Dome/inc/Material.hpp"
#include "Dome/inc/MaterialPoint.hpp"

//constructor
Material::Material(void) :
	m_buckling{false}, m_inelastic{false}, m_density{7.85e+03}, m_yield_stress{4.40e+08}, m_elastic_modulus{2.10e+11}, m_plastic_modulus{8.00e+09}
{
	return;
}

//destructor
Material::~Material(void)
{
	return;
}

//data
bool Material::buckling(void) const
{
	return m_buckling;
}
bool Material::buckling(bool buckling)
{
	return m_buckling = buckling;
}

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
void Material::return_mapping(MaterialPoint& material_point) const
{
	//data
	const double sy = m_yield_stress;
	const double E = m_elastic_modulus;
	const double e = material_point.m_strain;
	const double& s = material_point.m_stress;
	const double sb = material_point.m_buckling_stress;
	const double ep_old = material_point.m_plastic_strain_old;
	//predictor
	material_point.m_stiffness = E;
	material_point.m_stress = E * (e - ep_old);
	//buckling
	if(m_buckling && s < -sb)
	{
		material_point.m_stress = -sb;
		material_point.m_stiffness = 0;
	}
	//plasticity
	if(m_inelastic && fabs(s) > sy)
	{
		material_point.m_stiffness = 0;
		material_point.m_stress = sy * math::sign(s);
		material_point.m_plastic_strain_new = ep_old + (s - sy * math::sign(s)) / E;
	}
}