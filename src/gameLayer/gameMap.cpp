#include "gameMap.h"
#include <random>
#include <asserts.h>
#include <randomStuff.h>
void GameMap::create(int w, int h)
{
	/*
	// Preserve seed if already set. If seed is 0, generate one from random_device.
	if (seed == 0)
	{
		seed = static_cast<unsigned int>(std::random_device{}());
	}
	*/
	// Initialize RNG from the stored seed so generation is reproducible when seed is changed.
	rng = std::ranlux24_base(seed);

	// Clear previous data but keep other metadata (like seed).
	mapData.clear();
	wallData.clear();
	worldLayers.clear();

	mapData.resize(w * h);
	wallData.resize(w * h);

	this->w = w;
	this->h = h;

	// Use To Set Random Variations Across tiles using the seeded rng.
	for (auto& e : mapData) { e = {}; e.variation = getRandomInt(rng, 0, 3); } // Clears all Block Data
	for (auto& e : wallData) { e = {}; e.variation = getRandomInt(rng, 0, 3); } // Clears all Block Data
}

// Gets All Tile Data
Block& GameMap::getBlockUnsave(int x, int y)
{
	permaAssertCommentDevelopement(mapData.size() == w * h, "Map Data Not Initialized");

	permaAssertCommentDevelopement(x >= 0 && y >= 0 && x < w && y < h, "getBlocUnsafe out of bounds error");
	
	return mapData[x + y * w];

}

Block* GameMap::getBlockSafe(int x, int y)
{
	permaAssertCommentDevelopement(mapData.size() == w * h, "Map Data Not Initialized");
	
	if (x < 0 || y < 0 || x >= w || y >= h) { return nullptr; }

	return &mapData[x + y * w];
}
Wall& GameMap::getWallUnsave(int x, int y)
{
	permaAssertCommentDevelopement(wallData.size() == w * h, "Wall Data Not Initialized");

	permaAssertCommentDevelopement(x >= 0 && y >= 0 && x < w && y < h, "getWallUnsafe out of bounds error");

	return wallData[x + y * w];

}

Wall* GameMap::getWallSafe(int x, int y)
{
	permaAssertCommentDevelopement(wallData.size() == w * h, "Wall Data Not Initialized");
	
	if (x < 0 || y < 0 || x >= w || y >= h) { return nullptr; }

	return &wallData[x + y * w];
}
