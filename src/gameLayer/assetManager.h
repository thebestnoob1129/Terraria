#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <raylib.h>

struct AssetManager
{
	// Environment
	Texture2D dirt = {};
	Texture2D textures = {};
	Texture2D walls = {};
	Texture2D frame = {};
	Texture2D tree = {};
	Texture2D items = {};
	
	// Entities
	Texture2D player = {};
	Texture2D slime = {};
	Texture2D blueSlime = {};
	Texture2D mummy = {};
	Texture2D zombie = {};
	Texture2D eskimo_zombie = {};
	
	// Functions
	void loadAll();
	void LoadTexturePack(int currentPack = -1);
};

#endif
