#define _USE_MATH_DEFINES

#include <format>
#include <print>
#include <unordered_map>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <string>
#include <vector>
#include <cmath>

import treklair;

std::unordered_map<SDL_Keycode, bool> input_map;
std::unordered_map<SDL_Keycode, bool> input_map_pressed;
std::unordered_map<SDL_Keycode, bool> input_map_released;

static void setInput(SDL_Keycode key, bool value)
{
	if (value)
	{
		if (input_map[key] == false)
			input_map_pressed[key] = true;
		input_map[key] = true;
	}
	else
	{
		if (input_map[key] == true)
			input_map_released[key] = true;
		input_map[key] = false;
	}
};

int main(int argc, char** argv)
{
	std::srand(time(0));
	Uint64 timeNow = SDL_GetPerformanceCounter();
	Uint64 timeLast = 0;
	float deltaTime = 0;
	float timeDilation = 1.0f;

	Simulation simulation;

	Rigidbody base = Rigidbody(Box({ 1000,50 }));
	base.transform.position = { 500, 650 };
	base.inverseMass = 0;
	base.transform.rotation = 0;
	simulation.createRigidbody(base);
	base.transform.position = { 500, 50 };
	simulation.createRigidbody(base);

	Rigidbody side = Rigidbody(Box({50,800}));
	side.transform.position = { 50, 0};
	side.inverseMass = 0;
	side.transform.rotation = 0;
	simulation.createRigidbody(side);
	side.transform.position = {1050, 0};
	simulation.createRigidbody(side);

	Rigidbody box = Rigidbody(Box({ 100, 25 }));
	Rigidbody circle = Rigidbody(Circle({ 25 }));
	//circle.angularVelocity = 10;
	std::vector<Rigidbody*> createdBodies;
	
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

	
	Vec2 mousePos;
	bool keyPressed;

	GameRenderer gameRenderer;
	World world(1, 1);
	while (!done)
	{
		timeLast = timeNow;
		timeNow = SDL_GetPerformanceCounter();
		deltaTime = (float)((timeNow - timeLast) / (float)SDL_GetPerformanceFrequency());

		SDL_SetWindowTitle(sdl_window, ("treklair dt : " + std::to_string(deltaTime)).c_str());

		input_map_pressed.clear();
		input_map_released.clear();
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
				setInput(event.key.key, true);
			}
			else if (event.type == SDL_EVENT_KEY_UP)
			{
				setInput(event.key.key, false);
			}
			else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
			{
				setInput(event.key.key, true);
			}
			else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
			{
				setInput(event.key.key, false);
			}
		}

		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		mousePos -= Vec2(100, 0);
		engineRenderer.startFrame();

		if (input_map_pressed[SDLK_C] || input_map_pressed[SDLK_B])
		{
			Rigidbody rb = input_map_pressed[SDLK_C] ? circle : box;
			rb.transform.position = mousePos;
			rb.transform.rotation = 0;
			createdBodies.push_back(simulation.createRigidbody(rb));
		}

		if (input_map_pressed[SDLK_F1])
			simulation.simulate = !simulation.simulate;

		if (input_map_pressed[SDLK_F2])
			timeDilation = timeDilation == 0 ? 1 : 0;

		//clear created objects
		if (input_map_pressed[SDLK_F3])
		{
			simulation.removeRigidbodies(createdBodies);
			createdBodies.clear();
		}

		if (input_map_pressed[SDLK_F9])
			simulation.computeSimulation(1.0f / 10.0f);

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


		SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255);
		quickdraw::updateDebugDraw(deltaTime);

		SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 255, 255);
		simulation.debugDrawRigidbodies();

		SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255);
		simulation.update(deltaTime * timeDilation);

		engineRenderer.engineUI();

		ImGui::Begin("hello", nullptr);
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
