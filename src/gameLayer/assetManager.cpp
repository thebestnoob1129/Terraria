#include "assetManager.h"

#include <filesystem>
#include <string>

void AssetManager::loadAll()
{
	LoadTexturePack();
	
	tree = LoadTexture(RESOURCES_PATH "treetextures.png");
	items = LoadTexture(RESOURCES_PATH "items.png");

	// Backgrounds
	forestBG = LoadTexture(RESOURCES_PATH "forestBG.png");
	forestMountainsClose = LoadTexture(RESOURCES_PATH "forest/mountainsClose.png");
	forestMountainsFar = LoadTexture(RESOURCES_PATH "forest/mountainsFat.png");
	forestTrees = LoadTexture(RESOURCES_PATH "forest/trees.png");
	
	// Desert
	desertBG = LoadTexture(RESOURCES_PATH "desertBG.png");
	dunesBack = LoadTexture(RESOURCES_PATH "desert/dunesBack.png");
	dunesFront = LoadTexture(RESOURCES_PATH "desert/dunesFront.png");

	// Snow
	snowBG = LoadTexture(RESOURCES_PATH "snowBG.png");
	snowMountainClose = LoadTexture(RESOURCES_PATH "snow/mountainsBack.png");
	snowMountainFar = LoadTexture(RESOURCES_PATH "snow/mountainsFront.png");
	snowHills = LoadTexture(RESOURCES_PATH "snow/optionalHillnFrontOfTrees.png");
	snowTrees = LoadTexture(RESOURCES_PATH "snow/trees.png");
	
	// Sky
	clouds = LoadTexture( RESOURCES_PATH "sky/clouds.png");
	cloudsNight = LoadTexture( RESOURCES_PATH "sky/cloudsNight.png");
	moon = LoadTexture( RESOURCES_PATH "sky/moon.png");
	moonStandAlone = LoadTexture( RESOURCES_PATH "sky/moonStandAlone.png");
	sky = LoadTexture( RESOURCES_PATH "sky/sky.png");
	nightSky = LoadTexture( RESOURCES_PATH "sky/nightSky.png");
	stars = LoadTexture( RESOURCES_PATH "sky/stars.png");
	sun = LoadTexture( RESOURCES_PATH "sky/sun.png");
	sunStandAlone = LoadTexture(RESOURCES_PATH "sky/sunStandAlone.png");

	// Cave
	caveBG = LoadTexture(RESOURCES_PATH "caveBG.png");

	// Entities
	player = LoadTexture(RESOURCES_PATH "playerSprites.png");
	slime = LoadTexture(RESOURCES_PATH "slime.png");
	blueSlime = LoadTexture(RESOURCES_PATH "blueslime.png");
	mummy = LoadTexture(RESOURCES_PATH "mummy.png");
	zombie = LoadTexture(RESOURCES_PATH "zombie.png");
	eskimo_zombie = LoadTexture(RESOURCES_PATH "zombie_eskimo.png");

	SetTextureFilter(forestBG, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(forestBG, TEXTURE_WRAP_MIRROR_REPEAT);
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

