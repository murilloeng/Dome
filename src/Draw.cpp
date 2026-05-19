//Dome
#include "Dome/inc/Draw.hpp"

//Canvas
#include "Canvas/inc/Scene/Scene.hpp"
#include "Canvas/inc/Vertices/Model3D.hpp"

//constructor
Draw::Draw(void) : m_deformed{false}, m_step{0}
{
	return;
}

//constructor
Draw::~Draw(void)
{
	return;
}

//data
What& Draw::what(void)
{
	return m_what;
}

Dome* Draw::dome(void)
{
	return m_dome;
}
Dome* Draw::dome(Dome* dome)
{
	return m_dome = dome;
}

bool Draw::deformed(void) const
{
	return m_deformed;
}
bool Draw::deformed(bool deformed)
{
	return m_deformed = deformed;
}

uint32_t Draw::step(void) const
{
	return m_step;
}
uint32_t Draw::step(uint32_t step)
{
	return m_step = step;
}

//draw
void Draw::draw(void)
{
	//data
	const uint64_t offset_1 = m_counter_points * sizeof(uint32_t);
	const uint64_t offset_2 = offset_1 + m_counter_lines * sizeof(uint32_t);
	//draw
	m_vao.bind();
	m_shader.bind();
	glDrawElements(GL_POINTS, m_counter_points, GL_UNSIGNED_INT, nullptr);
	glDrawElements(GL_LINES, m_counter_lines, GL_UNSIGNED_INT, (void*) offset_1);
	glDrawElements(GL_TRIANGLES, m_counter_triangles, GL_UNSIGNED_INT, (void*) offset_2);
}
void Draw::setup(void)
{
	//data
	m_counter_lines = 0;
	m_counter_points = 0;
	m_counter_vertices = 0;
	m_counter_triangles = 0;
	//setup
	if(m_what.nodes()) setup_nodes();
	if(m_what.elements()) setup_elements();
	if(m_what.supports()) setup_supports();
	//allocate
	m_vbo.allocate(m_counter_vertices);
	m_ibo.allocate(m_counter_points + m_counter_lines + m_counter_triangles);
}
void Draw::update(void)
{
	//data
	m_index_lines = 0;
	m_index_points = 0;
	m_index_vertices = 0;
	m_index_triangles = 0;
	//update
	if(m_what.nodes()) update_nodes();
	if(m_what.elements()) update_elements();
	if(m_what.supports()) update_supports();
	//transfer
	m_vbo.transfer();
	m_ibo.transfer();
}

//setup
void Draw::setup_nodes(void)
{
	//data
	const uint64_t nn = m_dome->nodes().size();
	//setup
	m_counter_points += nn;
	m_counter_vertices += nn;
}
void Draw::setup_elements(void)
{
	//data
	const uint64_t nn = m_dome->nodes().size();
	const uint64_t ne = m_dome->elements().size();
	//setup
	m_counter_vertices += nn;
	m_counter_lines += 2 * ne;
}
void Draw::setup_supports(void)
{
	//data
	const uint32_t ns = m_dome->sides();
	//setup
	m_counter_lines += 40 * ns;
	m_counter_vertices += 26 * ns;
	m_counter_triangles += 48 * ns;
}

