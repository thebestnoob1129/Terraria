#include "zombie.h"

#include <assetManager.h>
#include <helpers.h>

void Zombie::render(AssetManager& assetManager)
{
	auto aabb = physics.transform.getAABB();

	DrawTexturePro(
		assetManager.zombie,
		getTextureAtlas(animation.positionX, animation.positionY, 32, 64),
		aabb,
		{ 0, 0 },// Origin From Top Left Corner
		0.0f,
		tint
	);

}

bool Zombie::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	TickTint(deltaTime);
	changeStateTimer -= deltaTime;
	float distanceToPlayer = Vector2Distance(entityUpdateData.playerPosition, getPosition());

	// Jump Action
	auto doJump = [&](bool touch)
		{
			if (jumpTimer < 0 || touch)
			{
				jumpTimer = getRandomFloat(entityUpdateData.rng, 3, 12);

				physics.jump(10);
				moveSpeed = getRandomFloat(entityUpdateData.rng, -7, 7);
			}
		};


	// Check State
	if (changeStateTimer <= 0)
	{

		changeStateTimer = getRandomFloat(entityUpdateData.rng, 1, 7);

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
		else if (distanceToPlayer < 2)
		{
			currentState = STATE_ATTACKING;
		}
		else
		{
			currentState = STATE_WONDERING;
		}

	}

	// Check If grounded
	if (physics.downTouch)
	{
		moveSpeed = 3;

		animation.setAnimation(1);
	}
	else
	{
		animation.setAnimation(2);
	}
	jumpTimer -= deltaTime;


	// Entity Change State
	switch (currentState)
	{
	case STATE_WONDERING:
	{
		// Jump Over Obstacles
		if (physics.velocity.x <= 0.001f || physics.velocity.x >= -0.001f)
		{
			if (physics.rightTouch)
			{
				doJump(true);
			}
			else if (physics.leftTouch)
			{
				doJump(true);
			}
		}

		moveSpeed = getRandomInt(entityUpdateData.rng, -5, 5);
	}
	break;

	case STATE_CHASING:
	{
			// Put whole if statement in doJump
		if (physics.velocity.x <= 0.001f || physics.velocity.x >= -0.001f)
		{
			if (physics.rightTouch)
			{
				doJump(true);
			}
			else if (physics.leftTouch)
			{
				doJump(true);
			}
		}

		if (entityUpdateData.playerPosition.x > getPosition().x)
		{
			moveSpeed = getRandomFloat(entityUpdateData.rng, 1, 7);
		}
		else
		{
			moveSpeed = -getRandomFloat(entityUpdateData.rng, 1, 7);
		}
	}
	break;

	case STATE_ATTACKING:
	{
		// Move Towards Player
		if (entityUpdateData.playerPosition.x > getPosition().x)
		{
			moveSpeed = getRandomFloat(entityUpdateData.rng, 1, 7);
		}
		else
		{
			moveSpeed = -getRandomFloat(entityUpdateData.rng, 1, 7);
		}

		if (distanceToPlayer < 2)
		{
			animation.setAnimation(3);
		}
		else
		{
			animation.setAnimation(1);
		}

		// Attack Player On Collision

	}
	default:
		break;
	}

	if (moveSpeed)
	{
		getPosition().x += deltaTime * moveSpeed;
	}

	animation.update(deltaTime, 0.08f, 7);

	return true;

}

Json Zombie::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);
	//j["key"] = data;

	return j;
}

bool Zombie::loadFromJson(Json& j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}
