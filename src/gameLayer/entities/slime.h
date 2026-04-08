#pragma once
#ifndef SLIME_H
#define SLIME_H

#include <physics.h>
#include <raylib.h>
#include <random>
#include <entityAnimation.h>

struct AssetManager;

struct Slime
{
	PhysicalEntity physics;
	EntityAnimation animation;

	Slime()
	{
		physics.transform.w = 0.9f;
		physics.transform.h = 0.9f;
	}

	Vector2& getPosition()
	{
		return physics.transform.position;
	}

	void render(AssetManager& assetManager);
	void update(float deltaTime, std::ranlux24_base rng, Vector2 playerPosition);

	enum
	{
		STATE_WONDERING = 0,
		STATE_CHASING,
	};

	int currentState = STATE_WONDERING;
	float changeStateTimer = 1;
	float jumpTimer = 0;
	float moveSpeed;
};

#endif