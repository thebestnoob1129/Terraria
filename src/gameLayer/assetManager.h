#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <raylib.h>
#include <unordered_map>

struct AssetManager
{
	// Environment
	Texture2D dirt = {};
	Texture2D textures = {};
	Texture2D walls = {};
	Texture2D frame = {};
	Texture2D tree = {};
	Texture2D items = {};
	
	// Backgrounds

	// Forest
	Texture2D forestBG = {};
	Texture2D forestMountainsClose = {};
	Texture2D forestMountainsFar = {};
	Texture2D forestTrees = {};

	// Desert
	Texture2D desertBG = {};
	Texture2D dunesBack = {};
	Texture2D dunesFront = {};
	
	// Snow
	Texture2D snowBG = {};
	Texture2D snowMountainClose = {};
	Texture2D snowMountainFar = {};
	Texture2D snowHills = {};
	Texture2D snowTrees = {};

	// Sky
	Texture2D clouds = {};
	Texture2D cloudsNight = {};
	Texture2D moon = {};
	Texture2D moonStandAlone = {};
	Texture2D sky = {};
	Texture2D nightSky = {};
	Texture2D stars = {};
	Texture2D sun = {};
	Texture2D sunStandAlone = {};

	// Cave
	Texture2D caveBG = {};


	// Entities
	Texture2D player = {};
	Texture2D playerFeet = {};
	Texture2D playerBack = {};
	Texture2D playerFront = {};
	Texture2D playerHead = {};

	Texture2D slime = {};
	Texture2D blueSlime = {};
	Texture2D mummy = {};
	Texture2D zombie = {};
	Texture2D eskimo_zombie = {};
	
	// Armor
	std::unordered_map<int, Texture2D> frontArmor;
	std::unordered_map<int, Texture2D> backArmor;
	std::unordered_map<int, Texture2D> feetArmor;
	std::unordered_map<int, Texture2D> headArmor;

	// Functions
	void loadAll();
	void LoadTexturePack(int currentPack = -1);

	Texture2D getHeadTexture(int item);
	Texture2D getBackTexture(int item);
	Texture2D getFeetTexture(int item);
	Texture2D getFrontTexture(int item);

};

#endif
