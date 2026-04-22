#pragma once
#ifndef MUMMY_H
#define MUMMY_H

#include <entity.h>
#include <raylib.h>

struct Mummy : public Entity
{
	Mummy()
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

	int getEntityType() override { return Entity_Mummy; }

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