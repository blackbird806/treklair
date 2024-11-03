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

	// game rendertarget
	SDL_Texture* renderTarget = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, gameSizeX, gameSizeY);
	SDL_SetTextureScaleMode(renderTarget, SDL_SCALEMODE_NEAREST);

	EngineRenderer engineRenderer;
	engineRenderer.initImgui();
	bool done = false;
	std::unordered_map<SDL_Keycode, bool> input_map;
	
	GameRenderer gameRenderer;
	World world(1, 1);
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

		SDL_SetRenderLogicalPresentation(sdl_renderer, gameSizeX, gameSizeY, SDL_LOGICAL_PRESENTATION_LETTERBOX);
		SDL_SetRenderTarget(sdl_renderer, renderTarget);
		SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
		SDL_RenderClear(sdl_renderer);

		SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255);
		gameRenderer.drawWorld(world);
		SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);

		SDL_SetRenderTarget(sdl_renderer, nullptr);
		SDL_SetRenderDrawColor(sdl_renderer, 10, 10, 10, 255);
		SDL_RenderClear(sdl_renderer);

		// TODO call only on resize
		{
			int w, h;
			SDL_GetCurrentRenderOutputSize(sdl_renderer, &w, &h);
			SDL_SetRenderLogicalPresentation(sdl_renderer, w, h, SDL_LOGICAL_PRESENTATION_DISABLED);
			SDL_RenderTexture(sdl_renderer, renderTarget, nullptr, nullptr);
		}
		ImGui::Begin("hello");
		ImGui::Button("test");
		ImGui::End();

		engineRenderer.draw();
	}

	engineRenderer.destroyImgui();

	SDL_DestroyTexture(renderTarget);
	SDL_DestroyRenderer(sdl_renderer);
	SDL_DestroyWindow(sdl_window);

    return 0;
}
