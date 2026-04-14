#pragma once
#ifndef SLIME_H
#define SLIME_H

#include <raylib.h>
#include <entity.h>

struct AssetManager;

struct Slime : Entity
{
	Slime()
	{
		physics.transform.w = 0.9f;
		physics.transform.h = 0.9f;
		moveSpeed = 5;

		life = getMaxLife();

	}
	

	Vector2& getPosition()
	{
		return physics.transform.position;
	}

	void render(AssetManager& assetManager) override;
	
	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	int getEntityType() override { return EntityType_Slime; }

	int getEntityList() override { return Entity_Slime; };

	float getMaxLife() override { return 10; };
};

#endif