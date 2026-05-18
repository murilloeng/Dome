//std
#include <cstdlib>

//Domes
#include "Domes/inc/Dome.hpp"

int main(void)
{
	//data
	Dome dome;
	dome.sides(5);
	dome.layers(3);
	dome.setup();
	dome.save("dome.txt");
	//return
	return EXIT_SUCCESS;
}