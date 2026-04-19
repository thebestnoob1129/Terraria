#include "helpers.h"

Rectangle getTextureAtlas(int x, int y, int cellSizePixelsX, int cellSizePixelsY, bool flipX)
{
	float sizeX = cellSizePixelsX;
	if (flipX) { sizeX *= -1; }

	return shrinkUV( Rectangle(
		static_cast<float>(x * cellSizePixelsX), // +1
		static_cast<float>(y * cellSizePixelsY), // +1
		static_cast<float>(sizeX), // -1
		static_cast<float>(cellSizePixelsY) )// -1
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

Rectangle flipTextureAtlasX(Rectangle r)
{
	r.width = -r.width;
	return r;
}

Rectangle shrinkUV(Rectangle in)
{
	float shrink = 0.1f;
	in.width -= shrink;
	in.height -= shrink;
	in.x += shrink / 2;
	in.y += shrink / 2;

	return in;
}
