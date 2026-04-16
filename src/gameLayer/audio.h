#pragma once
#ifndef AUDIO_H
#define AUDIO_H

namespace Audio
{
	
	void init();
	
	void loadAllMusicAndSounds();

	void update();

	void playSound(int sound, float volume = 1);

	void playMusic(int index);

	bool isSoundPlaying(int sound);

	bool isMusicPlaying();

	void stopAllMusic();

	enum Sounds
	{
		noSound = 0,
		placeBlock,
		breakBlock,
		
		FinalHit1,
		FinalHit2,
		FinalHit3,

		HImpact1,
		HImpact2,
		HImpact3,

		Hit1,
		Hit2,
		Hit3,

		Slider,

		Stone1,
		Stone2,
		Stone3,
		Stone4,
		Stone5,
		Stone6,
		Stone7,

		Wood1,
		Wood2,
		Wood3,
		Wood4,
		Wood5,
		Wood6,
		Wood7,

		SOUNDS_COUNT
	};

	enum Musics
	{
		noMusic = 0,
		musicForest,
		musicDesert,
		musicSnow,
		musicCave,

		MUSIC_COUNT
	};

}

#endif