//update
void Draw::update_nodes(void)
{
	//data
	const uint64_t nn = m_dome->nodes().size();
	uint32_t* ibo_ptr = m_ibo.data() + m_index_points;
	canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
	//buffers data
	for(uint32_t i = 0; i < nn; i++)
	{
		ibo_ptr[i] = i;
		vbo_ptr[i].m_color = "red";
		vbo_ptr[i].m_position = m_dome->node(i).position();
		if(m_deformed) vbo_ptr[i].m_position += m_dome->node(i).state() + 3 * m_step;
	}
	//update
	m_index_points += nn;
	m_index_vertices += nn;
}
void Draw::update_elements(void)
{
	//data
	const uint64_t nn = m_dome->nodes().size();
	const uint64_t ne = m_dome->elements().size();
	uint32_t* ibo_ptr = m_ibo.data() + m_counter_points + m_index_lines;
	canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
	//vbo data
	for(uint32_t i = 0; i < nn; i++)
	{
		vbo_ptr[i].m_color = "blue";
		vbo_ptr[i].m_position = m_dome->node(i).position();
		if(m_deformed) vbo_ptr[i].m_position += m_dome->node(i).state() + 3 * m_step;
	}
	//ibo data
	for(uint32_t i = 0; i < ne; i++)
	{
		ibo_ptr[2 * i + 0] = m_index_vertices + m_dome->element(i).node(0);
		ibo_ptr[2 * i + 1] = m_index_vertices + m_dome->element(i).node(1);
	}
	//update
	m_index_vertices += nn;
	m_index_lines += 2 * ne;
}
void Draw::update_supports(void)
{
	//data
	const uint32_t ns = m_dome->sides();
	const float size = 0.05f * float(m_dome->radius());
	canvas::Color color = scene()->background().inverse();
	uint32_t* ibo_ptr_lines = m_ibo.data() + m_counter_points + m_index_lines;
	uint32_t* ibo_ptr_triangles = m_ibo.data() + m_counter_points + m_counter_lines + m_index_triangles;
	canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
	const canvas::vec3 vbo_data[] = {
		{0, 0, 0},
		{-size, -size, -size},
		{+size, -size, -size},
		{+size, +size, -size},
		{-size, +size, -size},
		{-1.5f * size, -1.5f * size, -size},
		{+1.5f * size, -1.5f * size, -size},
		{+1.5f * size, +1.5f * size, -size},
		{-1.5f * size, +1.5f * size, -size},
		{-1.5f * size, -1.5f * size, -1.5f * size},
		{+1.5f * size, -1.5f * size, -1.5f * size},
		{+1.5f * size, +1.5f * size, -1.5f * size},
		{-1.5f * size, +1.5f * size, -1.5f * size}
	};
	const uint32_t ibo_data_lines[] = {
		0,  1,  0,  2,  0,  3,  0, 4,
		1,  2,  2,  3,  3,  4,  4, 1,
		5,  6,  6,  7,  7,  8,  8, 5,
		9, 10, 10, 11, 11, 12, 12, 9,
		5,  9,  6, 10,  7, 11,  8, 12
	};
	const uint32_t ibo_data_triangles[] = {
		0, 1,  2, 0,  2,  3, 0,  3,  4, 0,  4,  1,
		5, 6,  7, 5,  7,  8, 9, 10, 11, 9, 11, 12,
		5, 6, 10, 5, 10,  9, 6,  7, 11, 6, 11, 10,
		7, 8, 12, 7, 12, 11, 8,  5,  9, 8,  9, 12
	};
	//vbo data
	for(uint32_t i = 0; i < ns; i++)
	{
		const canvas::vec3 zi = m_dome->node(i).position();
		for(uint32_t j = 0; j < 13; j++)
		{
			vbo_ptr[26 * i + j +  0].m_color = color;
			vbo_ptr[26 * i + j + 13].m_color = "gray";
			vbo_ptr[26 * i + j +  0].m_position = zi + vbo_data[j];
			vbo_ptr[26 * i + j + 13].m_position = zi + vbo_data[j];
		}
	}
	//ibo data
	for(uint32_t i = 0; i < ns; i++)
	{
		for(uint32_t j = 0; j < 40; j++) ibo_ptr_lines[40 * i + j] = m_index_vertices + 26 * i + ibo_data_lines[j];
		for(uint32_t j = 0; j < 48; j++) ibo_ptr_triangles[48 * i + j] = m_index_vertices + 26 * i + ibo_data_triangles[j] + 13;
	}
	//update
	m_index_lines += 40 * ns;
	m_index_vertices += 26 * ns;
	m_index_triangles += 48 * ns;
}