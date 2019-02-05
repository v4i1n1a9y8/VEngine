#include "vWindow.h"



v::Window::Window()
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"lol",
		SDL_WINDOWPOS_CENTERED
		, SDL_WINDOWPOS_CENTERED
		, 800, 600,
		SDL_WINDOW_OPENGL
	);
	size = glm::vec2(800, 600);
	v::log("Window Created");
}

v::Window::Window(const char *t, int w, int h, Uint32 f, int x, int y)
{
	window = SDL_CreateWindow(
		t,
		x
		, y
		, w, h,
		f
	);
	size = glm::vec2(w, h);
}


void v::Window::Update()
{
	SDL_GL_SwapWindow(window);
}

v::Window::~Window()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
	v::log("Window Destroyed");
}
