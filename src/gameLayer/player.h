#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <physics.h>
#include <entity.h>
#include <assetManager.h>

struct AssetManager;

struct Player : public Entity
{
	Player()
	{
		physics.transform.w = 0.8f;
		physics.transform.h = 1.6f;

		life = getMaxLife();
	}	

	Vector2& getPosition()
	{
		return physics.transform.position;
	}

	void render(AssetManager& assetManager) override;

	int getEntityType() override { return EntityType_Player; };

	int getEntityList() override { return Entity_Player; };

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	float getMaxLife() override { return 10; }

};

#endif