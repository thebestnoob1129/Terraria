#pragma once
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <vector>

#include <blocks.h>
#include <random>

#include <drawBackground.h>

struct WorldLayer
{
	Block block = {};
	Wall wall = {};

	int heightStart = 100;
	int heightEnd = 300;
	int smoothness = 4;
	float frequency = 0.05f;
};

struct Biome
{
	Block groundBlock = {};
	Block stoneBlock = {};
	Wall wall = {};

	int widthStart = 100;
	int widthEnd = 300;
	int heightStart = 100;
	int heightEnd = 300;

	int smoothness = 4;
	float frequency = 0.05f;

};

struct GameMap
{
	int w = 1;
	int h = 1;

	std::vector<Block> mapData = {};
	std::vector<Wall> wallData = {};
	std::vector<WorldLayer> worldLayers = {};
	std::vector<Biome> biomes = {};
	unsigned int seed = 0;
	std::ranlux24_base rng = {};

	bool shouldSave = true;// World & entities

	void create(int w, int h);

	Block &getBlockUnsave(int x, int y);
	Block* getBlockSafe(int x, int y);
	Wall &getWallUnsave(int x, int y);
	Wall* getWallSafe(int x, int y);

	int desertStart = 0;
	int desertEnd = 0;

	Vector2 getMapSize()
	{
		return Vector2{ static_cast<float>(w), static_cast<float>(h) };
	}
};

#endif
