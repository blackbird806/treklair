module;
#include <SDL3/SDL.h>
#include "vec2.h"

export module treklair:gameRenderer;

import :globals;
import :world;
import :camera;

export struct GameRenderer
{
	void init()
	{
		mainRenderTarget = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, gameSizeX, gameSizeY);
		SDL_SetTextureScaleMode(mainRenderTarget, SDL_SCALEMODE_NEAREST);
	}

	void drawWorld(World const& world) const
	{
		for (int i = 0; auto const& tile : world.tiles)
		{
			int pos_x = (i % world.size_x) * tile_size;
			int pos_y = (i / world.size_x) * tile_size;
			SDL_FRect rect;
			rect.x = camera.worldToScreenX(pos_x);
			rect.y = camera.worldToScreenY(pos_y);
			rect.w = tile_size;
			rect.h = tile_size;
			SDL_RenderFillRect(sdl_renderer, &rect);
			i++;
		}
	}

	void drawGame(World const& world)
	{
		SDL_SetRenderLogicalPresentation(sdl_renderer, gameSizeX, gameSizeY, SDL_LOGICAL_PRESENTATION_LETTERBOX);
		SDL_SetRenderTarget(sdl_renderer, mainRenderTarget);
		SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
		SDL_RenderClear(sdl_renderer);

		SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255);
		drawWorld(world);
		SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
	}

	~GameRenderer()
	{
		if (mainRenderTarget)
			SDL_DestroyTexture(mainRenderTarget);
	}

	Camera camera;
	SDL_Texture* mainRenderTarget = nullptr;
};

