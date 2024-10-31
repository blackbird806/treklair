module;
#include <SDL3/SDL.h>
export module treklair:player;
import maths;

struct Player
{
	vec2 pos;
	vec2 size;
	SDL_Texture* sprite;
};