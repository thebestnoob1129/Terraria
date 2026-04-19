#pragma once
#ifndef SAVEMAP_H
#define SAVEMAP_H

#include <vector>
#include <blocks.h>
#include <fstream>

#include "entityIdHolder.h"

bool saveBlockDataToFile(std::vector<Block> &blocks, std::vector<Wall> &walls, int w, int h, const char* fileName);

bool loadBlockDataFromFile(std::vector<Block> &blocks, std::vector<Wall> &walls, int &w, int &h, const char* fileName);

bool writeEntireFile(const char* fileName, const void* data, size_t size);

bool readEntireFile(const char* fileName, const void* data, size_t size);

struct GameMap;
struct Player;
struct EntityHolder;

void saveWorld(GameMap& gameMap, EntityHolder& entities, Player& player);
bool loadWorld(GameMap& gameMap, EntityHolder& entities, Player& player);

void saveWorldOnly(GameMap& gameMap);
void saveEntitiesOnly(EntityHolder& entities, Player& player);
 
#endif
