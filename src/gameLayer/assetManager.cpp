#include "assetManager.h"

#include <filesystem>

void AssetManager::loadAll()
{
	dirt = LoadTexture(RESOURCES_PATH "Dirt\\Dirt_01-128x128.png");

	LoadTexturePack();

	tree = LoadTexture(RESOURCES_PATH "treetextures.png");
	player = LoadTexture(RESOURCES_PATH "player.png");
	slime = LoadTexture(RESOURCES_PATH "slime.png");
	/*
	brick = LoadTexture(RESOURCES_PATH "Brick\\Brick_01-128x128.png");
	metal = LoadTexture(RESOURCES_PATH "Metal\\Metal_01-128x128.png");
	plaster = LoadTexture(RESOURCES_PATH "Plaster\\Plaster_01-128x128.png");
	stone = LoadTexture(RESOURCES_PATH "Stone\\Stone_01-128x128.png");
	tile = LoadTexture(RESOURCES_PATH "Tile\\Tile_01-128x128.png");
	wood = LoadTexture(RESOURCES_PATH "Wood\\Wood_01-128x128.png");
	*/
}

void AssetManager::LoadTexturePack(int currentPack)
{
	if (currentPack == -1)
	{
		textures = LoadTexture(RESOURCES_PATH "textures.png");
		walls = LoadTexture(RESOURCES_PATH "texturesWithBackgroundVersion.png");
		frame = LoadTexture(RESOURCES_PATH "frame.png");
		return;
	}
	std::string path = RESOURCES_PATH "../texturePacks";
	std::vector<std::filesystem::directory_entry> packList;

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_directory())
		{
			packList.push_back(entry);
		}
	}

	std::string newPath = packList[currentPack].path().string();

	std::string texture = "/textures.png";
	std::string background = "/texturesWithBackgroundVersion.png";
	std::string _frame = "/frame.png";

	textures = LoadTexture((newPath + texture).c_str());
	walls = LoadTexture((newPath + background).c_str());
	frame = LoadTexture((newPath + _frame).c_str());
}

