#pragma once
#ifndef BS_ENGINE_H
#define BS_ENGINE_H

#include "BSOpenGL.h"
#include <unordered_map>

class BS_Engine
{
private:
	static bool initialize_Glew();
	static bool initialize_OpenGL();
	static void load_my_songs();

	static void set_matrices_for_first_use();

    static bool quit_engine;

    static std::unordered_map<std::string, GLuint> textures_map;
    static std::string texture_placeholder;

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

	static void load_new_texture(std::string _texture_name);
	static GLuint get_texture(std::string _texture_name);

	static void clear_engine();

	~BS_Engine();

protected:
private:
};

#endif // BS_ENGINE_H
