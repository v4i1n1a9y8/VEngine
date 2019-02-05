#include<iostream>
#include"vWindow.h"
#include"vContext.h"
#include"vEvent.h"

int main(int argc, char** argv) {
	v::Window window;
	v::Context context(window,3,3);

	v::Event::AddKeyEvent(
		SDLK_ESCAPE,nullptr,nullptr, [&]() {v::Event::running = false; }
	);
	v::log((char*)glGetString(GL_VERSION));
	while (v::Event::running) {
		v::Event::Poll();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		window.Update();
	}
	return 0;
}