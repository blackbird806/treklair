#include <format>
#include <print>
#include <SDL3/SDL.h>

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

	bool endGame = false;

	while (!endGame)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				endGame = true;
				break;

			default:
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(renderTarget);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

    return 0;
}
