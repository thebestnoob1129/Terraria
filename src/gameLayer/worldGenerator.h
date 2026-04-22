#pragma once
#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "gameMap.h"
#include <saveMap.h>
#include <randomStuff.h>
#include <FastNoiseSIMD.h>
#include <structure.h>



void generateWorld(GameMap& gameMap, int w = 2000, int h = 1000);

float* generateNoise(int seed, FastNoiseSIMD::NoiseType noise, unsigned int octaves, float frequency, float gain = 0.25f, int w = 1, int h = 1, int z = 0);
 
void generateCave(GameMap& gameMap, int octaves, float frequency, float radius = 5);

void generateWorm(GameMap& gameMap, float x, float y, int length, float radius = 2.5f, bool isBlock = true, Block block = Block{Block::air, 0});

void generatePerlin(GameMap& gameMap, Block block, Vector2 worm = {25, 25}, float chance = 0.5f);

void generateOre(GameMap& gameMap, Block ore, Wall wall, int octaves, float frequency, float radius);

static void generateTrees(GameMap& gameMap);

//void createWorldLayer(GameMap& gameMap, Block ground, Vector2 size = Vector2{ 50, 100 }, Vector2 hills = Vector2{ 4, 0.01f }, bool hasGrass = false);
void createWorldLayer(GameMap& gameMap, WorldLayer layer, bool hasGrass = false);

//void createBiome(GameMap& gameMap, Block ground, Wall wall, Vector2 width, Vector2 height = Vector2{ 0.2f, 0.4f }, bool hasGrass = false);
void createBiome(GameMap& gameMap, Biome biome, bool hasGrass = false);
void createBiome2(GameMap& gameMap, Biome biome, bool hasGrass = false);

Structure generateStructure(GameMap& gameMap, const char* filename);

#endif
