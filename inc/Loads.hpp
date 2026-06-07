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
	double vertical_load(double);
	double vertical_load(void) const;

private:
	//apply
	void apply(double*) const;
	void apply(double*, const uint32_t*, const double*) const;

	//data
	static Dome* m_dome;
	double m_vertical_load;

	//friends
	friend class Dome;
};