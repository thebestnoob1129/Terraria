#pragma once
#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <raylib.h>
#include <vector>
#include <gameMap.h>
#include <blocks.h>

struct Structure
{
	int w = 0;
	int h = 0;

	std::vector<Block> mapData;
	std::vector<Wall> wallData;

	void create(int w, int h);

	Block& getBlockUnsave(int x, int y);
	Block* getBlockSafe(int x, int y);
	Wall& getWallUnsave(int x, int y);
	Wall* getWallSafe(int x, int y);

	void copyFromMap(GameMap& map, Vector2 start, Vector2 end);
	void pasteIntoMap(GameMap& map, Vector2 start);

};
#endif