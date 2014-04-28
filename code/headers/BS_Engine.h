#pragma once
#ifndef BS_ENGINE_H
#define BS_ENGINE_H


class BS_Engine
{
private:
	static bool initialize_Glew();
	static bool initialize_OpenGL();
    static bool initialize_window();
	static void load_my_songs();
    static void loadCoinsAndDeaths();

public:
	BS_Engine();
	static bool initialize_graphics();
	static bool initialize_audio();
	static bool initialize_shader_program();
	static bool initialize_media();

	static bool initialize_everything();
	~BS_Engine();

protected:
private:
};

#endif // BS_ENGINE_H
