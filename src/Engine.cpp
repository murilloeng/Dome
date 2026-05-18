//std
#include <chrono>
#include <thread>
#include <stdexcept>

//Domes
#include "Domes/inc/Engine.hpp"

//Canvas
#include "Canvas/inc/API/Loader.hpp"

//constructor
Engine::Engine(void) : m_playing{false}, m_show_fps{true}, m_framerate{60}
{
	setup_glfw();
	setup_model();
	setup_scene();
	setup_callbacks();
}

//destructor
Engine::~Engine(void)
{
	delete m_draw;
	delete m_scene;
	glfwTerminate();
}

//start
void Engine::start(void)
{
	glfwSetTime(0);
	float t1 = 0, t2;
	while(!glfwWindowShouldClose(m_window))
	{
		//time
		t2 = (float) glfwGetTime();
		//play
		if(m_playing)
		{
			update_step(m_window, true);
			m_scene->update();
			const uint32_t t = uint32_t(1000 / m_framerate);
			std::this_thread::sleep_for(std::chrono::milliseconds(t));
		}
		//animations
		glfwPollEvents();
		m_scene->time(t2);
		m_scene->update_animations();
		//draw
		m_scene->draw();
		glfwSwapBuffers(m_window);
		//framerate
		if(m_show_fps)
		{
			printf("FPS: %d\n", uint32_t(1 / (t2 - t1)));
			t1 = t2;
		}
	}
}

//data
bool Engine::show_fps(void) const
{
	return m_show_fps;
}
bool Engine::show_fps(bool show_fps)
{
	return m_show_fps = show_fps;
}

double Engine::framerate(void) const
{
	return m_framerate;
}
double Engine::framerate(double framerate)
{
	return m_framerate = framerate;
}

//setup
void Engine::setup_glfw(void)
{
	//library
	if(glfwInit() != GLFW_TRUE)
	{
		throw std::runtime_error("GLFW initialization failed!");
	}
	//window
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(900, 900, "Canvas", nullptr, nullptr);
	if(!m_window)
	{
		glfwTerminate();
		throw std::runtime_error("GLFW window initialization failed!");
	}
	//context
	glfwMakeContextCurrent(m_window);
	//v-sync
	glfwSwapInterval(0);
	//functions
	canvas::load_functions();
}
void Engine::setup_model(void)
{
	//data
	char path[512];
	m_model = new fea::models::Model;
	FILE* file = fopen(".recent", "r");
	//check
	if(!file) return;
	if(!fgets(path, 512, file)) return;
	//load
	m_model->load(path);
	m_model->load_results();
	//close
	fclose(file);
}
void Engine::setup_scene(void)
{
	//scene
	m_scene = new canvas::Scene;
	canvas::shaders::Shader::add_path("../../Canvas/shd/");
	//objects
	m_draw = new Draw;
	m_draw->model(m_model);
	m_draw->compute_bounding_boxes();
	m_scene->add_object(m_draw);
	//camera
	m_scene->setup();
	m_scene->update();
	m_scene->camera().fixed_bounding_box(true);
	m_scene->camera().bounding_box(m_draw->bounding_box());
	//screen
	int32_t width, height;
	glfwGetWindowSize(m_window, &width, &height);
	m_scene->camera().callback_reshape(width, height);
}
void Engine::setup_callbacks(void)
{
	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(m_window, callback_keyboard);
	glfwSetScrollCallback(m_window, callback_wheel);
	glfwSetWindowSizeCallback(m_window, callback_resize);
	glfwSetCursorPosCallback(m_window, callback_position);
	glfwSetMouseButtonCallback(m_window, callback_button);
}

