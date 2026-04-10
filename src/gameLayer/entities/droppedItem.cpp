#include "droppedItem.h"

#include <assetManager.h>
#include <helpers.h>
#include <entityIdHolder.h>

void DroppedItem::render(AssetManager& assetManager) {

	auto aabb = getRectangleForEntity(physics.transform, 1, 1);

	DrawTexturePro(
		assetManager.textures,
		getTextureAtlas(itemType, 4, 32, 32),
		aabb, // Destination
		{ 0, 0 }, // origin
		0.0f, // rotation
		WHITE // Tint
	);

}

bool DroppedItem::update(float deltaTime, EntityUpdateData entityUpdateData)
{

	for (auto& e : entityUpdateData.entityHolder.entities) {
		if (e.first != entityUpdateData.ownID) {
			if (e.second->getEntityType() == EntityType::EntityType_DroppedItem) {
				DroppedItem* other = reinterpret_cast<DroppedItem*>(e.second.get());

				if (itemType == other->itemType) {
					if (Vector2Distance(getPosition(), other->getPosition()) < 0.7f) {
						
						if (other->itemCounter >= other->getMaxItems(other->itemType)) {
							continue;
						}
						if (itemCounter >= getMaxItems(itemType)) {
							continue;
						}
						
						other->itemCounter += itemCounter;
						return 0;
					}
				}

			}
		}
	}

	return true;
}