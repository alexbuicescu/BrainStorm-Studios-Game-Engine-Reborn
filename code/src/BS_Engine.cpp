#include "BS_Engine.h"
#include "BSOpenGL.h"
#include "My_Values.h"
#include "BSValues.h"
#include "BSTexture.h"

BS_Audio *_my_sound_device;
BSTexturedPolygonProgram2D *_my_shader_program;


bool BS_Engine::quit_engine;
SDL_Event BS_Engine::_sdl_event;
const GLuint BS_Engine::indexes_order[4] = {0, 1, 2, 3};
std::unordered_map<std::string, GLuint> BS_Engine::textures_map;
std::string BS_Engine::texture_placeholder = "Animations/placeholder.png";


BS_Engine::BS_Engine()
{
	//ctor
}

BS_Engine::~BS_Engine()
{
	//dtor
}

void BS_Engine::set_matrices_for_first_use()
{
    glm::vec3 m_position, m_direction = glm::vec3(0, 0, -1), m_up = glm::vec3(0, 1, 0);
	m_position = glm::vec3(CameraPosition->xCord, CameraPosition->yCord, -CameraPosition->zCord);

	LookAtMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);

	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 0);
}

bool BS_Engine::initialize_window()
{
    //This will make possible the use of std::cout
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );

	if (SDL_Init( SDL_INIT_EVERYTHING ) < 0)
	{
		std::cout<<"Couldn't initialize SDL: "<<SDL_GetError()<<'\n';
		return false;
	}

	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(0, 0);

	if(!isFullScreen)
	{
		SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL);
	}
	else
	{
		SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL | SDL_FULLSCREEN);
	}

	cross_platform_scale = (float)SCREEN_HEIGHT / 19;

	SDL_WM_SetCaption("BrainStorm Studios Game Engine Reborn", NULL);
	return true;
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

bool BS_Engine::initialize_shader_program()
{
	_my_shader_program = new BSTexturedPolygonProgram2D();

	//Load textured shader program
	if( !_my_shader_program->loadProgram("Shaders/shader_normal.glvs", "Shaders/shader_normal.glfs") )
	{
		printf( "Unable to load textured polygon program!\n" );
		return false;
	}

	//Bind textured shader program
	_my_shader_program->bind();

	//Initialize projection
	BSProjectionMatrix = glm::perspective(45.f, (float) SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 10000.0f);
	glUniformMatrix4fv( _my_shader_program->get_location_from_shader(BS_Available_Shaders::projection_matrix()), 1, GL_FALSE, glm::value_ptr( BSProjectionMatrix ) );

	//Initialize modelview
	BSModelViewMatrix = glm::mat4();
	glUniformMatrix4fv( _my_shader_program->get_location_from_shader(BS_Available_Shaders::model_view_matrix()), 1, GL_FALSE, glm::value_ptr( BSModelViewMatrix ) );

	//Set texture unit
	_my_shader_program->set_uniform(BS_Available_Shaders::texture_unit(), 0 );

	return true;

}

bool BS_Engine::initialize_everything()
{
	if(initialize_audio() == false)
	{
		printf("Unable to initialize audio library!\n");
		///return false;
	}

	if(initialize_window() == false)
	{
		printf("Unable to initialize SDL library!\n");
		return false;
	}

	if(initialize_graphics() == false)
	{
		printf("Unable to initialize graphics library!\n");
		return false;
	}

	//Load graphics programs
	if(initialize_shader_program() == false)
	{
		printf("Unable to load shader programs!\n");
		return false;
	}

	load_new_texture(texture_placeholder);
	set_matrices_for_first_use();

	quit_engine = false;

	return true;
}

void BS_Engine::load_new_texture(std::string _texture_name)
{
    if(textures_map[_texture_name] == 0)
    {
        textures_map[_texture_name] = BS_Renderer::load_texture(_texture_name);
    }
}

GLuint BS_Engine::get_texture(std::string _texture_name)
{
    if(textures_map[_texture_name] != 0)
    {
        return textures_map[_texture_name];
    }
    else
    {
        return textures_map[texture_placeholder];
    }
}

bool BS_Engine::get_quit_game_status()
{
    return quit_engine;
}

void BS_Engine::set_quit_game_status(bool _status)
{
    quit_engine = _status;
}

void BS_Engine::clear_engine()
{
	SDL_Quit();
	exit(0);
}
