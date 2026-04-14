#include "assetManager.h"

#include <filesystem>
#include <string>

void AssetManager::loadAll()
{
	dirt = LoadTexture(RESOURCES_PATH "Dirt\\Dirt_01-128x128.png");
	tree = LoadTexture(RESOURCES_PATH "treetextures.png");
	items = LoadTexture(RESOURCES_PATH "items.png");

	LoadTexturePack();


	// Entities
	player = LoadTexture(RESOURCES_PATH "playerSprites.png");
	slime = LoadTexture(RESOURCES_PATH "slime.png");
	blueSlime = LoadTexture(RESOURCES_PATH "blueslime.png");
	mummy = LoadTexture(RESOURCES_PATH "mummy.png");
	zombie = LoadTexture(RESOURCES_PATH "zombie.png");
	eskimo_zombie = LoadTexture(RESOURCES_PATH "zombie_eskimo.png");

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

