//std
#include <cstdlib>

//Dome
#include "Dome/inc/Engine.hpp"

void draw(Dome* dome)
{
	//data
	Engine engine;
	engine.dome(dome);
	//start
	engine.start();
}

int main(void)
{
	Dome dome;
	dome.sides(10);
	dome.layers(10);
	dome.setup();
	draw(&dome);
	//return
	return EXIT_SUCCESS;
}