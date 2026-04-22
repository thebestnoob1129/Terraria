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
		setColliderSize();
		moveSpeed = 5;

		life = getMaxLife();

	}
	

	Vector2& getPosition()
	{
		return physics.transform.position;
	}

	void render(AssetManager& assetManager) override;
	
	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	int getEntityType() override { return Entity_Slime; }

	float getMaxLife() override { return 10; }

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	void setColliderSize() override
	{
		physics.transform.w = 0.9f;
		physics.transform.h = 0.9f;
	}
};

#endif