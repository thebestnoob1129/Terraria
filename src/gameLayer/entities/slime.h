#pragma once
#ifndef SLIME_H
#define SLIME_H

#include <raylib.h>
#include <entityAnimation.h>
#include <entity.h>

struct AssetManager;

struct Slime : Entity
{
	PhysicalEntity physics;
	EntityAnimation animation;
	
	Slime()
	{
		physics.transform.w = 0.9f;
		physics.transform.h = 0.9f;
		moveSpeed = 5;
	}
	
	Vector2& getPosition()
	{
		return physics.transform.position;
	}

	void render(AssetManager& assetManager) override;
	
	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	int getEntityType() { return EntityType_Slime; }

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