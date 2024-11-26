module;
#include <SDL3/SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

export module treklair:engineRenderer;

import :globals;
import :worldEditor;

static bool show_demo_window = false;

export struct EngineRenderer
{
	ImGuiWindowFlags window_flags;
	ImGuiDockNodeFlags dockspace_flags;
	WorldEditor worldEditor;

	void initImgui()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		// multiviewport is noy suported by sdl renderer3 
		// https://github.com/ocornut/imgui/issues/5835
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL3_InitForSDLRenderer(sdl_window, sdl_renderer);
		ImGui_ImplSDLRenderer3_Init(sdl_renderer);

		window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	}

	void engineUI()
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("MainWindow", nullptr, window_flags);
		ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		ImGui::PopStyleVar(3);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("open demo window"))
				{
					show_demo_window = !show_demo_window;
				}
				if (ImGui::MenuItem("open world editor"))
				{
					worldEditor.open = !worldEditor.open;
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
		ImGui::End();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		worldEditor.drawUI();
	}

	void destroyImgui()
	{
		ImGui_ImplSDLRenderer3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	void startFrame()
	{
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}

	void draw()
	{
		//SDL_SetRenderTarget(sdl_renderer, nullptr);
		//SDL_SetRenderDrawColor(sdl_renderer, 10, 10, 10, 255);
		//SDL_RenderClear(sdl_renderer);

		int w, h;
		SDL_GetWindowSize(sdl_window, &w, &h);
		SDL_SetRenderLogicalPresentation(sdl_renderer, w, h, SDL_LOGICAL_PRESENTATION_DISABLED);

		// Rendering
		ImGui::Render();

		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdl_renderer);

		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		SDL_RenderPresent(sdl_renderer);
	}
};
