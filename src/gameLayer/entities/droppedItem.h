#pragma once
#ifndef DROPPEDITEM_H
#define DROPPEDITEM_H

#include <entity.h>


struct AssetManager;

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

	int getEntityList() override { return Entity_DroppedItem; }

	float getMaxLife() override { return 1.f; }

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