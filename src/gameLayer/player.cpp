#include "player.h"

#include <assetManager.h>
#include <helpers.h>

void Player::render(AssetManager& assetManager)
{
	auto aabb = physics.transform.getAABB();

	DrawTexturePro(
		assetManager.player,
		getTextureAtlas( animation.positionX, animation.positionY, 32, 64 ),
		aabb,
		{ 0 , 0 },
		0.0f,
		WHITE
	);
}

bool Player::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	return true;
}
