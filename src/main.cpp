#include <format>
#include <print>
#include <unordered_map>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

import treklair;

int main(int argc, char** argv)
{
	std::print("hello {}", "world");

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::print("failed to init sdl");
		return -1;
	}

	if (!SDL_CreateWindowAndRenderer("treklair", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &sdl_window, &sdl_renderer))
	{
		std::print("failed to create renderer and window");
		return -1;
	}
	
	EngineRenderer engineRenderer;
	engineRenderer.initImgui();
	bool done = false;
	std::unordered_map<SDL_Keycode, bool> input_map;
	World world(1, 1);
	engineRenderer.worldEditor.world = &world;
	while (!done)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
		
			if (event.type == SDL_EVENT_QUIT)
				done = true;

			else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(sdl_window))
				done = true;

			else if (event.type == SDL_EVENT_KEY_DOWN)
			{
				input_map[event.key.key] = true;
			}
			else if (event.type == SDL_EVENT_KEY_UP)
			{
				input_map[event.key.key] = false;
			}
		}
		
		engineRenderer.startFrame();

		engineRenderer.engineUI();

		engineRenderer.draw();
	}

	engineRenderer.destroyImgui();

	SDL_DestroyRenderer(sdl_renderer);
	SDL_DestroyWindow(sdl_window);

    return 0;
}
