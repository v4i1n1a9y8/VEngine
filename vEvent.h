#pragma once
#include<iostream>
#include<vector>
#include<functional>
#include<SDL2/SDL.h>
#include<map>

namespace v {
	namespace Event
	{
		namespace
		{
			
			static std::vector<std::function<void()>> beforeQuit;
			static std::vector<std::function<void()>> afterQuit;
			struct KeyEvent {
				SDL_Keycode code;
				std::function<void()> Down;
				std::function<void()> Up;
				std::function<void()> Pressed;
				bool pressedDown;
			};
			static std::vector<KeyEvent> onKey;
		}
		bool running = true;

		void Poll() {
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					for (auto f : beforeQuit) f();
					running = false;
					for (auto f : afterQuit) f();
				}
				if (event.type == SDL_KEYDOWN) {
					for (auto &e : onKey) {
						if (event.key.keysym.sym == e.code){
							if (e.Down) e.Down();
							if (!e.pressedDown)
							{
								if(e.Pressed) e.Pressed();
								e.pressedDown = true;
							}
						}
					}
				}
				if (event.type == SDL_KEYUP) {
					for (auto &e : onKey) {
						if (event.key.keysym.sym == e.code) {
							if (e.Up) e.Up();
							e.pressedDown = false;
						}
					}
				}
			}
		}
	
		void AddKeyEvent(SDL_Keycode key,std::function<void()> Down, std::function<void()> Up, std::function<void()> Pressed) {
			KeyEvent ke = { key,Down,Up,Pressed,false };
			onKey.push_back(ke);
		}
	}
}
