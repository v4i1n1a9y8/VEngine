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
			static std::vector<std::function<void(int,int)>> onResize;
			struct MouseMotionEvent {
				int x;
				int y;
				Sint32 rx;
				Sint32 ry;
			};
			static std::vector<std::function<void(MouseMotionEvent)>> onMouseMove;
			struct KeyEvent {
				SDL_Keycode code;
				std::function<void()> Down;
				std::function<void()> Up;
				std::function<void()> Pressed;
				bool pressedDown;
			};
			static std::vector<KeyEvent> onKey;
			static std::map<SDL_Keycode, bool> keyStatus;
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
				if (event.type == SDL_WINDOWEVENT) {
					if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
						for (auto f : onResize) {
							if (f) f(event.window.data1,event.window.data2);
						}
					}
				}
				if (event.type == SDL_KEYDOWN) {
					keyStatus[event.key.keysym.sym] = true;
				}
				if (event.type == SDL_KEYUP) {
					keyStatus[event.key.keysym.sym] = false;
				}
				if (event.type == SDL_MOUSEMOTION) {
					for (auto &f : onMouseMove) {
						f(MouseMotionEvent{event.motion.x,event.motion.x ,event.motion.xrel ,event.motion.yrel });
					}
				}
			}
			for (auto &ke : onKey) {
				if (keyStatus[ke.code]) {
					if (ke.Down) ke.Down();
					if (!ke.pressedDown)
					{
						if (ke.Pressed) ke.Pressed();
						ke.pressedDown = true;
					}
				}
				else {
					if (ke.Up) ke.Up();
					ke.pressedDown = false;
				}
			}
		}
	
		void AddKeyEvent(SDL_Keycode key,std::function<void()> Down, std::function<void()> Up, std::function<void()> Pressed) {
			KeyEvent ke = { key,Down,Up,Pressed,false };
			onKey.push_back(ke);
		}
	}
}
