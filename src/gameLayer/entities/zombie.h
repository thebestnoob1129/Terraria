#pragma once
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <entity.h>
#include <raylib.h>

struct Zombie : public Entity
{
	Zombie()
	{
		setColliderSize();
		moveSpeed = 7;

		life = getMaxLife();

	}

	Vector2& getPosition()
	{
		return physics.transform.position;
	}

	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	int getEntityType() override { return Entity_Zombie; }

	float getMaxLife() override { return 10; }

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	void setColliderSize() override
	{
		physics.transform.w = 0.8f;
		physics.transform.h = 1.6f;
	}
};

#endif