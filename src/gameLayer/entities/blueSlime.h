#pragma once

#ifndef BLUESLIME_H
#define BLUESLIME_H

#include "slime.h"

struct BlueSlime : public Slime
{	
	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	int getEntityType() override { return EntityType_Slime; }

	int getEntityList() override { return Entity_BlueSlime; };

	float getMaxLife() override { return 15; };

};

#endif
