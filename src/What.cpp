//Dome
#include "Dome/inc/What.hpp"

//constructor
What::What(void) : m_nodes{true}, m_loads{true}, m_elements{true}, m_supports{true}
{
	return;
}

//destructor
What::~What(void)
{
	return;
}

//data
bool What::nodes(void) const
{
	return m_nodes;
}
bool What::nodes(bool nodes)
{
	return m_nodes = nodes;
}

bool What::loads(void) const
{
	return m_loads;
}
bool What::loads(bool loads)
{
	return m_loads = loads;
}

bool What::elements(void) const
{
	return m_elements;
}
bool What::elements(bool elements)
{
	return m_elements = elements;
}

bool What::supports(void) const
{
	return m_supports;
}
bool What::supports(bool supports)
{
	return m_supports = supports;
}