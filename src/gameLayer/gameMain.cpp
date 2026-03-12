#include "gameMain.h"

#include <raylib.h>
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <helpers.h>
#include <raymath.h>
#include <randomStuff.h>

#include "imgui.h"
#include "worldGenerator.h"

struct GameData
{
	GameMap gameMap;
	Camera2D camera;
}gameData;

AssetManager assetManager;

static std::ranlux24_base rng(std::random_device{}());

bool init_game()
{
	assetManager.loadAll();

	generateWorld(gameData.gameMap);

	// Camera
	gameData.camera.target = { 0, 0 };
	gameData.camera.rotation = 0.0f;
	gameData.camera.zoom = 100.0f;

	return true;
}

bool update_game()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1 / 5.f; }

	gameData.camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	ClearBackground({ 75, 75, 150, 255 });

#pragma region Camera Movement
	static float cameraSpeed = 7;

	if (IsKeyDown(KEY_LEFT)) gameData.camera.target.x -= cameraSpeed * deltaTime;
	if (IsKeyDown(KEY_RIGHT)) gameData.camera.target.x += cameraSpeed * deltaTime;
	if (IsKeyDown(KEY_UP)) gameData.camera.target.y -= cameraSpeed * deltaTime;
	if (IsKeyDown(KEY_DOWN)) gameData.camera.target.y += cameraSpeed * deltaTime;

#pragma endregion

#pragma region Create / Destroy Blocks
	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = static_cast<int>(floor(worldPos.x));
	int blockY = static_cast<int>(floor(worldPos.y));
	auto frameBlock = gameData.gameMap.getBlockSafe(blockX, blockY);// || gameData.gameMap.getWallSafe(blockX, blockY);


	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (b) *b = {};
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (b) b->type = Block::gold;
	}

#pragma endregion

	BeginMode2D(gameData.camera);

#pragma region Occulating Viewport 2D

	Vector2 topLeftView = GetScreenToWorld2D({ 0, 0 }, gameData.camera);
	Vector2 bottomRightView = GetScreenToWorld2D({ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) }, gameData.camera);

	int startXView = static_cast<int>(floorf(topLeftView.x - 1));
	int endXView = static_cast<int>(ceilf(bottomRightView.x + 1));
	int startYView = static_cast<int>(floorf(topLeftView.y - 1));
	int endYView = static_cast<int>(ceilf(bottomRightView.y + 1));

	startXView = Clamp(startXView, 0, gameData.gameMap.w - 1);
	endXView = Clamp(endXView, 0, gameData.gameMap.w - 1);

	startYView = Clamp(startYView, 0, gameData.gameMap.h - 1);
	endYView = Clamp(endYView, 0, gameData.gameMap.h - 1);

#pragma endregion

	for (int y = startYView; y < endYView; y++)
	{
		for (int x = startXView; x < endXView; x++)
		{
			auto& b = gameData.gameMap.getBlockUnsave(x, y);
			auto& w = gameData.gameMap.getWallUnsave(x, y);


			if (w.type != Wall::air)
			{
				DrawTexturePro(
					assetManager.walls, getTextureAtlas(w.type, w.variation, 32, 32),
					{ static_cast<float>(x), static_cast<float>(y), 1, 1 },
					{ 0, 0 },// Origin From Top Left Corner
					0.0f, // Rotation
					WHITE);// Tint
			}

			if (b.type != Block::air)
			{
				DrawTexturePro(
					assetManager.textures, getTextureAtlas(b.type, b.variation, 32, 32),
					{ static_cast<float>(x), static_cast<float>(y), 1, 1 },
					{ 0, 0 },// Origin From Top Left Corner
					0.0f, // Rotation
					WHITE);// Tint
			}
		}
	}

	DrawTexturePro(
		assetManager.frame, 
		{ 0, 0, static_cast<float>(assetManager.frame.width), static_cast<float>(assetManager.frame.height) },
		{ static_cast<float>(blockX), static_cast<float>(blockY), 1, 1 },
		{ 0, 0 },// Origin From Top Left Corner
		0.0f, // Rotation
		WHITE);// Tint

	EndMode2D();

	ImGui::Begin("Game Controller");
	ImGui::SliderFloat("Camera zoom:", &gameData.camera.zoom, 10, 150);
	ImGui::SliderFloat("Camera speed:", &cameraSpeed, 10, 150);
	ImGui::End();

	DrawFPS(10, 10);

	return true;
}

void close_game()
{

	gameData = {};

	std::cout << "Close Game \n";
	std::ofstream f(RESOURCES_PATH "debug.log");

	f << "\n CLOSED\n";
	f.close();
}