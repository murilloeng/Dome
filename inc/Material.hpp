#pragma once

class Material
{
public:
	//constructor
	Material(void);

	//destructor
	~Material(void);

	//data
	double density(double);
	double density(void) const;

	double yield_stress(double);
	double yield_stress(void) const;

	double elastic_modulus(double);
	double elastic_modulus(void) const;

	double plastic_modulus(double);
	double plastic_modulus(void) const;

private:
	//data
	double m_density;
	double m_yield_stress;
	double m_elastic_modulus;
	double m_plastic_modulus;
};