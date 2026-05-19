#pragma once

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
	//data
	double m_strain;
	double m_stress;
	double m_stiffness;
	double m_plastic_strain_old;
	double m_plastic_strain_new;

	//friends
	friend class Element;
	friend class Material;
};