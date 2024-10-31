module;
#include <SDL3/SDL.h>
export module treklair:player;
import maths;

struct Player
{
	🗿Vec2 pos;
	🗿Vec2 size;
	SDL_Texture* sprite;
};