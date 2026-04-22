#pragma once
#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <raylib.h>

#include <structure.h>
#include <player.h>
#include <drawBackground.h>

#include <saveMap.h>
#include <worldGenerator.h>
#include <inventory.h>

struct GamePlay
{
	GameMap gameMap = {};
	bool showImgui = false;

	// Camera Information
	Camera2D camera = {};
	bool insideInventory = false;

	// World Information
	DrawBackground background;
	bool useGravity = true;
	float currentTime = 0;

	// Creative Information
	int creativeSelectedBlock = Block::dirt;
	Structure copyStructure = {};
	Vector2 selectionStart = {};
	Vector2 selectionEnd = {};

	// Save Information
	char saveName[100]{};

	// Player Inventory
	Player player = {};
	Inventory inventory = {};

	// Entity Information
	EntityHolder entities;
	int maxEnemies = 50;
	int spawnRange = 50; // Blocks
	float spawnTimer = 10;

	Rectangle getInventoryRectangle(float w, float h);

	void spawnSlime(Vector2 position);

	void spawnDroppedItem(Vector2 position, int type);

	void spawnRandomEnemy(Vector2 playerPosition);

	bool init(AssetManager &assetManager);
	bool update(AssetManager &assetManager);

};

#endif
