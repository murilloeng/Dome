#pragma once

class Section
{
public:
	//constructor
	Section(void);

	//destructor
	~Section(void);

	//data
	double area(double);
	double area(void) const;

	double inertia(double);
	double inertia(void) const;

private:
	//data
	double m_area;
	double m_inertia;
};