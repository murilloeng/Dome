#pragma once

class What
{
public:
	//constructor
	What(void);

	//destructor
	~What(void);

	//data
	bool nodes(bool);
	bool nodes(void) const;

	bool loads(bool);
	bool loads(void) const;

	bool elements(bool);
	bool elements(void) const;

	bool supports(bool);
	bool supports(void) const;

private:
	//data
	bool m_nodes;
	bool m_loads;
	bool m_elements;
	bool m_supports;
};
