#include "assetManager.h"

#include <filesystem>
#include <string>

#include "items.h"

void AssetManager::loadAll()
{
	LoadTexturePack();
	
	tree = LoadTexture(RESOURCES_PATH "treetextures.png");
	items = LoadTexture(RESOURCES_PATH "items.png");
	hearts = LoadTexture(RESOURCES_PATH "hearts.png");

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

	slime = LoadTexture(RESOURCES_PATH "slime.png");
	blueSlime = LoadTexture(RESOURCES_PATH "blueslime.png");
	mummy = LoadTexture(RESOURCES_PATH "mummy.png");
	zombie = LoadTexture(RESOURCES_PATH "zombie.png");
	eskimo_zombie = LoadTexture(RESOURCES_PATH "zombie_eskimo.png");

	SetTextureFilter(forestBG, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(forestBG, TEXTURE_WRAP_MIRROR_REPEAT);
	
	//player = LoadTexture(RESOURCES_PATH "playerSprites.png");
	feetArmor[0] = LoadTexture(RESOURCES_PATH "body/player_feet.png");
	headArmor[0] = LoadTexture(RESOURCES_PATH "body/player_head.png");
	frontArmor[0] = LoadTexture(RESOURCES_PATH "body/player_front.png");
	backArmor[0] = LoadTexture(RESOURCES_PATH "body/player_back.png");

	headArmor[Item::partyHat] = LoadTexture(RESOURCES_PATH "body/party_hat.png");
	headArmor[Item::sunGlasses] = LoadTexture(RESOURCES_PATH "body/sunglasses.png");

	feetArmor[Item::copperBoots] = LoadTexture(RESOURCES_PATH "body/copper_armour_feet.png");
	headArmor[Item::copperHelmet] = LoadTexture(RESOURCES_PATH "body/copper_armour_head.png");
	frontArmor[Item::copperChestPlate] = LoadTexture(RESOURCES_PATH "body/copper_armour_front.png");
	backArmor[Item::copperChestPlate] = LoadTexture(RESOURCES_PATH "body/copper_armour_back.png");

	feetArmor[Item::ironBoots] = LoadTexture(RESOURCES_PATH "body/iron_armour_feet.png");
	headArmor[Item::ironHelmet] = LoadTexture(RESOURCES_PATH "body/iron_armour_head.png");
	frontArmor[Item::ironChestPlate] = LoadTexture(RESOURCES_PATH "body/iron_armour_front.png");
	backArmor[Item::ironChestPlate] = LoadTexture(RESOURCES_PATH "body/iron_armour_back.png");

	feetArmor[Item::goldBoots] = LoadTexture(RESOURCES_PATH "body/gold_armour_feet.png");
	headArmor[Item::goldHelmet] = LoadTexture(RESOURCES_PATH "body/gold_armour_head.png");
	frontArmor[Item::goldChestPlate] = LoadTexture(RESOURCES_PATH "body/gold_armour_front.png");
	backArmor[Item::goldChestPlate] = LoadTexture(RESOURCES_PATH "body/gold_armour_back.png");

	feetArmor[Item::iceBoots] = LoadTexture(RESOURCES_PATH "body/ice_armour_feet.png");
	headArmor[Item::iceHelmet] = LoadTexture(RESOURCES_PATH "body/ice_armour_head.png");
	frontArmor[Item::iceChestPlate] = LoadTexture(RESOURCES_PATH "body/ice_armour_front.png");
	backArmor[Item::iceChestPlate] = LoadTexture(RESOURCES_PATH "body/ice_armour_back.png");
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

Texture2D AssetManager::getHeadTexture(int item)
{
	auto found = headArmor.find(item);
	if (found != headArmor.end())
	{
		if (found->second.id == 0)
		{
			return headArmor[0];
		}
		return found->second;
	}
	return headArmor[0];
}
Texture2D AssetManager::getBackTexture(int item)
{
	auto found = backArmor.find(item);
	if (found != backArmor.end())
	{
		if (found->second.id == 0)
		{
			return backArmor[0];
		}
		return found->second;
	}
	return backArmor[0];
}
Texture2D AssetManager::getFeetTexture(int item)
{
	auto found = feetArmor.find(item);
	if (found != feetArmor.end())
	{
		if (found->second.id == 0)
		{
			return feetArmor[0];
		}
		return found->second;
	}
	return feetArmor[0];
}

Texture2D AssetManager::getFrontTexture(int item)
{
	auto found = frontArmor.find(item);
	if (found != frontArmor.end())
	{
		if (found->second.id == 0)
		{
			return frontArmor[0];
		}
		return found->second;
	}
	return frontArmor[0];
}

