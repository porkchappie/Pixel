#include "pxl/application.h"

#include "glad/glad.h"
#include "SDL.h"

#include "pxl/init.h"
#include "pxl/events.h"

namespace pxl {
	struct engine {
		application* app;
		SDL_Window* window{ nullptr };
		SDL_GLContext context{ nullptr };
		SDL_Event event;
		union {
			key_event keyevent;
			pointer_event pointerevent;
		};
		void init() {
			app->on_create();
		}
		void key() {
			const auto& sdlkeyevent = event.key;
			keyevent.timestamp = sdlkeyevent.timestamp;
			keyevent.action = static_cast<key::action>(sdlkeyevent.state ^ 1 | (sdlkeyevent.repeat << 1));
			keyevent.scancode = static_cast<scancode>(sdlkeyevent.keysym.scancode);
			keyevent.keycode = static_cast<keycode>(sdlkeyevent.keysym.sym);
			keyevent.modifier = static_cast<modifier>(sdlkeyevent.keysym.mod);
			for (auto& listener : app->on.key)
				listener->on_key(keyevent);
		}
		void pointer() {
			const auto& sdlpointerevent = event.button;
			pointerevent.timestamp = sdlpointerevent.timestamp;
			pointerevent.action = static_cast<pointer::action>(sdlpointerevent.state ^ 1);
			pointerevent.which = sdlpointerevent.which;
			pointerevent.button = sdlpointerevent.button;
			pointerevent.clicks = sdlpointerevent.clicks;
			pointerevent.modifier = static_cast<modifier>(SDL_GetModState());
			pointerevent.x = sdlpointerevent.x;
			pointerevent.y = sdlpointerevent.y;
			for (auto& listener : app->on.pointer)
				listener->on_pointer(pointerevent);
		}
		void resize() {
			app->on_resize();
		}
		void update(float dt) {
			app->on_update(dt);
		}
	};
}

int main(int argc, char** argv) {
	using namespace pxl;

	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		engine engine;
		application* app{ nullptr };
		{
			const auto init = init_pxl();
			if (const auto window = SDL_CreateWindow(init.title,
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, init.width, init.height,
				SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN)) {
				if (const auto context = SDL_GL_CreateContext(window)) {
					engine.window = window;
					engine.context = context;

					gladLoadGLLoader(SDL_GL_GetProcAddress);
					app = init.create_app(&engine);
					engine.app = app;
				}

			}
		}


		if (app) {
			engine.init();
			auto& e = engine.event;
			bool run = true;
			do {
				while (SDL_PollEvent(&e)) {
					switch (e.type)
					{
					case SDL_KEYDOWN:
					case SDL_KEYUP:
						engine.key();
						break;
					case SDL_MOUSEBUTTONDOWN:
					case SDL_MOUSEBUTTONUP:
						engine.pointer();
						break;
					case SDL_WINDOWEVENT: {
						switch (e.window.event)
						{
						case SDL_WINDOWEVENT_RESIZED:
							engine.resize();
							break;
						default:
							break;
						}
						break;
					}
					case SDL_QUIT:
						run = false;
						break;
					default:
						break;
					}
				}

				engine.update(0);
			} while (run); // TODO
			return 0;
		}
	}

	return -1;
}