#pragma once

class Dome;
class MaterialPoint;

class Material
{
public:
	//constructor
	Material(void);

	//destructor
	~Material(void);

	//data
	bool buckling(bool);
	bool buckling(void) const;

	bool inelastic(bool);
	bool inelastic(void) const;

	double density(double);
	double density(void) const;

	double yield_stress(double);
	double yield_stress(void) const;

	double elastic_modulus(double);
	double elastic_modulus(void) const;

	double plastic_modulus(double);
	double plastic_modulus(void) const;

	//return mapping
	void return_mapping(MaterialPoint&) const;

private:
	//data
	bool m_buckling;
	bool m_inelastic;
	double m_density;
	double m_yield_stress;
	double m_elastic_modulus;
	double m_plastic_modulus;

	//friends
	friend class Dome;
};