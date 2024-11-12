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
	🗿AABB aabb = 🗿AABB({ 100,100 }, { 150,150 });
	🗿AABB aabb2 = 🗿AABB({ 100,100 }, { 130,150 });
	🗿Rigidbody c = 🗿Rigidbody(🗿Circle(50));
	🗿Rigidbody c2 = 🗿Rigidbody(🗿Circle(50));
	🗿Rigidbody b = 🗿Rigidbody(🗿Box({50, 50}));
	🗿Rigidbody b2 = 🗿Rigidbody(🗿Box({ 75,75 }));
	c2.transform.position = 🗿Vec2(200, 200);
	b.transform.position = 🗿Vec2(300, 300);

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
	
	🗿Vec2 mousePos;

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
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		mousePos -= 🗿Vec2(100, 0);
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

		🗿Contact contacts[2];
		b2.transform.position = mousePos;
		b.transform.rotation = 1;
		b2.transform.rotation = 2;
		//aabb2.min = mousePos - 🗿Vec2(50, 50);
		//aabb2.max = mousePos + 🗿Vec2(50,50);
		//if(physics::AABBOverlap(aabb, aabb2))
		//if (physics::BoxCircleOverlap(b.box, c.circle, b.transform, c.transform))
		int contactCount = physics::BoxOverlap(b.box, b2.box, b.transform, b2.transform, contacts);
		if (contactCount > 1)
		//if (physics::BoxOverlap(b2.box, b.box, b2.transform, b.transform))
		//if (physics::CircleOverlap(c.circle, c2.circle, c.transform, c2.transform))
			SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255);
		else
			SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 255, 255);

		for (int i = 0; i < contactCount; i++)
			quickdraw::DrawContact(contacts[i]);

		quickdraw::DrawRigidbody(b);
		quickdraw::DrawRigidbody(b2);
		//quickdraw::DrawRigidbody(c2);
		//quickdraw::DrawAABB(aabb);
		//quickdraw::DrawAABB(aabb2);

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
