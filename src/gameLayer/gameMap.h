#pragma once
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <vector>

#include <blocks.h>
#include <walls.h>
#include <random>

struct GameMap
{
	int w;
	int h;
	 
	std::vector<Block> mapData;
	std::vector<Wall> wallData;
	std::ranlux24_base seed;

	void create(int w, int h);

	Block &getBlockUnsave(int x, int y);
	Block* getBlockSafe(int x, int y);
	Wall &getWallUnsave(int x, int y);
	Wall* getWallSafe(int x, int y);

};

#endif
