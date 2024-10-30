module;
#include <SDL3/SDL.h>
#include "vec2.h"
export module treklair:player;

struct Player
{
	vec2 pos;
	vec2 size;
	SDL_Texture* sprite;
};