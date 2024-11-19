#include <format>
#include <print>
#include <unordered_map>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <string>

import treklair;

int main(int argc, char** argv)
{
	Uint64 timeNow = SDL_GetPerformanceCounter();
	Uint64 timeLast = 0;
	float deltaTime = 0;

	Simulation simulation;

	AABB aabb = AABB({ 100,100 }, { 150,150 });
	AABB aabb2 = AABB({ 100,100 }, { 130,150 });
	Rigidbody c = Rigidbody(Circle(50));
	Rigidbody c2 = Rigidbody(Circle(50));
	Rigidbody b = Rigidbody(Box({50, 50}));
	Rigidbody b2 = Rigidbody(Box({ 75,75 }));
	Rigidbody base = Rigidbody(Box({ 1000,50 }));
	base.transform.position = { 0,500 };
	base.kinematic = true;
	base.transform.rotation = 0;
	c2.transform.position = Vec2(200, 200);
	b.transform.position = Vec2(300, 100);
	b.angularVelocity = 0;
	b.linearVelocity = Vec2(00, 0);
	simulation.createRigidbody(base);
	//simulation.createRigidbody(c);
	//simulation.createRigidbody(c2);
	Rigidbody* mouseBody = simulation.createRigidbody(b);
	//simulation.createRigidbody(b2);
	

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
	
	Vec2 mousePos;
	bool mousePressed;

	GameRenderer gameRenderer;
	World world(1, 1);
	while (!done)
	{
		timeLast = timeNow;
		timeNow = SDL_GetPerformanceCounter();
		deltaTime = (float)((timeNow - timeLast) / (float)SDL_GetPerformanceFrequency());

		SDL_SetWindowTitle(sdl_window, ("treklair dt : " + std::to_string(deltaTime)).c_str());

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
			else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
			{
				input_map[event.button.button] = true;
			}
			else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
			{
				input_map[event.button.button] = false;
			}
		}
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		mousePos -= Vec2(100, 0);
		engineRenderer.startFrame();

		if (input_map[SDL_BUTTON_LEFT])
		{
			if (!mousePressed)
			{
				b.transform.position = mousePos;
				simulation.createRigidbody(b);
				mousePressed = true;
			}
		}
		else
		{
			mousePressed = false;
		}

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

		SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 255, 255);
		quickdraw::updateDebugDraw(deltaTime);
		simulation.debugDrawRigidbodies();
		SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255);
		simulation.update(deltaTime);
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
