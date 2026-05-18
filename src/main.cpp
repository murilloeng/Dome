//std
#include <cstdlib>

//Domes
#include "Domes/inc/Dome.hpp"

int main(void)
{
	//data
	Dome dome;
	dome.setup();
	dome.save("dome.txt");
	//return
	return EXIT_SUCCESS;
}