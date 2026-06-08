#pragma once

class Dome;
class Element;
class Material;

class MaterialPoint
{
public:
	//constructor
	MaterialPoint(void);

	//destructor
	~MaterialPoint(void);

private:
	//analysis
	void reset(void);
	void update(void);
	void restore(void);
	//data
	double m_strain;
	double m_stress;
	double m_stiffness;
	double m_buckling_stress;
	double m_plastic_strain_old;
	double m_plastic_strain_new;

	//friends
	friend class Dome;
	friend class Element;
	friend class Material;
};