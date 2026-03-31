#pragma once
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <vector>

#include <blocks.h>
#include <asserts.h>
#include <random>
#include <randomStuff.h>

struct WorldLayer
{
	const char* name;
	Block block = {};
	Wall wall = {};
	int heightStart = 0;
	int heightEnd = 0;
	unsigned int height = 0;
	int smoothness = 0;
	float frequency = 0;
};

struct GameMap
{
	int w = 1;
	int h = 1;

	std::vector<Block> mapData = {};
	std::vector<Wall> wallData = {};
	std::vector<WorldLayer> layerData = {};
	unsigned int seed = 0;
	std::ranlux24_base rng = {};

	void create(int w, int h);

	Block &getBlockUnsave(int x, int y);
	Block* getBlockSafe(int x, int y);
	Wall &getWallUnsave(int x, int y);
	Wall* getWallSafe(int x, int y);

};

#endif
