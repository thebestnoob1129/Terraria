#pragma once
#ifndef DRAWBACKGROUND_H
#define DRAWBACKGROUND_H

#include <raylib.h>
#include <audio.h>


float getCycleTime(float cycleLengthSeconds);
float getDayFactor(float cycle);

struct AssetManager;

struct DrawBackground
{
	enum BackgroundType
	{
		none = 0,
		forest,
		desert,
		snow,
		cave
	};

	int currentBackgroundType = forest;
	int currentMusic = Audio::musicForest;

	int currentTransitionType = none;
	int currentMusicTransition = none;

	float transitionTime = 0;
	float musicTransitionTime = 0;

	void draw(Texture2D bg, float deltaTime, Camera2D camera, Vector2 mapSize, float parallax = 0.3f, float opacity = 1);
	void draw(float deltaTime, AssetManager& assetManager, Camera2D camera, Vector2 mapSize, float parallax = 0.3f, float opacity = 1);

	void setBackground(int background);
};



#endif
