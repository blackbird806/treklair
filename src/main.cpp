#include <format>
#include <print>
#include <unordered_map>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

import treklair;
import physics;

constexpr int gameSizeX = 640;
constexpr int gameSizeY = 480;
constexpr float gameAspectRatio = (float)gameSizeX / gameSizeY;

int main(int argc, char** argv)
{
	Rigidbody🗿* rb;

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

	// game rendertarget
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
	bool show_demo_window = false;

	bool done = false;
	std::unordered_map<SDL_Keycode, bool> input_map;

	World world(20, 10);
	Camera camera;

	while (!done)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
		
			if (event.type == SDL_EVENT_QUIT)
				done = true;

			else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
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

		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		SDL_SetRenderTarget(renderer, renderTarget);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		world.draw(renderer, camera);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_SetRenderTarget(renderer, nullptr);
		// TODO call only on resize
		{
			int w, h;
			SDL_GetCurrentRenderOutputSize(renderer, &w, &h);

			if (w > h)
				w = h * gameAspectRatio;
			else
				h = w / gameAspectRatio;

			gScaleFactor = (float)w / gameSizeX;

			SDL_RenderTexture(renderer, renderTarget, nullptr, nullptr);
		}
		// Rendering
		ImGui::Render();
		//SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		//SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//SDL_RenderClear(renderer);
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
