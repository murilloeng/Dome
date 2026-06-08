#pragma once

class Dome;

class Loads
{
public:
	//constructor
	Loads(void);

	//destructor
	~Loads(void);

	//data
	double distributed_load(uint32_t) const;
	double distributed_load(uint32_t, double);
	const double* distributed_load(void) const;

private:
	//apply
	void apply(double*) const;
	void apply(double*, const uint32_t*, const double*) const;

	//data
	static Dome* m_dome;
	double m_distributed_load[3];

	//friends
	friend class Dome;
};