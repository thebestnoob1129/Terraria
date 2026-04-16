#pragma once
#ifndef DRAWBACKGROUND_H
#define DRAWBACKGROUND_H

#include <raylib.h>

float getCycleTime(float cycleLengthSeconds);
float getDayFactor(float cycle);

struct DrawBackground
{
	void draw(Texture2D bg, float deltaTime, Camera2D camera, Vector2 mapSize, float parallax = 0.3f, float opacity = 1);
};



#endif
