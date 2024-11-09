module;
#include <SDL3/SDL.h>
#include "imgui.h"

export module treklair:worldEditor;

import :globals;
import :world;
import :camera;
import :sdlhelpers;

export class WorldEditor
{
public:
	
	SDL_Texture* worldTarget = nullptr;
	SDL_Texture* toolTarget = nullptr;

	Camera  camera;
	int worldTargetWidth, worldTargetHeight;
	int toolTargetWidth, toolTargetHeight;

	WorldEditor()
	{
		worldTargetWidth = gameSizeX;
		worldTargetHeight = gameSizeY;
		worldTarget = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, worldTargetWidth, worldTargetHeight);
		SDL_SetTextureScaleMode(worldTarget, SDL_SCALEMODE_NEAREST);
	}

	~WorldEditor()
	{
		SDL_DestroyTexture(worldTarget);
		SDL_DestroyTexture(toolTarget);
	}

	bool open = true;
	bool displayGrid = true;

	World* world = nullptr;
	bool worldDirty = true;

	void recreateToolTarget()
	{
		if (toolTarget)
			SDL_DestroyTexture(toolTarget);

		toolTarget = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, toolTargetWidth, toolTargetHeight);
		SDL_SetTextureScaleMode(toolTarget, SDL_SCALEMODE_NEAREST);
	}

	// https://github.com/ocornut/imgui/issues/1287
	// the tool target shall have the same size of the render region in the UI to avoid goofy scales
	void handleWindowResize()
	{
		ImVec2 view = ImGui::GetContentRegionAvail();

		if (view.x != toolTargetWidth || view.y != toolTargetHeight)
		{
			if (view.x == 0 || view.y == 0)
			{
				// The window is too small or collapsed.
				return;
			}

			toolTargetWidth = view.x;
			toolTargetHeight = view.y;

			recreateToolTarget();
		}

	}

	void drawGrid()
	{
		SDL_SetRenderTarget(sdl_renderer, toolTarget);

		SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
		if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
		ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

		int const gridSizeX = tile_size;
		int const gridSizeY = tile_size;

		float scaleFactorX = toolTargetWidth / worldTargetWidth;
		float scaleFactorY = toolTargetHeight / worldTargetHeight;

		for (int i = 0; i < (toolTargetWidth / gridSizeX) + 1; i++)
		{
			int const x = i * gridSizeX;
			SDL_RenderLine(sdl_renderer, x, 0, x, toolTargetHeight);
		}

		for (int i = 0; i < (toolTargetHeight / gridSizeY) + 1; i++)
		{
			int const y = i * gridSizeY;
			SDL_RenderLine(sdl_renderer, 0, y, toolTargetWidth, y);
		}

	}

	void drawWorld()
	{
		//SDL_SetRenderLogicalPresentation(sdl_renderer, gameSizeX, gameSizeY, SDL_LOGICAL_PRESENTATION_LETTERBOX);
		SDL_SetRenderTarget(sdl_renderer, worldTarget);
		SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
		SDL_RenderClear(sdl_renderer);

		SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255);
		for (int i = 0; auto const& tile : world->tiles)
		{
			int pos_x = (i % world->size_x) * tile_size;
			int pos_y = (i / world->size_x) * tile_size;
			SDL_FRect rect;
			rect.x = camera.worldToScreenX(pos_x);
			rect.y = camera.worldToScreenY(pos_y);
			rect.w = tile_size;
			rect.h = tile_size;
			SDL_RenderFillRect(sdl_renderer, &rect);
			i++;
		}
	}

	void drawUI()
	{
		if (!open)
			return;

		ImGui::Begin("World Editor", &open);
		ImGui::Checkbox("Display grid", &displayGrid);

		const char* items[] = { "None", "DebugWall" };
		static int item_current = 1;
		ImGui::ListBox("tiles", &item_current, items, IM_ARRAYSIZE(items), 4);

		if (worldDirty && world)
		{
			drawWorld();
		}
		
		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
		if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
		ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

		handleWindowResize();

		SDL_SetRenderTarget(sdl_renderer, toolTarget);
		SDL_RenderTexture(sdl_renderer, worldTarget, nullptr, nullptr);\
		if (displayGrid)
			drawGrid();

		ImGui::Image((ImTextureID)(intptr_t)toolTarget, canvas_sz);
		ImGuiIO& io = ImGui::GetIO();

		//io.MousePos

		ImGui::End();
	}

};