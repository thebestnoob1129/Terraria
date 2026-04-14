#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include <raymath.h>

struct Settings
{
	float masterVolume = 1;
	float musicVolume = 0.75f;
	float soundsVolume = 0.75f;

	void sanitize()
	{
		masterVolume = Clamp(masterVolume, 0.0f, 1.0f);
		musicVolume = Clamp(musicVolume, 0.0f, 1.0f);
		soundsVolume = Clamp(soundsVolume, 0.0f, 1.0f);
	}

	bool operator==(const Settings& other) const
	{
		return musicVolume == other.musicVolume &&
			masterVolume == other.masterVolume &&
			soundsVolume == other.soundsVolume;
	}

	bool operator!=(const Settings& other) const
	{
		return !(*this == other);
	}
};

Settings& getSettings();

void saveSettings();
void loadSettings();
void updateSettings();

#endif
