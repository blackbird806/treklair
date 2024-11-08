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
	Camera  camera;
	int targetWidth, targetHeight;

	WorldEditor()
	{
		targetWidth = gameSizeX;
		targetHeight = gameSizeY;
		worldTarget = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, targetWidth, targetHeight);
	}

	bool open = true;
	bool displayGrid = true;

	World* world = nullptr;
	bool worldDirty = true;

	void drawGrid()
	{
		//SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
		if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
		ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

		draw_list->PushClipRect(canvas_p0, canvas_p1, true);

		int const gridSizeX = tile_size;
		int const gridSizeY = tile_size;
		for (int i = 0; i < targetWidth / gridSizeX; i++)
		{
			int const x = i * gridSizeX;
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
		}

		for (int i = 0; i < targetHeight / gridSizeY; i++)
		{
			int const y = i * gridSizeY;
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
		}
		draw_list->PopClipRect();

	}

	void drawWorld()
	{
		SDL_SetRenderLogicalPresentation(sdl_renderer, gameSizeX, gameSizeY, SDL_LOGICAL_PRESENTATION_LETTERBOX);
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

		ImGui::Image((ImTextureID)(intptr_t)worldTarget, canvas_sz);
		if (displayGrid)
			drawGrid();
		ImGuiIO& io = ImGui::GetIO();

		//io.MousePos

		ImGui::End();
	}

};