#pragma once
#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "gameMap.h"
#include <saveMap.h>
#include "randomStuff.h"
#include "raymath.h"
#include <iostream>
#include <FastNoiseSIMD.h>
#include <structure.h>



void generateWorld(GameMap& gameMap, int w = 2000, int h = 1000);

float* generateNoise(int seed, FastNoiseSIMD::NoiseType noise, unsigned int octaves, float frequency, float gain = 0.25f, int w = 1, int h = 1, int z = 0);
 
void generateWorm(GameMap& gameMap, float x, float y, int length, float radius = 2.5f, bool isBlock = true);

void generateCave(GameMap& gameMap, int octaves, float frequency, float radius = 5);

static void generateTrees(GameMap& gameMap);

void createWorldLayer(GameMap& gameMap, Block ground, Vector2 size = Vector2{ 50, 100 }, Vector2 hills = Vector2{ 4, 0.01f }, bool hasGrass = false);

void createBiome(GameMap& gameMap, Block ground, Wall wall, Vector2 width, Vector2 height = Vector2{ 0.2f, 0.4f }, bool hasGrass = false);


Structure generateStructure(GameMap& gameMap, const char* filename);

#endif
