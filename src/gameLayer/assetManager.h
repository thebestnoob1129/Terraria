#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <raylib.h>
#include <string>

struct AssetManager
{
	Texture2D dirt = {};
	Texture2D textures = {};
	Texture2D walls = {};
	Texture2D frame = {};
	Texture2D tree = {};
	Texture2D player = {};
	Texture2D slime = {};
	void loadAll();
	void LoadTexturePack(int currentPack = -1);
};

#endif
