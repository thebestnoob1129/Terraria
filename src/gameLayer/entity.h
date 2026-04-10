#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>
#include <assetManager.h>
#include <physics.h>
#include <random>
#include <entityIdHolder.h>

struct AssetManager;
struct EntityHolder;

enum EntityType {
	EntityType_Player = 0,
	EntityType_Slime,
	EntityType_DroppedItem,
	EntityType_Tool,
};

struct EntityUpdateData {


	Vector2 playerPosition = { 0, 0};

	std::ranlux24_base& rng;
	EntityHolder& entityHolder;

	std::uint64_t ownID = 0;

};

struct Entity {
	PhysicalEntity physics = {};

	Vector2& getPosition() {
		return physics.transform.position;
	}

	void teleport(Vector2 position) {
		physics.teleport(position);
	}

	virtual void render(AssetManager& assetManager) = 0;

	virtual bool update(float deltaTime, EntityUpdateData entityUpdateData) = 0;

	virtual int getEntityType() = 0;
	

};

#endif