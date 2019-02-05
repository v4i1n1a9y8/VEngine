#pragma once
#include<SDL2/SDL.h>
#include<glm/vec2.hpp>
#include"vUtils.h"
#include"vWindow.h"
#include<GL/glew.h>

namespace v {
	class Context {
	public:
		Context(Window& window, int major = 3, int minor = 3, SDL_GLprofile profile = SDL_GL_CONTEXT_PROFILE_CORE) {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			context = window.getGLContext();
			glewExperimental = true;
			glewInit();
			v::log("context created");
		}
		~Context() {
			SDL_GL_DeleteContext(context);
			v::log("context destroyed");
		}
	private:
		SDL_GLContext context;
	};
}
