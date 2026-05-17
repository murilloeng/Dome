#pragma once

//std
#include <cstdint>

class Element
{
public:
	//constructor
	Element(void);

	//destructor
	~Element(void);

	//data
	uint32_t node(uint32_t) const;
	uint32_t node(uint32_t, uint32_t);

private:
	//data
	uint32_t m_nodes[2];
};