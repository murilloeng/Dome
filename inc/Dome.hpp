#pragma once

//std
#include <cstdint>

//FEA
#include "FEA/inc/Model.hpp"

//Sections
#include "Sections/inc/Section.hpp"

//Materials
#include "Materials/inc/Mechanic/Mechanic.hpp"

class Dome : public fea::Model
{
public:
	//constructor
	Dome(void);

	//destructor
	~Dome(void);

	//data
	uint32_t sides(uint32_t);
	uint32_t sides(void) const;

	double twist(double);
	double twist(void) const;

	double height(uint32_t) const;
	double height(uint32_t, double);

	double radius(uint32_t) const;
	double radius(uint32_t, double);

	double load(uint32_t, uint32_t) const;
	double load(uint32_t, uint32_t, double);

	sections::Section* section(void) const;

	materials::Mechanic* material(void) const;

	//print
	void print(void) const;

	//solve
	void solve_modal(void);
	void solve_static(void);
	void solve_dynamic(void);
	void solve_buckling(void);
	void solve_harmonic(void);

private:
	//setup
	void setup_model(void);
	void setup_loads(void);
	void setup_nodes(void);
	void setup_elements(void);
	void setup_supports(void);

	//data
	using fea::Model::solve, fea::Model::compute;
	using fea::Model::mesh, fea::Model::boundary, fea::Model::analysis;

	//data
	uint32_t m_sides;

	double m_twist;
	double m_loads[6];
	double m_height[2];
	double m_radius[2];

	sections::Section* m_section;
	materials::Mechanic* m_material;
};