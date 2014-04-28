#include "BS_Engine.h"
#include "BSOpenGL.h"
#include "My_Values.h"
#include "BSValues.h"
#include "BSTexture.h"

BS_Audio *_my_sound_device;
BSTexturedPolygonProgram2D *_my_shader_program;
BSTexture *gOpenGLTexture2 = new BSTexture;
BSObstacles *obstacle2 = new BSObstacles();
BSLevelMenu *theLevels2 = new BSLevelMenu;


BS_Engine::BS_Engine()
{
	//ctor
}

BS_Engine::~BS_Engine()
{
	//dtor
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
	if( !_my_shader_program->loadProgram("Shaders/BSTexturedPolygonProgram2D.glvs", "Shaders/BSTexturedPolygonProgram2D.glfs") )
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

void BS_Engine::loadCoinsAndDeaths()
{
	std::ifstream coinsAndDeaths("coinsAndDeaths");
	coinsAndDeaths >> coinsTotalNo >> deathsTotalNo;
}


bool BS_Engine::initialize_media()
{
	iData[0] = 0;
	iData[1] = 1;
	iData[2] = 2;
	iData[3] = 3;

	SDL_PollEvent( &event );

	BS_Renderer::set_vao_data(originalBody->vao, originalBody->vbo, originalBody->ibo, originalBody->data, 1.0f, 1.0f);
	///obstacle->setTheVaoData(originalBody->vao, originalBody->vbo, originalBody->ibo, originalBody->data, 1.0f, 1.0f);

	gOpenGLTexture2->loadTheLoadingImage();


	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gOpenGLTexture2->renderTheLoadingImage(1);
	SDL_GL_SwapBuffers();

	BS_Renderer::loadATexture("blank.png", blankTexture);


	std::cout << "Began loading some textures..." << '\n' << '\n';
	gOpenGLTexture2->loadTextures();
	std::cout << '\n' << "Finished loading the textures!" << '\n' << '\n';

	std::cout << "Began loading backgrounds..." << '\n' << '\n';
	obstacle2->addBackgrounds();
	std::cout << '\n' << "Finished loading backgrounds..." << '\n' << '\n';

	std::cout << "Began loading the bodies..." << '\n' << '\n';
	obstacle2->setUpBodies();
	std::cout << '\n' << "Finished loading the bodies!" << '\n' << '\n';

	std::cout << "Began loading the snow flakes..." << '\n' << '\n';
	obstacle2->addSnowFlakeS();///functia incarcaHarta() trebuie sa fie interogata inainte!!! pentru CameraExtremeLeft,up,right,down
	std::cout << '\n' << "Finished loading the snow flakes!" << '\n' << '\n';

	///std::cout<<"Began loading the mouse circles..."<<'\n'<<'\n';
	///obstacle->createMouseCircles();
	///std::cout<<'\n'<<"Finished loading the mouse circles!"<<'\n'<<'\n';

	std::cout << "Began loading the cloudy particles..." << '\n' << '\n';
	obstacle2->addCloudyParticles();
	std::cout << '\n' << "Finished loading cloudy particles!" << '\n' << '\n';

	std::cout << "Began loading the rain drops..." << '\n' << '\n';
	gOpenGLTexture2->buildRainDrops();
	std::cout << '\n' << "Finished loading rain drops!" << '\n' << '\n';

	std::cout << "Began loading the stats of coins and deaths..." << '\n' << '\n';
	loadCoinsAndDeaths();
	std::cout << '\n' << "Finished loading the stats of coins and deaths!" << '\n' << '\n';

	///gOpenGLTexture->initVboAndVao();

	std::cout << "Began loading the chapters..." << '\n' << '\n';
	theLevels2->loadChapters();
	std::cout << '\n' << "Finished loading the chapters!" << '\n' << '\n';

	std::cout << "Began loading the menu buttons..." << '\n' << '\n';
	gOpenGLTexture2->initMenuButtons();
	std::cout << '\n' << "Finished loading the menu buttons!" << '\n' << '\n';

	///theLevels->loadLevels();
	std::cout << "Began loading the menu levels..." << '\n' << '\n';
	gOpenGLTexture2->initMenuLevels();
	std::cout << '\n' << "Finished loading the menu levels!" << '\n' << '\n';

	std::cout << "Began loading the font..." << '\n' << '\n';
	gOpenGLTexture2->initTheFont();
	std::cout << '\n' << "Finished loading the font!" << '\n' << '\n';

	std::cout << "done utilities" << '\n';
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

	if(initialize_media() == false)
	{
		printf("Unable to load media!\n");
		///return false;
	}

	return true;
}
