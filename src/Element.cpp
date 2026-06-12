//std
#include <cmath>

//Dome
#include "Dome/inc/Dome.hpp"
#include "Dome/inc/Element.hpp"
#include "Dome/inc/Section.hpp"

//Math
#include "Math/inc/Linear/Vec3.hpp"

//constructor
Element::Element(void) : m_nodes{0, 0}
{
	return;
}

//destructor
Element::~Element(void)
{
	return;
}

//data
uint32_t Element::node(uint32_t index) const
{
	return m_nodes[index];
}
uint32_t Element::node(uint32_t index, uint32_t node)
{
	return m_nodes[index] = node;
}

//formulation
void Element::apply(const double* x)
{
	//data
	const double A = m_dome->section().area();
	const double& s = m_material_point.m_stress;
	const double& K = m_material_point.m_stiffness;
	const math::Vec3 z1 = m_dome->node(m_nodes[0]).position();
	const math::Vec3 z2 = m_dome->node(m_nodes[1]).position();
	const math::Vec3 x1 = m_dome->node(m_nodes[0]).position(x);
	const math::Vec3 x2 = m_dome->node(m_nodes[1]).position(x);
	//length
	const double l0 = (z2 - z1).norm();
	const double ln = (x2 - x1).norm();
	//strain
	const double e = strain_measure(ln / l0);
	const double h = strain_hessian(ln / l0);
	const double g = strain_gradient(ln / l0);
	//material
	m_material_point.m_strain = e;
	m_dome->material().return_mapping(m_material_point);
	//apply
	m_f = s * g * A;
	m_K = s * h * A / l0 + K * g * g * A / l0;
}
void Element::inertia(double* M, const double* x) const
{
	return;
}
void Element::stiffness(double* K, const double* x) const
{
	//data
	const uint32_t nu = m_dome->dof_unkown();
	const uint32_t* d1 = m_dome->node(m_nodes[0]).dof();
	const uint32_t* d2 = m_dome->node(m_nodes[1]).dof();
	const math::Vec3 x1 = m_dome->node(m_nodes[0]).position(x);
	const math::Vec3 x2 = m_dome->node(m_nodes[1]).position(x);
	//data
	const double ln = (x2 - x1).norm();
	const math::Vec3 tn = (x2 - x1) / ln;
	//force
	for(uint32_t j = 0; j < 3; j++)
	{
		for(uint32_t k = 0; k < 3; k++)
		{
			if(d1[j] < nu && d1[k] < nu)
			{
				K[d1[j] + nu * d1[k]] += m_K * tn[j] * tn[k];
				K[d1[j] + nu * d1[k]] += m_f / ln * ((j == k) - tn[j] * tn[k]);
			}
			if(d1[j] < nu && d2[k] < nu)
			{
				K[d1[j] + nu * d2[k]] -= m_K * tn[j] * tn[k];
				K[d1[j] + nu * d2[k]] -= m_f / ln * ((j == k) - tn[j] * tn[k]);
			}
			if(d2[j] < nu && d1[k] < nu)
			{
				K[d2[j] + nu * d1[k]] -= m_K * tn[j] * tn[k];
				K[d2[j] + nu * d1[k]] -= m_f / ln * ((j == k) - tn[j] * tn[k]);
			}
			if(d2[j] < nu && d2[k] < nu)
			{
				K[d2[j] + nu * d2[k]] += m_K * tn[j] * tn[k];
				K[d2[j] + nu * d2[k]] += m_f / ln * ((j == k) - tn[j] * tn[k]);
			}
		}
	}
}
void Element::internal_force(double* f, const double* x) const
{
	//data
	const uint32_t nu = m_dome->dof_unkown();
	const uint32_t* d1 = m_dome->node(m_nodes[0]).dof();
	const uint32_t* d2 = m_dome->node(m_nodes[1]).dof();
	const math::Vec3 x1 = m_dome->node(m_nodes[0]).position(x);
	const math::Vec3 x2 = m_dome->node(m_nodes[1]).position(x);
	//data
	const double ln = (x2 - x1).norm();
	//internal force
	for(uint32_t j = 0; j < 3; j++)
	{
		if(d1[j] < nu) f[d1[j]] -= m_f * (x2[j] - x1[j]) / ln;
		if(d2[j] < nu) f[d2[j]] += m_f * (x2[j] - x1[j]) / ln;
	}
}

//strains
double Element::strain_hessian(double s)
{
	return 
		m_strain_measure == StrainMeasure::Linear ? 0 :
		m_strain_measure == StrainMeasure::Quadratic ? 1 :
		m_strain_measure == StrainMeasure::Logarithmic ? -1 / s / s : 0;
}
double Element::strain_measure(double s)
{
	return 
		m_strain_measure == StrainMeasure::Linear ? s :
		m_strain_measure == StrainMeasure::Quadratic ? (s * s - 1) / 2 :
		m_strain_measure == StrainMeasure::Logarithmic ? log(s) : 0;
}
double Element::strain_gradient(double s)
{
	return 
		m_strain_measure == StrainMeasure::Linear ? 1 :
		m_strain_measure == StrainMeasure::Quadratic ? s :
		m_strain_measure == StrainMeasure::Logarithmic ? 1 / s : 0;
}

StrainMeasure Element::strain_measure(void)
{
	return m_strain_measure;
}
StrainMeasure Element::strain_measure(StrainMeasure strain_measure)
{
	return m_strain_measure = strain_measure;
}

//static data
Dome* Element::m_dome = nullptr;
StrainMeasure Element::m_strain_measure = StrainMeasure::Quadratic;