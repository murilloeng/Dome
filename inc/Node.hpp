#pragma once

//std
#include <cstdint>

class Node
{
public:
	//constructor
	Node(void);

	//destructor
	~Node(void);

private:
	//data
	double m_dof[3];
	double m_position[3];
	uint32_t m_dof_indexes[3];
};