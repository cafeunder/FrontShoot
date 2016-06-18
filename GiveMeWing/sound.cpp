#include <DxLib.h>
#include "database.h"
#include "sound.h"

Sound sound;

void Sound::PlaySoundHandle(int sound_index)
{
	PlaySoundMem(sound_db.QuerySoundData(sound_index)->handle,sound_db.QuerySoundData(sound_index)->playtype);
}

void Sound::ChangeVolumeSound(int vol, int index)
{
	ChangeVolumeSoundMem(vol, sound_db.QuerySoundData(index)->handle);
}

void Sound::StopSoundHandle(int sound_index)
{
	ChangeVolumeSoundMem(255, sound_db.QuerySoundData(sound_index)->handle);
	StopSoundMem(sound_db.QuerySoundData(sound_index)->handle);
}