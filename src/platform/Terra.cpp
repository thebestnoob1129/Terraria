// Terra.cpp : Defines the entry point for the application.
//

#include "Terra.h"

using namespace std;

// const char *PlayerData = ResourcePath "./playerData:

int main()
{
#if PRODUCITON_BUILD == 1
	std::cout << "Production Build \n";
	SetTraceLogLevel(LOG_NONE);// No Console Log From Raylib
#endif


	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 400, "Terra: Demo");
	SetExitKey(KEY_NULL);
	SetTargetFPS(240);

#pragma region Imgui
	rlImGuiSetup(true);

	ImGuiIO &io = ImGui::GetIO();
	io.FontGlobalScale = 2;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsClassic();

#pragma endregion

	if (!init_game())
	{
		return 0;
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

#pragma region Imgui
		rlImGuiBegin();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);
		ImGui::PopStyleColor(2);
#pragma endregion 

		if (!update_game())
		{
			CloseWindow();
		}

#pragma region ImGui
		rlImGuiEnd();
#pragma endregion
		EndDrawing();
	}

	rlImGuiShutdown();

	CloseWindow();

	close_game();

	return 0;
}
