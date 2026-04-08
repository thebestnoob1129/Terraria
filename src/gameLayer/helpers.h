#pragma once 
#ifndef HELPERS_H
#define HELPERS_H
#include <raylib.h>
#include <physics.h>

Rectangle getTextureAtlas(int x, int y, int cellSizePixelsX = 32, int cellSizePixelsY = 32);

Rectangle getRectangeForEntity(const Transform2D& transform, float width = 1, float height = 1);
#endif
