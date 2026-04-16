#pragma once
#ifndef MUMMY_H
#define MUMMY_H

#include <entity.h>
#include <raylib.h>

struct Mummy : public Entity
{
	Mummy()
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

	int getEntityList() override { return Entity_Mummy; }

	float getMaxLife() override { return 10; }
};
#endif