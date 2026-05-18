//std
#include <cstdlib>

//Dome
#include "Dome/inc/Engine.hpp"

int main(void)
{
	//data
	Engine engine;
	engine.dome()->sides(10);
	engine.dome()->layers(10);
	//start
	engine.start();
	//return
	return EXIT_SUCCESS;
}