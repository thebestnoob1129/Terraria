#include "blueSlime.h"
#include "helpers.h"

void BlueSlime::render(AssetManager& assetManager)
{
	auto aabb = getRectangleForEntity(physics.transform, 1, 1);

	DrawTexturePro(
		assetManager.blueSlime,
		getTextureAtlas(animation.positionX, animation.positionY, 48, 48),
		aabb,
		{ 0, 0 },// Origin From Top Left Corner
		0.0f,
		tint
	);

}

bool BlueSlime::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	TickTint(deltaTime);

	changeStateTimer -= deltaTime;

	if (changeStateTimer <= 0)
	{

		changeStateTimer = getRandomFloat(entityUpdateData.rng, 3, 9);

		float distanceToPlayer = Vector2Distance(entityUpdateData.playerPosition, getPosition());

		if (distanceToPlayer < 20)
		{
			if (getRandomChance(entityUpdateData.rng, 0.8))
			{
				currentState = STATE_CHASING;
			}
			else
			{
				currentState = STATE_WONDERING;
			}
		}
		else
		{
			currentState = STATE_WONDERING;
		}

	}


	if (physics.downTouch)
	{
		moveSpeed = 0;

		animation.setAnimation(0);
	}
	else
	{
		animation.setAnimation(1);
	}
	jumpTimer -= deltaTime;

	switch (currentState)
	{
	case STATE_WONDERING:
	{
		if (jumpTimer < 0)
		{
			jumpTimer = getRandomFloat(entityUpdateData.rng, 3, 14);

			physics.jump(10);
			moveSpeed = getRandomFloat(entityUpdateData.rng, -9, 9);
		}
	}
	break;

	case STATE_CHASING:
	{
		if (jumpTimer < 0)
		{
			jumpTimer = getRandomFloat(entityUpdateData.rng, 1, 5);

			physics.jump(10);

			if (entityUpdateData.playerPosition.x > getPosition().x)
			{
				moveSpeed = getRandomFloat(entityUpdateData.rng, 3, 9);
			}
			else
			{
				moveSpeed = -getRandomFloat(entityUpdateData.rng, 3, 9);
			}
		}
	}
	break;
	default:
		break;
	}

	if (moveSpeed)
	{
		getPosition().x += deltaTime * moveSpeed;
	}

	animation.update(deltaTime, 0.08, 7);

	return true;

}

Json BlueSlime::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);

	return j;
}

bool BlueSlime::loadFromJson(Json &j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}
