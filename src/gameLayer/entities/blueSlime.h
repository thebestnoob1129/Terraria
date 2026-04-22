#pragma once

#ifndef BLUESLIME_H
#define BLUESLIME_H

#include "slime.h"

struct BlueSlime : public Slime
{	
	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	int getEntityType() override { return Entity_BlueSlime; }


	float getMaxLife() override { return 15; };

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	// Maybe Add Json Override
};

#endif
