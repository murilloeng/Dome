#pragma once

//std
#include <cstdint>

class Dome;
typedef double(*time_function)(double);

class Load
{
public:
	//constructor
	Load(void);

	//destructor
	~Load(void);

	//data
	double value(double);
	double value(void) const;

	uint32_t dof(uint32_t);
	uint32_t dof(void) const;

	uint32_t node(uint32_t);
	uint32_t node(void) const;

	time_function function(void) const;
	time_function function(time_function);

	//analysis
	void external_force(double*) const;

private:
	//data
	double m_value;
	uint32_t m_dof;
	uint32_t m_node;
	time_function m_function;

	friend class Dome;
	static Dome* m_dome;
};