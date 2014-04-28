#pragma once
#ifndef BS_ENGINE_H
#define BS_ENGINE_H


class BS_Engine
{
private:
	static bool initialize_Glew();
	static bool initialize_OpenGL();
	static void load_my_songs();
public:
	BS_Engine();
	static bool initialize_graphics();
	static bool initialize_audio();
	~BS_Engine();
protected:
private:
};

#endif // BS_ENGINE_H