//step
void Engine::update_model(GLFWwindow* window)
{
	//data
	Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//setup
	string = engine->m_model->path();
	rl_startup_hook = readline_start;
	rl_basic_quote_characters = const_cast<char*>("\"'");
	rl_filename_quote_characters = const_cast<char*>(" ");
	rl_completer_quote_characters = const_cast<char*>("\"'");
	//input
	char* input = readline("Enter model path: ");
	//file
	readline_path_fix(input);
	FILE* file = fopen(".recent", "w");
	fprintf(file, "%s", input);
	fclose(file);
	//model
	delete engine->m_model;
	engine->m_model = new fea::models::Model;
	//load
	engine->m_model->load(input);
	engine->m_model->load_results();
	engine->m_draw->model(engine->m_model);
	//scene
	engine->m_scene->setup();
	engine->m_scene->update();
	//camera
	engine->m_scene->camera().bounding_box(engine->m_draw->bounding_box());
	engine->m_scene->camera().bound();
	//focus
	glfwFocusWindow(window);
	//delete
	free(input);
}
void Engine::update_playing(GLFWwindow* window)
{
	//data
	Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//playing
	engine->m_playing = !engine->m_playing;
}
void Engine::update_step(GLFWwindow * window, bool increase)
{
	//data
	const Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//step
	const uint32_t step_1 = engine->m_draw->step();
	const uint32_t steps = engine->m_model->results()->steps();
	const uint32_t step_2 = increase ? (step_1 + 1) % (steps + 1) : step_1 != 0 ? step_1 - 1 : steps;
	//draw
	engine->m_draw->step(step_2);
	if(!engine->m_show_fps) printf("step: %d\n", step_2);
	//update
	engine->m_scene->update();
}
void Engine::update_type(GLFWwindow* window, bool increase)
{
	//engine
	const Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//data
	const Mode mode = engine->m_draw->mode();
	const uint32_t dof_set = engine->m_model->results()->dof_set();
	const uint64_t state_set = engine->m_model->results()->state_set();
	//mode
	if(mode == Mode::Nodes)
	{
		//data
		const uint32_t count = math::bit_count(dof_set);
		const uint32_t index_1 = engine->m_draw->what().dof_index();
		const uint32_t index_2 = increase ? (index_1 + 1) % count : index_1 != 0 ? index_1 - 1 : count - 1;
		//update
		engine->m_draw->what().dof_index(index_2);
		if(!engine->m_show_fps) printf("DOF: %s\n", engine->m_draw->what().dof_name(dof_set));
	}
	else if(mode == Mode::Elements)
	{
		//data
		const uint32_t count = math::bit_count(state_set);
		const uint32_t index_1 = engine->m_draw->what().state_index();
		const uint32_t index_2 = increase ? (index_1 + 1) % count : index_1 != 0 ? index_1 - 1 : count - 1;
		//update
		engine->m_draw->what().state_index(index_2);
		if(!engine->m_show_fps) printf("State: %s\n", engine->m_draw->what().state_name(state_set));
	}
	//update
	engine->m_scene->update();
}
void Engine::update_framerate(GLFWwindow* window, bool increase)
{
	//data
	const double factor = 1.10;
	Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//framerate
	engine->m_framerate *= increase ? factor : 1 / factor;
}

void Engine::update_what_edges(GLFWwindow* window)
{
	//data
	const Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//edges
	engine->m_draw->what().edges(!engine->m_draw->what().edges());
	//update
	engine->m_scene->setup();
	engine->m_scene->update();
}
void Engine::update_what_faces(GLFWwindow* window)
{
	//data
	const Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//edges
	engine->m_draw->what().faces(!engine->m_draw->what().faces());
	//update
	engine->m_scene->setup();
	engine->m_scene->update();
}
void Engine::update_what_volume(GLFWwindow* window)
{
	//data
	const Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//edges
	engine->m_draw->what().volume(!engine->m_draw->what().volume());
	//update
	engine->m_scene->setup();
	engine->m_scene->update();
}

void Engine::update_mode_mesh(GLFWwindow* window)
{
	//data
	const Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//edges
	engine->m_draw->mode(Mode::Mesh);
	//update
	engine->m_scene->setup();
	engine->m_scene->update();
}
void Engine::update_mode_nodes(GLFWwindow* window)
{
	//data
	const Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//edges
	engine->m_draw->mode(Mode::Nodes);
	//update
	engine->m_scene->setup();
	engine->m_scene->update();
}
void Engine::update_mode_elements(GLFWwindow* window)
{
	//data
	const Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//edges
	engine->m_draw->mode(Mode::Elements);
	//update
	engine->m_scene->setup();
	engine->m_scene->update();
}
void Engine::update_mode_deformed(GLFWwindow* window)
{
	//data
	const Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//edges
	engine->m_draw->mode(Mode::Deformed);
	//update
	engine->m_scene->setup();
	engine->m_scene->update();
}
void Engine::update_mode_geometry(GLFWwindow* window)
{
	//data
	const Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//edges
	engine->m_draw->mode(Mode::Geometry);
	//update
	engine->m_scene->setup();
	engine->m_scene->update();
}

