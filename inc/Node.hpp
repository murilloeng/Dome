#pragma once

//std
#include <cstdint>

#include "Math/inc/Linear/Vec3.hpp"

class Dome;

class Node
{
public:
	//constructor
	Node(void);

	//destructor
	~Node(void);

	//save
	void save(uint32_t) const;

	//data
	const double* loads(void);
	double load(uint32_t) const;
	double load(uint32_t, double);

	uint32_t dof(uint32_t) const;
	const uint32_t* dof(void) const;

	double position(uint32_t) const;
	double position(uint32_t, double);

	const double* position(void) const;
	math::Vec3 position(const double*) const;

	const double* state(void) const;
	double state(uint32_t, uint32_t) const;

	const double* velocity(void) const;
	double velocity(uint32_t, uint32_t) const;

	const double* acceleration(void) const;
	double acceleration(uint32_t, uint32_t) const;
	
private:
	//analysis
	void assemble(double*) const;
	void allocate(uint32_t, bool);
	void record(const double*, uint32_t);
	void record(const double*, const double*, const double*, uint32_t);

	//data
	static Dome* m_dome;

	uint32_t m_dof[3];
	double m_loads[3];
	double m_position[3];
	double *m_state, *m_velocity, *m_acceleration;

	//friends
	friend class Dome;
};