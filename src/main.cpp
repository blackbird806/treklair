#define _USE_MATH_DEFINES

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

import treklair;
import std;

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

Gyrosystem gyrosystem;

static void createGyrosystem(Simulation& simulation)
{
	Rigidbody wheel = Rigidbody(Circle({ 50 }));
	Rigidbody body = Rigidbody(Box({ 25, 25 }));
	//body.inverseMass = 1.0/100.0;
	wheel.transform.position = { 0, -300 };
	body.transform.position = wheel.transform.position + Vec2(0, -85);
	gyrosystem.wheel = simulation.createRigidbody(wheel);
	gyrosystem.body = simulation.createRigidbody(body);
	simulation.ignoreBodies(gyrosystem.wheel, gyrosystem.body);
	gyrosystem.spring = simulation.createDistanceConstraint(SpringContraint(gyrosystem.wheel, gyrosystem.body, 0, 1500, 100));
	gyrosystem.init();
	simulation.addUpdateStruct(Gyrosystem::static_update, &gyrosystem);
}

static void createLevel(Simulation& simulation)
{
	Rigidbody base = Rigidbody(Box({ 1000,50 }));
	base.transform.position = { 0, 0 };
	base.inverseMass = 0;
	base.transform.rotation = 0;
	simulation.createRigidbody(base);
	base.transform.position = { 2300, -300 };
	simulation.createRigidbody(base);

	Rigidbody half = Rigidbody(Box({ 500,50 }));
	half.transform.position = { -1000, 0 };
	half.inverseMass = 0;
	half.transform.rotation = M_PI/2;
	simulation.createRigidbody(half);
	half.transform.position = { 3200, -500 };
	simulation.createRigidbody(half);


	half.transform.rotation = -M_PI / 4;
	half.transform.position = { 1000, 0 };
	simulation.createRigidbody(half);

	Rigidbody small = Rigidbody(Box({ 250,50 }));
	half.transform.position = { 50, 500 };
	//simulation.createRigidbody(small);
}


int main(int argc, char** argv)
{
	std::srand(std::time(0));
	Uint64 timeNow = SDL_GetPerformanceCounter();
	Uint64 timeLast = 0;
	float deltaTime = 0;
	float timeDilation = 1.0f;

	Simulation simulation;

	createLevel(simulation);

	Rigidbody rect = Rigidbody(Box({ 200, 10 }));
	Rigidbody circle = Rigidbody(Circle({ 25 }));
	Rigidbody square = Rigidbody(Box({ 25, 25 }));
	std::vector<Rigidbody*> createdBodies;

	SpringContraint distanceConstraint = SpringContraint(nullptr, nullptr, 100, 300, 1000 );
	
	createGyrosystem(simulation);

	std::print("launching gyrobot sim \nUse arrow keys to move, \nX / C / V to spawn bodies \nZ to toggle spring joint spawning");

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::print("failed to init sdl");
		return -1;
	}

	if (!SDL_CreateWindowAndRenderer("Gyrobot", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &sdl_window, &sdl_renderer))
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
	bool isSpawningSpring = false;

	GameRenderer gameRenderer;
	World world(1, 1);
	while (!done)
	{
		timeLast = timeNow;
		timeNow = SDL_GetPerformanceCounter();
		deltaTime = (float)((timeNow - timeLast) / (float)SDL_GetPerformanceFrequency());

		//SDL_SetWindowTitle(sdl_window, ("treklair dt : " + std::to_string(deltaTime)).c_str());

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

		if (input_map_pressed[SDLK_C] || input_map_pressed[SDLK_B] || input_map_pressed[SDLK_V])
		{
			Rigidbody rb = input_map_pressed[SDLK_C] ? circle : input_map_pressed[SDLK_B] ? rect : square;
			rb.transform.position = mousePos + gameRenderer.camera.pos;
			createdBodies.push_back(simulation.createRigidbody(rb));

			if (isSpawningSpring == true && createdBodies.size() % 3 == 0)
			{
				distanceConstraint.firstBody = createdBodies.back();
				distanceConstraint.secondBody = createdBodies[createdBodies.size() - 2];
				simulation.createDistanceConstraint(distanceConstraint);
				distanceConstraint.secondBody = createdBodies[createdBodies.size() - 3];
				simulation.createDistanceConstraint(distanceConstraint);
				distanceConstraint.firstBody = createdBodies[createdBodies.size() - 2];
				simulation.createDistanceConstraint(distanceConstraint);
			}
		}

		if (input_map_pressed[SDLK_Z] == true)
		{
			isSpawningSpring = !isSpawningSpring;
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
			//simulation.clearConstraints();
		}

		if (input_map_pressed[SDLK_F9])
			simulation.computeSimulation(1.0f / 10.0f);

		Vec2 directionalInput;
		directionalInput.x = (input_map[SDLK_RIGHT] ? 1 : 0) - (input_map[SDLK_LEFT] ? 1 : 0);
		directionalInput.y = - (input_map[SDLK_DOWN] ? 1 : 0);
		gyrosystem.setBendInput(directionalInput.x);
		gyrosystem.setSpringInput(directionalInput.y);

		quickdraw::setCameraPosition(gameRenderer.camera.pos = (gyrosystem.wheel->transform.position) - Vec2(gameSizeX, gameSizeY) * 0.75f);

		SDL_SetRenderLogicalPresentation(sdl_renderer, gameSizeX, gameSizeY, SDL_LOGICAL_PRESENTATION_LETTERBOX);
		SDL_SetRenderTarget(sdl_renderer, renderTarget);
		SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
		SDL_RenderClear(sdl_renderer); 

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
		SDL_SetRenderDrawColor(sdl_renderer, 0, 255, 0, 255);
		simulation.debugDrawConstraints();

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
};