//canvas
uint32_t Engine::canvas_modifiers(int32_t glfw_modifiers)
{
	//data
	int32_t glfw_list[] = {
		GLFW_MOD_ALT, GLFW_MOD_SHIFT, GLFW_MOD_CONTROL
	};
	canvas::modifier canvas_list[] = {
		canvas::modifier::alt, canvas::modifier::shift, canvas::modifier::control
	};
	//modifiers
	uint32_t canvas_modifiers = 0;
	for(uint32_t i = 0; i < 3; i++)
	{
		if(glfw_modifiers & glfw_list[i])
		{
			canvas_modifiers |= uint32_t(canvas_list[i]);
		}
	}
	//return
	return canvas_modifiers;
}
canvas::button Engine::canvas_button(int32_t glfw_button)
{
	//data
	int32_t glfw_list[] = {
		GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE
	};
	canvas::button canvas_list[] = {
		canvas::button::left, canvas::button::right, canvas::button::middle
	};
	//modifiers
	for(uint32_t i = 0; i < 3; i++)
	{
		if(glfw_button == glfw_list[i])
		{
			return canvas_list[i];
		}
	}
	//return
	return canvas::button::none;
}

//callbacks
void Engine::callback_wheel(GLFWwindow* window, double dx1, double dx2)
{
	//data
	Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//camera
	engine->m_scene->camera().callback_wheel(dx2 < 0);
	//update
	glfwSwapBuffers(window);
}
void Engine::callback_button(GLFWwindow* window, int32_t button, int32_t action, int32_t modifiers)
{
	//data
	double x1, x2;
	glfwGetCursorPos(window, &x1, &x2);
	Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//callback
	engine->m_scene->camera().callback_mouse(canvas_button(button), action == GLFW_PRESS, int32_t(x1), int32_t(x2), canvas_modifiers(modifiers));
	//update
	glfwSwapBuffers(window);
}
void Engine::callback_resize(GLFWwindow* window, int32_t width, int32_t height)
{
	//data
	Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//camera
	engine->m_scene->camera().callback_reshape(width, height);
}
void Engine::callback_position(GLFWwindow* window, double x1, double x2)
{
	Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	engine->m_scene->camera().callback_motion(int32_t(x1), int32_t(x2));
	glfwSwapBuffers(window);
}
void Engine::callback_keyboard(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
	//data
	if(action == GLFW_RELEASE) return;
	const bool shift = mods & GLFW_MOD_SHIFT;
	const bool control = mods & GLFW_MOD_CONTROL;
	const char* name = glfwGetKeyName(key, 0);
	Engine* engine = (Engine*) glfwGetWindowUserPointer(window);
	//model
	if(control && key == GLFW_KEY_O) update_model(window);
	//play
	else if(shift && key == GLFW_KEY_P) update_playing(window);
	//type
	else if(shift && key == GLFW_KEY_UP) update_type(window, true);
	else if(shift && key == GLFW_KEY_DOWN) update_type(window, false);
	//step
	else if(shift && key == GLFW_KEY_LEFT) update_step(window, false);
	else if(shift && key == GLFW_KEY_RIGHT) update_step(window, true);
	//what
	else if(control && key == GLFW_KEY_E) update_what_edges(window);
	else if(control && key == GLFW_KEY_F) update_what_faces(window);
	else if(control && key == GLFW_KEY_V) update_what_volume(window);
	//mode
	else if(shift && key == GLFW_KEY_M) update_mode_mesh(window);
	else if(shift && key == GLFW_KEY_N) update_mode_nodes(window);
	else if(shift && key == GLFW_KEY_E) update_mode_elements(window);
	else if(shift && key == GLFW_KEY_D) update_mode_deformed(window);
	else if(shift && key == GLFW_KEY_G) update_mode_geometry(window);
	//framerate
	else if(shift && key == GLFW_KEY_S) update_framerate(window, true);
	else if(control && key == GLFW_KEY_S) update_framerate(window, false);
	//glfw
	else if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
	//camera
	else if(name) engine->m_scene->camera().callback_key(*name);
	else if(key == GLFW_KEY_MINUS) engine->m_scene->camera().callback_wheel(true);
	else if(shift && key == GLFW_KEY_EQUAL) engine->m_scene->camera().callback_wheel(false);
}