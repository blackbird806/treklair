export module treklair:globals;

import :vec2;

export const float FLT_SMALL = 0.1f;

export constexpr int gameSizeX = 720;
export constexpr int gameSizeY = 480;
export constexpr float gameAspectRatio = (float)gameSizeX / gameSizeY;

export constexpr float depenetrationForce = 500.0f;
export constexpr float depenatrationMaxForce = 100.0f;
export constexpr Vec2 gravity = {0, 1000 };

export struct SDL_Window* sdl_window;
export struct SDL_Renderer* sdl_renderer;