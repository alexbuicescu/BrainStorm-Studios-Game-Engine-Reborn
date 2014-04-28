#include "BS_Engine.h"
#include "BSOpenGL.h"
#include "My_Values.h"

BS_Audio *_my_sound_device;

BS_Engine::BS_Engine()
{
	//ctor
}

BS_Engine::~BS_Engine()
{
	//dtor
}

bool BS_Engine::initialize_Glew()
{
	//Initialize GLEW
	GLenum glewError = glewInit();

	if(glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		return false;
	}

	//Make sure OpenGL 2.1 is supported
	if(!GLEW_VERSION_2_1)
	{
		printf("OpenGL 2.1 not supported!\n");
		return false;
	}

	return true;
}

bool BS_Engine::initialize_OpenGL()
{
	//Initialize clear color
	glClearColor(1.f, 1.f, 1.f, 1.f);

	//Enable texturing
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Check for error
	GLenum error = glGetError();

	if(error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		return false;
	}

	return true;
}

bool BS_Engine::initialize_graphics()
{
	if(initialize_Glew() == false)
	{
		return false;
	}

	if(initialize_OpenGL() == false)
	{
		return false;
	}

	return true;
}

void BS_Engine::load_my_songs()
{

}

bool BS_Engine::initialize_audio()
{
	_my_sound_device = new BS_Audio();///the constructor also initializes OpenAL

	if(!_my_sound_device->get_OpenAL_status())
	{
		printf("Unable to initialize audio library!\n");
		return false;
	}

	load_my_songs();

	return true;
}
