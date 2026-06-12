//std
#include <cstring>

//Domes
#include "Dome/inc/Dome.hpp"
#include "Dome/inc/Loads.hpp"

//constructor
Loads::Loads(void) : m_density{2.50e+03}, m_thickness{1.00e-01}
{
	return;
}

//destructor
Loads::~Loads(void)
{
	return;
}


//data
double Loads::weight(void) const
{
	//data
	double W = 0;
	const double g = 9.81;
	const double r = m_density;
	const double t = m_thickness;
	const uint32_t ns = m_dome->sides();
	const uint32_t nl = m_dome->layers();
	//weight
	math::Vec3 x[4];
	for(uint32_t i = 0; i < nl; i++)
	{
		for(uint32_t j = 0; j < ns; j++)
		{
			if(i + 1 != nl)
			{
				x[0] = m_dome->node((i + 0) * ns + (j + 0) % ns).position();
				x[1] = m_dome->node((i + 0) * ns + (j + 1) % ns).position();
				x[2] = m_dome->node((i + 1) * ns + (j + 1) % ns).position();
				x[3] = m_dome->node((i + 1) * ns + (j + 0) % ns).position();
				W += g * r * t * ((x[1] - x[0]).cross(x[2] - x[0]).norm()) / 2;
				W += g * r * t * ((x[2] - x[0]).cross(x[3] - x[0]).norm()) / 2;
			}
			else
			{
				x[2] = m_dome->node(ns * nl).position();
				x[0] = m_dome->node((i + 0) * ns + (j + 0) % ns).position();
				x[1] = m_dome->node((i + 0) * ns + (j + 1) % ns).position();
				W += g * r * t * (x[1] - x[0]).cross(x[2] - x[0]).norm() / 2;
			}
		}
		
	}
	//return
	return W;
}

double Loads::density(void) const
{
	return m_density;
}
double Loads::density(double density)
{
	return m_density = density;
}

double Loads::thickness(void) const
{
	return m_thickness;
}
double Loads::thickness(double thickness)
{
	return m_thickness = thickness;
}

//apply
void Loads::apply(double* fe) const
{
	//data
	uint32_t nodes[3];
	const uint32_t ns = m_dome->sides();
	const uint32_t nl = m_dome->layers();
	const uint32_t nu = m_dome->dof_unkown();
	//load
	const double g = 9.81e+00;
	const double r = m_density;
	const double t = m_thickness;
	const double p[] = {0, 0, -g * r * t};
	//setup
	memset(fe, 0, nu * sizeof(double));
	//apply
	for(uint32_t i = 0; i < nl; i++)
	{
		for(uint32_t j = 0; j < ns; j++)
		{
			if(i + 1 != nl)
			{
				nodes[0] = (i + 0) * ns + (j + 0) % ns;
				nodes[1] = (i + 0) * ns + (j + 1) % ns;
				nodes[2] = (i + 1) * ns + (j + 1) % ns;
				apply(fe, nodes, p);
				nodes[0] = (i + 0) * ns + (j + 0) % ns;
				nodes[1] = (i + 1) * ns + (j + 1) % ns;
				nodes[2] = (i + 1) * ns + (j + 0) % ns;
				apply(fe, nodes, p);
			}
			else
			{
				nodes[2] = nl * ns;
				nodes[0] = (i + 0) * ns + (j + 0) % ns;
				nodes[1] = (i + 0) * ns + (j + 1) % ns;
				apply(fe, nodes, p);
			}
		}
	}
}
void Loads::apply(double* fe, const uint32_t* nodes, const double* distributed_load) const
{
	//data
	const uint32_t nu = m_dome->dof_unkown();
	const math::Vec3 x1 = m_dome->node(nodes[0]).position();
	const math::Vec3 x2 = m_dome->node(nodes[1]).position();
	const math::Vec3 x3 = m_dome->node(nodes[2]).position();
	//area
	const double A = (x2 - x1).cross(x3 - x1).norm() / 2;
	//apply
	for(uint32_t i = 0; i < 3; i++)
	{
		for(uint32_t j = 0; j < 3; j++)
		{
			const uint32_t dof = m_dome->node(nodes[i]).dof(j);
			if(dof < nu) fe[dof] += A * distributed_load[j] / 3;
		}
	}
}

//static data
Dome* Loads::m_dome = nullptr;