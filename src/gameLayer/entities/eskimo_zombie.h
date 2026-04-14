#pragma once
#ifndef ESKIMO_ZOMBIE_H
#define ESKIMO_ZOMBIE_H

#include "entity.h"
#include <raylib.h>

struct EskimoZombie : public Entity
{
	EskimoZombie()
	{
		physics.transform.w = 0.8f;
		physics.transform.h = 1.6f;
		moveSpeed = 7;

		life = getMaxLife();

	}


	Vector2& getPosition()
	{
		return physics.transform.position;
	}

	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	int getEntityType() override { return EntityType_Humanoid; }

	int getEntityList() override { return Entity_Zombie_Eskimo; }

	float getMaxLife() override { return 10; }
};

#endif