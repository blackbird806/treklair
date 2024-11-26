export module treklair:globals;

import :vec2;

export constexpr int gameSizeX = 720;
export constexpr int gameSizeY = 480;
export constexpr float gameAspectRatio = (float)gameSizeX / gameSizeY;

export constexpr Vec2 gravity = {0, 200 };

export struct SDL_Window* sdl_window;
export struct SDL_Renderer* sdl_renderer;