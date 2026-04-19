#include "drawBackground.h"
#include <algorithm>
#include <raymath.h>
#include <assetManager.h>
#include <iostream>

#include "audio.h"

void DrawBackground::draw(Texture2D bg, float deltaTime, Camera2D camera,
                          Vector2 mapSize,
                          float parallax, float opacity)
{
	
	auto drawOneBackground = [&]()
	{
		int screenW = GetScreenWidth();
		int screenH = GetScreenHeight();

		float aspectRatio = static_cast<float>(bg.width) / static_cast<float>(bg.height);
		float bgScaleScreen = 2.0f;

		float base = std::max(screenW, screenH) * bgScaleScreen;
		
		// Scale background to always cover the screen
		float bgW = base;
		float bgH = base;

		if (aspectRatio > 1.0f)
		{
			// bgW is wider
			bgH = bgW / aspectRatio;
		}
		else
		{
			// bgH is taller
			bgW = bgH * aspectRatio;
		}

		// Compute Camera Ranges

		const float halfViewW = (screenW * 0.5f) / camera.zoom;
		const float halfViewH = (screenH * 0.5f) / camera.zoom;

		const float camMinX = halfViewW;
		const float camMaxX = mapSize.x - halfViewW;
		const float camMinY = halfViewH;
		const float camMaxY = mapSize.y - halfViewH;

		Vector2 camPos = camera.target;
		camPos.x = Clamp(camPos.x, camMinX, camMaxX);
		camPos.y = Clamp(camPos.y, camMinY, camMaxY);

		const float camRangeX = std::max(0.0f, camMaxX - camMinX);
		const float camRangeY = std::max(0.0f, camMaxY - camMinY);

		const float normX = camRangeX > 0.0f ? (camPos.x - camMinX) / camRangeX : 0.0f;
		const float normY = camRangeY > 0.0f ? (camPos.y - camMinY) / camRangeY : 0.0f;

		// How far the background moves in screen space
		const float maxOffX = bgW - screenW;
		const float maxOffY = bgH - screenH;

		const float offX = -maxOffX * normX * parallax;
		const float offY = -maxOffY * normY * parallax;

		Rectangle src = {0, 0, static_cast<float>(bg.width), static_cast<float>(bg.height )};
		Rectangle dest = {offX, offY, bgW, bgH};

		DrawTexturePro(bg, src, dest, { 0, 0 }, 0.0f, { 255, 255, 255, static_cast<unsigned char>(255 * opacity) });




	};

	drawOneBackground();

}
void DrawBackground::draw(float deltaTime, AssetManager& assetManager, Camera2D camera,
	Vector2 mapSize,
	float parallax, float opacity)
{

	auto drawOneBackground = [&](int type, float parall = 0.3f, float opac = 1)
	{

		Texture bg = assetManager.forestBG;

		switch (type)
		{
		case forest: bg = assetManager.forestBG; break;
		case desert: bg = assetManager.desertBG; break;
		case snow: bg = assetManager.snowBG; break;
		case cave: bg = assetManager.caveBG; break;
		}

		int screenW = GetScreenWidth();
		int screenH = GetScreenHeight();

		float aspectRatio = static_cast<float>(bg.width) / static_cast<float>(bg.height);
		float bgScaleScreen = 2.0f;

		float base = std::max(screenW, screenH) * bgScaleScreen;
		
		// Scale background to always cover the screen
		float bgW = base;
		float bgH = base;

		if (aspectRatio > 1.0f)
		{
			// bgW is wider
			bgH = bgW / aspectRatio;
		}
		else
		{
			// bgH is taller
			bgW = bgH * aspectRatio;
		}

		// Compute Camera Ranges

		const float halfViewW = (screenW * 0.5f) / camera.zoom;
		const float halfViewH = (screenH * 0.5f) / camera.zoom;

		const float camMinX = halfViewW;
		const float camMaxX = mapSize.x - halfViewW;
		const float camMinY = halfViewH;
		const float camMaxY = mapSize.y - halfViewH;

		Vector2 camPos = camera.target;
		camPos.x = Clamp(camPos.x, camMinX, camMaxX);
		camPos.y = Clamp(camPos.y, camMinY, camMaxY);

		const float camRangeX = std::max(0.0f, camMaxX - camMinX);
		const float camRangeY = std::max(0.0f, camMaxY - camMinY);

		const float normX = camRangeX > 0.0f ? (camPos.x - camMinX) / camRangeX : 0.0f;
		const float normY = camRangeY > 0.0f ? (camPos.y - camMinY) / camRangeY : 0.0f;

		// How far the background moves in screen space
		const float maxOffX = bgW - screenW;
		const float maxOffY = bgH - screenH;

		const float offX = -maxOffX * normX * parall;
		const float offY = -maxOffY * normY * parall;

		Rectangle src = {0, 0, static_cast<float>(bg.width), static_cast<float>(bg.height )};
		Rectangle dest = {offX, offY, bgW, bgH};

		DrawTexturePro(bg, src, dest, { 0, 0 }, 0.0f, { 255, 255, 255, static_cast<unsigned char>(255 * opac) });




	};

	drawOneBackground(currentBackgroundType, parallax, opacity);

	transitionTime -= deltaTime;
	if (transitionTime > 0)
	{
		float opacity = transitionTime;
		if (opacity > 1) { opacity = 1; }
		drawOneBackground(currentTransitionType, parallax, opacity);
	}

}

void DrawBackground::setBackground(int background)
{
	if (background != currentBackgroundType)
	{
		if (transitionTime <= 0)
		{
			transitionTime = 1;
			currentTransitionType = currentBackgroundType;
			currentBackgroundType = background;


		}
		else
		{
			
		}
	}
}


#include <chrono>
#include <cmath>

float getCycleTime(float cycleLengthSeconds)
{
	// Time To Wait Before Changing Cycles
	using namespace std::chrono;

	static auto start = steady_clock::now();
	auto now = steady_clock::now();

	float elapsed = duration<float>(now - start).count();

	auto f = fmod(elapsed, cycleLengthSeconds * 2.f) / cycleLengthSeconds;

	if (f > 1.0f)
	{
		f = 2.f - f;
	}
	return f;
}

#include <numbers>

float getDayFactor(float cycle)
{
	// 0 = night | 1 = day
	return cycle;
	
	//return ( sin( cycle * 2.0f * std::numbers::pi_v<float> - std::numbers::pi_v<float> / 2.0f ) + 1.0f ) / 2.0f;
}
