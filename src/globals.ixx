export module treklair:globals;

export constexpr int gameSizeX = 720;
export constexpr int gameSizeY = 480;
export constexpr float gameAspectRatio = (float)gameSizeX / gameSizeY;

export constexpr float gravity = 1000;

export struct SDL_Window* sdl_window;
export struct SDL_Renderer* sdl_renderer;