#include "BSUtilities.h"
#include "My_Values.h"

bool aIncarcatHarta = false;
BSObstacles *obstacle = new BSObstacles();
BSLevelMenu *theLevels = new BSLevelMenu;

//Textured polygon shader
//BSTexturedPolygonProgram2D gTexturedPolygonProgram2D;

//Loaded texture
BSTexture *gOpenGLTexture = new BSTexture;
BSColorRGBA gTextureColor = { 1.f, 1.f, 1.f, 1.f };




bool BSUtilities::initGL()
{
	//Initialize GLEW
	GLenum glewError = glewInit();

	if( glewError != GLEW_OK )
	{
		printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
		return false;
	}

	//Make sure OpenGL 2.1 is supported
	if( !GLEW_VERSION_2_1 )
	{
		printf( "OpenGL 2.1 not supported!\n" );
		return false;
	}

	//Initialize clear color
	glClearColor( 1.f, 1.f, 1.f, 1.f );

	//Enable texturing
	glEnable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );

    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//Check for error
	GLenum error = glGetError();

	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		return false;
	}

	return true;
}

bool BSUtilities::loadGP()
{
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

bool BSUtilities::loadMedia()
{
	iData[0] = 0;
	iData[1] = 1;
	iData[2] = 2;
	iData[3] = 3;

        SDL_PollEvent( &event );

	obstacle->setTheVaoData(originalBody->vao, originalBody->vbo, originalBody->ibo, originalBody->data, 1.0f, 1.0f);

	gOpenGLTexture->loadTheLoadingImage();


	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gOpenGLTexture->renderTheLoadingImage(1);
	SDL_GL_SwapBuffers();

	loadATexture("blank.png", blankTexture);


	std::cout<<"Began loading some textures..."<<'\n'<<'\n';
	gOpenGLTexture->loadTextures();
	std::cout<<'\n'<<"Finished loading the textures!"<<'\n'<<'\n';

	std::cout<<"Began loading backgrounds..."<<'\n'<<'\n';
	obstacle->addBackgrounds();
	std::cout<<'\n'<<"Finished loading backgrounds..."<<'\n'<<'\n';

	std::cout<<"Began loading the bodies..."<<'\n'<<'\n';
	obstacle->setUpBodies();
	std::cout<<'\n'<<"Finished loading the bodies!"<<'\n'<<'\n';

	std::cout<<"Began loading the snow flakes..."<<'\n'<<'\n';
	obstacle->addSnowFlakeS();///functia incarcaHarta() trebuie sa fie interogata inainte!!! pentru CameraExtremeLeft,up,right,down
	std::cout<<'\n'<<"Finished loading the snow flakes!"<<'\n'<<'\n';

	///std::cout<<"Began loading the mouse circles..."<<'\n'<<'\n';
	///obstacle->createMouseCircles();
	///std::cout<<'\n'<<"Finished loading the mouse circles!"<<'\n'<<'\n';

    std::cout<<"Began loading the cloudy particles..."<<'\n'<<'\n';
	obstacle->addCloudyParticles();
	std::cout<<'\n'<<"Finished loading cloudy particles!"<<'\n'<<'\n';

    std::cout<<"Began loading the rain drops..."<<'\n'<<'\n';
	gOpenGLTexture->buildRainDrops();
	std::cout<<'\n'<<"Finished loading rain drops!"<<'\n'<<'\n';

    std::cout<<"Began loading the stats of coins and deaths..."<<'\n'<<'\n';
	loadCoinsAndDeaths();
	std::cout<<'\n'<<"Finished loading the stats of coins and deaths!"<<'\n'<<'\n';

    ///gOpenGLTexture->initVboAndVao();

	std::cout<<"Began loading the chapters..."<<'\n'<<'\n';
	theLevels->loadChapters();
	std::cout<<'\n'<<"Finished loading the chapters!"<<'\n'<<'\n';

	std::cout<<"Began loading the menu buttons..."<<'\n'<<'\n';
	gOpenGLTexture->initMenuButtons();
    std::cout<<'\n'<<"Finished loading the menu buttons!"<<'\n'<<'\n';

	///theLevels->loadLevels();
	std::cout<<"Began loading the menu levels..."<<'\n'<<'\n';
	gOpenGLTexture->initMenuLevels();
    std::cout<<'\n'<<"Finished loading the menu levels!"<<'\n'<<'\n';

	std::cout<<"Began loading the font..."<<'\n'<<'\n';
	gOpenGLTexture->initTheFont();
    std::cout<<'\n'<<"Finished loading the font!"<<'\n'<<'\n';

	std::cout<<"done utilities"<<'\n';
	return true;
}

void BSUtilities::loadCoinsAndDeaths()
{
	std::ifstream coinsAndDeaths("coinsAndDeaths");
	coinsAndDeaths>>coinsTotalNo>>deathsTotalNo;
}

void BSUtilities::update()
{
	if(!isTeleporting && gravityIsChanging == 0)
	{
		world->Step((float) 1/theTimeHowFast , 8, 3); //1/60, 8, 3
		//world.step(1/30,10,10); //performs a time step in the box2d simulation
		world->ClearForces();
		//world->DrawDebugData();
	}
}

void canPressKeyAgainFunc(unsigned char key)
{
	if(canPressKeyAgain[key] == false)
	{
		canPressKeyAgain[key] = true;
	}
}
