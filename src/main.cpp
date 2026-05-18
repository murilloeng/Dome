//std
#include <cstdlib>

//Domes
#include "Domes/inc/Dome.hpp"

//Canvas
#include "Canvas/inc/API/Loader.hpp"
#include "Canvas/inc/Shaders/Shader.hpp"

int main(void)
{
	//setup
	canvas::load_functions();
	canvas::shaders::Shader::add_path("../Canvas/shd/");
	//data
	Dome dome;
	dome.sides(5);
	dome.layers(3);
	dome.setup();
	dome.save("dome.txt");
	//return
	return EXIT_SUCCESS;
}