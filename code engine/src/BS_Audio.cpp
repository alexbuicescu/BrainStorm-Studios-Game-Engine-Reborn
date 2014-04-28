#include "BS_Audio.h"
#include <stdio.h>

typedef unsigned int DWORD;
typedef unsigned char BYTE;

BS_Audio::BS_Audio()
{
    is_initialized = false;
	createOpenAlContext();
}

BS_Audio::~BS_Audio()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(contextOpenAL);
	alcCloseDevice(deviceOpenAL);
}

void BS_Audio::play_sound(std::string _sound)
{
    if(get_OpenAL_status() && get_sound_source(_sound))
	{
	    alSourcePlay(get_sound_source(_sound));
	}
}

void BS_Audio::stop_playing_sound(std::string _sound)
{
    if(get_OpenAL_status() && get_sound_source(_sound) != 0)
	{
	    alSourceStop(get_sound_source(_sound));
	}
}

bool BS_Audio::isPlaying(std::string _sound)
{
    if(get_OpenAL_status() && get_sound_source(_sound))
	{
		ALenum state;
		alGetSourcei(get_sound_source(_sound), AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}
	else
	{
		return false;
	}
}

ALuint BS_Audio::get_sound_source(std::string _sound)
{
    if(get_OpenAL_status())
    {
        return sounds[_sound];
    }
    return 0;
}

bool BS_Audio::createOpenAlContext()
{
	//Now OpenAL needs to be initialized
	deviceOpenAL = alcOpenDevice(NULL);
	if(!deviceOpenAL)
	{
		std::cout<<"no sound device"<<'\n';
		return 0;
	}

	//Error during device opening
	contextOpenAL = alcCreateContext(deviceOpenAL, NULL);
	alcMakeContextCurrent(contextOpenAL);
	if(!contextOpenAL)
	{
		std::cout<<"no sound context"<<'\n';
		return 0;
	}

	is_initialized = true;
	return true;
}

bool BS_Audio::get_OpenAL_status()
{
	return is_initialized;
}

bool BS_Audio::load_sound_wav(std::string _sound_path, ALuint &_sound_source, ALuint &_sound_buffer)
{
    if(get_OpenAL_status() && get_sound_source(_sound_path))
	{
	    //Loading of the WAVE file
        //Create FILE pointer for the WAVE file
        FILE *my_file = NULL;
        //Open the WAVE file
        my_file = fopen(_sound_path.c_str(), "rb");
        if(!my_file)
        {
            std::cout << "Failed to open file" << '\n';
            return 0;
        }

        //Variables to store info about the WAVE file (all of them is not needed for OpenAL)
        char type[4];
        DWORD size, chunkSize;
        short formatType, channels;
        DWORD sampleRate, avgBytesPerSec;
        short bytesPerSample, bitsPerSample;
        DWORD dataSize;

        //Check that the WAVE file is OK
        //Reads the first bytes in the file
        fread(type, sizeof(char), 4, my_file);
        //Should be "RIFF"
        if(type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')
        {
            std::cout << "No RIFF" << '\n';
            return 0;
        }

        //Continue to read the file
        fread(&size, sizeof(DWORD), 1, my_file);
        //Continue to read the file
        fread(type, sizeof(char), 4, my_file);
        //This part should be "WAVE"
        if(type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')
        {
            std::cout << "not WAVE" << '\n';
            return 0;
        }

        //Continue to read the file
        fread(type, sizeof(char), 4, my_file);
        //This part should be "fmt "
        if(type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')
        {
            std::cout << "not fmt " << '\n';
            return 0;
        }

        //Now we know that the file is a acceptable WAVE file
        //Info about the WAVE data is now read and stored
        fread(&chunkSize, sizeof(DWORD), 1, my_file);
        fread(&formatType, sizeof(short), 1, my_file);
        fread(&channels, sizeof(short), 1, my_file);
        fread(&sampleRate, sizeof(DWORD), 1, my_file);
        fread(&avgBytesPerSec, sizeof(DWORD), 1, my_file);
        fread(&bytesPerSample, sizeof(short), 1, my_file);
        fread(&bitsPerSample, sizeof(short), 1, my_file);

        fread(type, sizeof(char), 4, my_file);
        //This part should be "data"
        if(type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')
        {
            std::cout << "Missing DATA" << '\n';
            return 0;
        }

        //The size of the sound data is read
        fread(&dataSize, sizeof(DWORD), 1, my_file);

        //Allocate memory for the sound data
        unsigned char* buf = new unsigned char[dataSize];
        //Read the sound data and display the
        std::cout << fread(buf, sizeof(BYTE), dataSize, my_file) << " bytes loaded\n";
        //number of bytes loaded.
        //Should be the same as the Data Size if OK


        ///The Sample Rate of the WAVE file
        ALuint frequency = sampleRate;
        ///The audio format (bits per sample, number of channels)
        ALenum format = 0;



        ///Generate one OpenAL Buffer and link to "buffer"
        alGenBuffers(1, &_sound_buffer);
        ///Generate one OpenAL Source and link to "source"
        alGenSources(1, &_sound_source);


        if(alGetError() != AL_NO_ERROR)
        {
            std::cout << "error GenSource" << '\n';
        }

        ///Figure out the format of the WAVE file
        if(bitsPerSample == 8)
        {
            if(channels == 1)
                format = AL_FORMAT_MONO8;
            else
                if(channels == 2)
                    format = AL_FORMAT_STEREO8;
        }
        else
            if(bitsPerSample == 16)
            {
                if(channels == 1)
                    format = AL_FORMAT_MONO16;
                else
                    if(channels == 2)
                        format = AL_FORMAT_STEREO16;
            }

        if(!format)
        {
            std::cout << "Wrong BitPerSample" << '\n';
        }

        ///Store the sound data in the OpenAL Buffer
        alBufferData(_sound_buffer, format, buf, dataSize, frequency);

        if(alGetError() != AL_NO_ERROR)
        {
            std::cout << "Error loading ALBuffer" << '\n';
        }

        ///Sound setting variables
        //Position of the source sound
        ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
        //Velocity of the source sound
        ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
        //Position of the listener
        ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
        //Velocity of the listener
        ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
        //Orientation of the listener, First direction vector, then vector pointing up)
        ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

        ///Listener
        //Set position of the listener
        alListenerfv(AL_POSITION,    ListenerPos);
        //Set velocity of the listener
        alListenerfv(AL_VELOCITY,    ListenerVel);
        //Set orientation of the listener
        alListenerfv(AL_ORIENTATION, ListenerOri);

        ///Source
        //Link the buffer to the source
        alSourcei(_sound_source, AL_BUFFER,   _sound_buffer);
        //Set the pitch of the source
        alSourcef(_sound_source, AL_PITCH,    1.0f);
        //Set the gain of the source
        alSourcef(_sound_source, AL_GAIN,     1.0f);
        //Set the position of the source
        alSourcefv(_sound_source, AL_POSITION, SourcePos);
        //Set the velocity of the source
        alSourcefv(_sound_source, AL_VELOCITY, SourceVel);
        //Set if source is looping sound
        alSourcei(_sound_source, AL_LOOPING,  AL_FALSE);


        //Play the sound buffer linked to the source
        if(alGetError() != AL_NO_ERROR)
        {
            std::cout << "Error playing sound" << '\n';
        }
        return true;
	}
	return false;
}
