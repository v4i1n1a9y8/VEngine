#pragma once
#include<SDL2/SDL.h>
#include<glm/vec2.hpp>
#include"vUtils.h"

namespace v {
	class Window
	{
	public:
		Window();
		Window(const char*, int, int, Uint32, int x=SDL_WINDOWPOS_CENTERED,int y=SDL_WINDOWPOS_CENTERED);

		void Update();
		~Window();
		SDL_GLContext getGLContext() {
			return SDL_GL_CreateContext(window);
		}
	private:
		SDL_Window* window;
		glm::vec2 size;
	};
	
}

