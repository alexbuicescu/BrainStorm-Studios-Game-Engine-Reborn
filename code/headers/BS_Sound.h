#pragma once
#ifndef BS_SOUND_H
#define BS_SOUND_H

#include <iostream>
#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <map>

class BS_Sound
{
public:
    BS_Sound();
    ~BS_Sound();
	bool isPlaying(std::string _sound);
	bool get_OpenAL_status();
	void play_sound(std::string _sound);
	void stop_playing_sound(std::string _sound);

protected:
private:
	bool createOpenAlContext();
	bool openTheWavfile(std::string _sound_path, ALuint &_sound_source, ALuint &_sound_buffer);
	std::map<std::string, ALuint> sounds;
	ALfloat Pitch = 1.5f;
    ALCdevice *deviceOpenAL;
    ALCcontext *contextOpenAL;
    bool is_initialized = false;
};

class BS_Available_Sounds
{
public:
    virtual ~BS_Available_Sounds() = 0;

    static std::string jump()
    {
        return "jump.wav";
    }
    static std::string coin()
    {
        return "coin.wav";
    }
    static std::string button()
    {
        return "button.wav";
    }
    static std::string power_up()
    {
        return "power_up.wav";
    }
    static std::string player_dieing()
    {
        return "player_dieing.wav";
    }
    static std::string player_teleport()
    {
        return "player_teleport.wav";
    }
    static std::string melody_beginning_game()
    {
        return "melody_beginning_game.wav";
    }
    static std::string melody_1()
    {
        return "melody_1.wav";
    }
    static std::string melody_2()
    {
        return "melody_2.wav";
    }
    static std::string melody_3()
    {
        return "melody_3.wav";
    }
};

#endif // BS_SOUND_H
