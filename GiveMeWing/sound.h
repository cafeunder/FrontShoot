#ifndef SOUND_DEF
#define SOUND_DEF

class Sound
{
public :
	void PlaySoundHandle(int sound_index);
	void ChangeVolumeSound(int vol, int index);
	void StopSoundHandle(int sound_index);
};

extern Sound sound;

#endif