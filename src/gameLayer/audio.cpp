#include "audio.h"
#include <raylib.h>
#include <vector>
#include <cmath>
#include <asserts.h>
#include <iostream>
#include <settings.h>
#include <raymath.h>
#include <string>

namespace Audio
{
	
	void init()
	{
		InitAudioDevice();
		SetMasterVolume(0.75f);

		loadAllMusicAndSounds();
	}

	std::vector<Sound> allSounds;
	std::vector<Sound> soundsPlayed;

	std::vector<Music> allMusic;
	int currentMusicPlaying = 0;

	void loadAllMusicAndSounds()
	{
		allMusic.push_back({});

		auto loadMusic = [&](const char* path)
		{
			Music m;
			m = LoadMusicStream(path);

			if (m.stream.buffer)
			{
				m.looping = true;
				allMusic.push_back(m);
			}
			else
			{
				allMusic.push_back({});
			}
		};

		loadMusic(RESOURCES_PATH "music/forest.ogg");
		loadMusic(RESOURCES_PATH "music/desert.ogg");
		loadMusic(RESOURCES_PATH "music/snow.ogg");
		loadMusic(RESOURCES_PATH "music/cave.ogg");

		permaAssertComment(allMusic.size() == MUSIC_COUNT, "Forgot to add a song here!");
		// All Sounds

		auto loadSound = [&](const char* path)
		{
			Sound s;

			s = LoadSound(path);

			if (s.stream.buffer)
			{
				allSounds.push_back(s);
			}
			else
			{
				allSounds.push_back({}); // Nullptr
			}
		};

		auto loadVariation = [&](const char* path, int count)
		{
			for (int i = 1; i <= count; i++)
			{
				Sound s;

				std::string newPath = std::string(path) + std::to_string(i) + std::string(".ogg");

				s = LoadSound(newPath.c_str());

				if (s.stream.buffer)
				{
					allSounds.push_back(s);
				}
				else
				{
					allSounds.push_back({});
				}
			}
		};

		// Null Sound
		allSounds.push_back({});

		// Load All Sounds

		loadSound(RESOURCES_PATH "sounds/place.ogg");
		loadSound(RESOURCES_PATH "sounds/break.ogg");

		loadVariation(RESOURCES_PATH "sounds/bonus/FinalHit", 3);

		loadVariation(RESOURCES_PATH "sounds/bonus/HImpact", 3);

		loadVariation(RESOURCES_PATH "sounds/bonus/Hit", 3);

		loadSound(RESOURCES_PATH "sounds/bonus/Slider.ogg");

		loadVariation(RESOURCES_PATH "sounds/bonus/Stone", 7);

		loadVariation(RESOURCES_PATH "sounds/bonus/Wood", 7);

		permaAssertComment(allSounds.size() == SOUNDS_COUNT, "Forgot to add a sound here");
	}

	void playMusic(int index)
	{
		if (allMusic.size() <= index) { return; }
		if (currentMusicPlaying == index) { return; }

		StopMusicStream(allMusic[currentMusicPlaying]);

		allMusic[index].looping = true;
		PlayMusicStream(allMusic[index]);
		SetMusicVolume(allMusic[index], std::pow(getSettings().musicVolume * getSettings().masterVolume, 1.0));

		currentMusicPlaying = index;
	}

	void update()
	{
		if (!isMusicPlaying())
		{
			currentMusicPlaying = 0;
			return;
		}

		SetMusicVolume(allMusic[currentMusicPlaying], std::pow(getSettings().musicVolume * getSettings().masterVolume, 1.0));

		UpdateMusicStream(allMusic[currentMusicPlaying]);
		// Limit Sound Played

		if (soundsPlayed.size() > 12)
		{
			soundsPlayed.pop_back();
		}

		for ( size_t i = 0; i < soundsPlayed.size(); i++)
		{
			auto s = soundsPlayed[i];
			if (IsSoundPlaying(s))
			{
				//std::cout << IsSoundPlaying(s);
			}
			else
			{
				soundsPlayed.erase(soundsPlayed.begin() + static_cast<int>(i));
			}

		}
	}

	void stopAllMusic()
	{
		StopMusicStream(allMusic[currentMusicPlaying]);
		currentMusicPlaying = 0;
	}

	void playSound(int sound, float volume)
	{
		if (sound <= noSound || sound >= SOUNDS_COUNT) { return; }

		volume = Clamp(volume, 0, 1);

		// Adjusts Volume based on distance from player and adjusts
		volume *= getSettings().masterVolume * getSettings().masterVolume;
		volume *= getSettings().soundsVolume * getSettings().soundsVolume;

		SetSoundVolume(allSounds[sound], volume);

		/* Create a getAlias to return all sounds that can be played multiple times
		if (IsSoundPlaying(allSounds[sound]))
		{
			return;
		}
		else
		{
			PlaySound(allSounds[sound]);
			soundsPlayed.push_back(allSounds[sound]);
		}
		*/

		PlaySound(allSounds[sound]);
		soundsPlayed.push_back(allSounds[sound]);
	}

	bool isMusicPlaying()
	{
		if (!currentMusicPlaying) { return false; }
		return IsAudioStreamPlaying(allMusic[currentMusicPlaying].stream);
	}

}
