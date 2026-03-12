#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <raylib.h>
struct AssetManager
{
	Texture2D dirt = {};
	Texture2D textures = {};
	Texture2D walls = {};
	Texture2D frame = {};
	Texture2D tree = {};
	
	void loadAll();
};

#endif
