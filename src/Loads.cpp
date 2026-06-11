//std
#include <cstring>

//Domes
#include "Dome/inc/Dome.hpp"
#include "Dome/inc/Loads.hpp"

//constructor
Loads::Loads(void) : m_distributed_load{0, 0, -2.50e+03}
{
	return;
}

//destructor
Loads::~Loads(void)
{
	return;
}


//data
const double* Loads::distributed_load(void) const
{
	return m_distributed_load;
}
double Loads::distributed_load(uint32_t index) const
{
	return m_distributed_load[index];
}
double Loads::distributed_load(uint32_t index, double load)
{
	return m_distributed_load[index] = load;
}

//apply
void Loads::apply(double* fe) const
{
	//data
	uint32_t nodes[3];
	const uint32_t ns = m_dome->sides();
	const uint32_t nl = m_dome->layers();
	const uint32_t nu = m_dome->dof_unkown();
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
				apply(fe, nodes, m_distributed_load);
				nodes[0] = (i + 0) * ns + (j + 0) % ns;
				nodes[1] = (i + 1) * ns + (j + 1) % ns;
				nodes[2] = (i + 1) * ns + (j + 0) % ns;
				apply(fe, nodes, m_distributed_load);
			}
			else
			{
				nodes[2] = nl * ns;
				nodes[0] = (i + 0) * ns + (j + 0) % ns;
				nodes[1] = (i + 0) * ns + (j + 1) % ns;
				apply(fe, nodes, m_distributed_load);
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