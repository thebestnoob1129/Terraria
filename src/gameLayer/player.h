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
		setColliderSize();

		life = getMaxLife();
	}	

	Vector2& getPosition()
	{
		return physics.transform.position;
	}

	void setColliderSize() override
	{
		physics.transform.w = 0.8f;
		physics.transform.h = 1.6f;
	}

	void render(AssetManager& assetManager) override;

	int getEntityType() override { return EntityType_Player; }

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	float getMaxLife() override { return 40; }

	Json formatToJson() override ;

	bool loadFromJson(Json& j) override;

	int armorHead = Item::partyHat;
	int armorChest = Item::goldChestPlate;
	int armorLegs = Item::iceBoots;
	int heldItem = Item::goldSword;

};

#endif