#include <format>
#include <print>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

constexpr int gameSizeX = 640;
constexpr int gameSizeY = 480;
constexpr float gameAspectRatio = (float)gameSizeX / gameSizeY;

int main(int argc, char** argv)
{
	std::print("hello {}", "world");
	
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::print("failed to init sdl");
		return -1;
	}
	SDL_Window* window;
	SDL_Renderer* renderer;

	if (!SDL_CreateWindowAndRenderer("treklair", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer))
	{
		std::print("failed to create renderer and window");
		return -1;
	}

	SDL_Texture* renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, gameSizeX, gameSizeY);
	SDL_SetTextureScaleMode(renderTarget, SDL_SCALEMODE_NEAREST);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
	bool show_demo_window = true;

	bool done = false;

	while (!done)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			switch (event.type)
			{
				if (event.type == SDL_EVENT_QUIT)
					done = true;
				if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
					done = true;

			default:
				break;
			}
		}

		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);


		// Rendering
		ImGui::Render();
		//SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		//SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
		SDL_RenderPresent(renderer);
	}

	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyTexture(renderTarget);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

    return 0;
}
