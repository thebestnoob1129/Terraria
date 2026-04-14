#include "droppedItem.h"

#include <assetManager.h>
#include <helpers.h>
#include <entityIdHolder.h>
#include <items.h>

void DroppedItem::render(AssetManager& assetManager) {


	Texture2D texture = getTextureForItemType(itemType, assetManager);
	Rectangle rectangle = getTextureCoordinatesForItemType(itemType);

	Rectangle aabb = getRectangleForEntity(physics.transform, 1, 1);
	if (itemType > Block::BLOCK_COUNT)
	{
		// Doesn't Apply To Collider
		aabb = getRectangleForEntity(physics.transform, 0.6f, 0.6f);
	}

	DrawTexturePro(
		texture,
		rectangle,
		aabb, // Destination
		{ 0, 0 }, // origin
		0.0f, // rotation
		WHITE // Tint
	);

}

bool DroppedItem::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	for (auto& e : entityUpdateData.entityHolder.entities) { // Scan All Entities
		if (e.first != entityUpdateData.ownID) { // Compare All IDs to itself
			if (e.second->getEntityType() == EntityType::EntityType_DroppedItem) { // Compare If Dropped Item
				DroppedItem* other = reinterpret_cast<DroppedItem*>(e.second.get());

				if (itemType == other->itemType) { // Compare Item Type
					if (Vector2Distance(getPosition(), other->getPosition()) < 0.7f) { // Pick Up Range

						/*
						if (other->itemCounter >= other->getMaxItems(other->itemType)) {
							continue;
						}
						if (itemCounter >= getMaxItems(itemType)) {
							continue;
						}
						*/
						other->itemCounter += itemCounter;
						return false;
					}
				}

			}
		}
	}

	return true;
}