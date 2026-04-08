#include "helpers.h"

Rectangle getTextureAtlas(int x, int y, int cellSizePixelsX, int cellSizePixelsY)
{
	return Rectangle(
		static_cast<float>(x * cellSizePixelsX),
		static_cast<float>(y * cellSizePixelsY),
		static_cast<float>(cellSizePixelsX),
		static_cast<float>(cellSizePixelsY)
	);
}

Rectangle getRectangeForEntity(const Transform2D& transform, float width, float height)
{
	Transform2D rezult = transform;
	rezult.w = width;
	rezult.h = height;

	// Moves sprite to match bottom with collision box
	rezult.position.y -= (rezult.h - transform.h) / 2;

	return rezult.getAABB();
}
