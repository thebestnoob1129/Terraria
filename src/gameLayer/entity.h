#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>
#include <assetManager.h>
#include <physics.h>
#include <random>
#include <memory>
#include <entityIdHolder.h>
#include <entityAnimation.h>
#include <iostream>
#include <randomStuff.h>
#include <string>
#include <audio.h>

struct AssetManager;
struct EntityHolder;
struct EntityAnimation;

enum EntityList
{
	Entity_Player = 0,
	Entity_DroppedItem,
	Entity_Slime,
	Entity_BlueSlime,
	Entity_Mummy,
	Entity_Zombie,
	Entity_Zombie_Eskimo,
	ENTITY_COUNT
};

enum EntityType {
	EntityType_Player = 0,
	EntityType_Slime,
	EntityType_Humanoid,
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
	EntityAnimation animation = {};
	Color tint = WHITE;
	Color baseTint = WHITE;
	float tintTimer = 0.0f;
	float life = 1;

	Vector2& getPosition() {
		return physics.transform.position;
	}

	void teleport(Vector2 position) {
		physics.teleport(position);
	}

	// Call this each frame from derived update() implementations to handle tint timing.
	void TickTint(float deltaTime = 0) {
		if (tintTimer > 0.0f) {
			tintTimer -= deltaTime;//GetFrameTime();
			if (tintTimer <= 0.0f) {
				tintTimer = 0.0f;
				tint = baseTint;
			}
		}
	}

	virtual void render(AssetManager& assetManager) = 0;

	virtual bool update(float deltaTime, EntityUpdateData entityUpdateData) = 0;

	virtual int getEntityType() = 0;

	virtual int getEntityList() = 0;
	
	virtual float getMaxLife() = 0;

	virtual void OnHit(float damage, Color col = Color{255, 0, 0, 255})
	{
		std::cout << "Target Hit \n";
		
		std::ranlux24_base rng(std::random_device{}());

		if (getEntityType() != EntityType_DroppedItem) {
			if (life - damage <= 0)
			{
				Audio::playSound(getRandomInt(rng, Audio::FinalHit1, static_cast<int>(Audio::FinalHit1) + 2));
			}
			else
			{
				Audio::playSound(getRandomInt(rng, Audio::Hit1, static_cast<int>(Audio::Hit1) + 2));
			}
		}
		life -= damage;

		// Flash the entity by setting tint and a timer (2 seconds)
		tint = col;
		tintTimer = 0.1f;
	}

	virtual void ChangeTint(Color color, float timer = 2.0f)
	{
		// Non-blocking: set tint and duration; call TickTint from update to process.
		tint = color;
		tintTimer = timer;
		if (timer <= 0.0f) {
			tintTimer = 0.0f;
			tint = baseTint;
		}
	}

	virtual int getDrop()
	{
		std::ranlux24_base rng(std::random_device{}());

		// Search Through Array of item IDS

		// Select Item Id From Array and Return

		int item = getRandomInt(rng, 0, 32);
		return item;
	}

	enum AI_State
	{
		STATE_WONDERING = 0,
		STATE_CHASING,
		STATE_ATTACKING
	};


	int currentState = STATE_WONDERING;
	float changeStateTimer = 1;
	float jumpTimer = 0;
	float moveSpeed;
};

#endif