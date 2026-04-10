#pragma once
#ifndef DROPPEDITEM_H
#define DROPPEDITEM_H

#include <physics.h>
#include <raylib.h>
#include <random>
#include <entity.h>

struct AssetManager;
struct EntityUpdateData;

struct DroppedItem : public Entity 
{
	DroppedItem() {
		physics.transform.w = 0.8f;
		physics.transform.h = 0.8f;
	}

	int itemType = 0;
	int itemCounter = 1;

	void render(AssetManager& assetManager);

	bool update(float deltaTime, EntityUpdateData entityUpdateData);

	int getEntityType() { return EntityType_DroppedItem; }

	int getMaxItems(int itemType) {

		if (itemType == EntityType_Tool) {
			itemCounter = std::clamp(itemCounter, 1, 1);
		}

		if (itemType == EntityType_DroppedItem) {
			itemCounter = std::clamp(itemCounter, 1, 999);
		}

		return itemCounter;

	}
};

#endif 