#pragma once
#ifndef BS_ENGINE_H
#define BS_ENGINE_H

#include "BSOpenGL.h"

class BS_Engine
{
private:
	static bool initialize_Glew();
	static bool initialize_OpenGL();
	static void load_my_songs();

    static bool quit_engine;

public:
    static SDL_Event _sdl_event;
    static const GLuint indexes_order[4];
	BS_Engine();
	static bool initialize_audio();

    static bool initialize_window();

	///initializes Glew and OpenGL
	static bool initialize_graphics();

	///initializes my shaders
	static bool initialize_shader_program();

    ///initializes audio, window, graphics, shader_program and media
	static bool initialize_everything();

	static bool get_quit_game_status();
	static void set_quit_game_status(bool _status);

	static void clear_engine();

	~BS_Engine();

protected:
private:
};

#endif // BS_ENGINE_H
