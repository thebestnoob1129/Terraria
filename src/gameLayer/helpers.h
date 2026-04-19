#pragma once 
#ifndef HELPERS_H
#define HELPERS_H
#include <raylib.h>
#include <physics.h>

Rectangle getTextureAtlas(int x, int y, int cellSizePixelsX = 32, int cellSizePixelsY = 32, bool flipX = false);

// Returns an axis-aligned rectangle for rendering an entity centered on its transform
inline Rectangle getRectangleForEntity(const Transform2D& transform, float width = 1, float height = 1)
{
    return { transform.position.x - width * 0.5f, transform.position.y - height * 0.5f, width, height };
}

Rectangle flipTextureAtlasX(Rectangle r);

Rectangle shrinkUV(Rectangle in);
#endif
