#pragma once
#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "gameMap.h"
#include "randomStuff.h"
#include "raymath.h"
#include <iostream>
#include <FastNoiseSIMD.h>



void generateWorld(GameMap& gameMap, unsigned int seed = 1234, int w = 2000, int h = 1000);

float* generateNoise(unsigned int seed, FastNoiseSIMD::NoiseType noise, unsigned int octaves, float frequency, float gain = 0.25f, int w = 1, int h = 1, int z = 1);
 
void generateWorm(GameMap& gameMap, float x, float y, int length, float radius = 2.5f);

#endif
