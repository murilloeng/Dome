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
	double weight(void) const;

	double density(double);
	double density(void) const;

	double thickness(double);
	double thickness(void) const;

private:
	//apply
	void apply(double*) const;
	void apply(double*, const uint32_t*, const double*) const;

	//data
	double m_density;
	double m_thickness;
	static Dome* m_dome;

	//friends
	friend class Dome;
};