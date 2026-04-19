#include "player.h"

#include <assetManager.h>
#include <helpers.h>

void Player::render(AssetManager& assetManager)
{
	auto aabb = physics.transform.getAABB();
	/*
	DrawTexturePro(
		assetManager.player,
		getTextureAtlas( animation.positionX, animation.positionY, 32, 64 ),
		aabb,
		{ 0 , 0 },
		0.0f,
		WHITE
	);
	*/

	auto textureUV = getTextureAtlas(animation.positionX, animation.positionY, 32, 64, animation.movingLeft);

	if (heldItem)
	{
		textureUV = getTextureAtlas(animation.positionX, animation.positionY + 3, 32, 64, animation.movingLeft);
	}

	// Player
	DrawTexturePro(
		assetManager.getBackTexture(armorChest),
		textureUV,
		aabb,
		{ 0 , 0 },
		0.0f,
		WHITE
	);
	DrawTexturePro(
		assetManager.getFeetTexture(armorLegs),
		textureUV,
		aabb,
		{ 0 , 0 },
		0.0f,
		WHITE
	);
	DrawTexturePro(
		assetManager.getHeadTexture(armorHead),
		textureUV,
		aabb,
		{ 0 , 0 },
		0.0f,
		WHITE
	);
	if (heldItem)
	{
		Texture texture = getTextureForItemType(heldItem, assetManager);
		Rectangle textureUVItem = getTextureCoordinatesForItemType(heldItem);

		auto pos = aabb;

		// if is block{
		if (heldItem < Block::BLOCK_COUNT)
		{
			pos.width = 0.4f;
			pos.height = 0.4f;
			if (animation.movingLeft)
			{
				pos.y += 0.5f;
				pos.x -= 0.2f;
			}
			else
			{
				pos.y += 0.5f;
				pos.x += 0.6f;
			}
		}
		else
		{
			pos.width = 1;
			pos.height = 1;
			if (animation.movingLeft)
			{
				pos.y += 0.1f;
				pos.x -= 0.7f;
				textureUVItem = flipTextureAtlasX(textureUVItem);
			}
			else
			{
				pos.y += 0.1f;
				pos.x += 0.5f;
			}
		}

		DrawTexturePro(
			texture,
			textureUVItem,
			pos,
			{ 0, 0 },
			0.0f,
			WHITE);
	}
	DrawTexturePro(
		assetManager.getFrontTexture(armorChest),
		textureUV,
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

Json Player::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);

	//j["key"] = data;

	return j;
}

bool Player::loadFromJson(Json& j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}
