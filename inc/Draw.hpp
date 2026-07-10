#pragma once

//Dome
#include "Dome/inc/Dome.hpp"
#include "Dome/inc/What.hpp"

//Canvas
#include "Canvas/inc/Objects/Bases/BaseModel3D.hpp"

class Draw : public canvas::objects::BaseModel3D
{
public:
	//constructor
	Draw(void);

	//destructor
	~Draw(void);

	//data
	What& what(void);

	Dome* dome(void);
	Dome* dome(Dome*);
	
	bool deformed(bool);
	bool deformed(void) const;

	uint32_t step(uint32_t);
	uint32_t step(void) const;

private:
	//draw
	void draw(void) override;
	void setup(void) override;
	void update(void) override;

	//setup
	void setup_nodes(void);
	void setup_elements(void);
	void setup_supports(void);

	//update
	void update_nodes(void);
	void update_elements(void);
	void update_supports(void);

	//data
	What m_what;
	Dome* m_dome;
	bool m_deformed;
	uint32_t m_step;

	uint32_t m_index_lines;
	uint32_t m_index_points;
	uint32_t m_index_vertices;
	uint32_t m_index_triangles;

	uint32_t m_counter_lines;
	uint32_t m_counter_points;
	uint32_t m_counter_vertices;
	uint32_t m_counter_triangles;
};