#include "BS_Game_Demo.h"
#include "BS_Engine.h"
#include "BSTexture.h"
#include "BSOpenGL.h"
#include "BS_Object.h"

long BS_Game_Demo::current_time;
int BS_Game_Demo::FPS = 60;
BSTexture *gOpenGLTexture2 = new BSTexture;
BSObstacles *obstacle2 = new BSObstacles();
BSLevelMenu *theLevels2 = new BSLevelMenu;


BS_Object *my_first_object;

void BS_Game_Demo::update_input()
{
    SDL_PollEvent(&BS_Engine::_sdl_event);

    if(BS_Engine::_sdl_event.type == SDL_QUIT)
    {
        BS_Engine::set_quit_game_status(true);
    }
}

void BS_Game_Demo::update_physics()
{

}

void BS_Game_Demo::update_game()
{
    update_input();
    update_physics();
    _my_camera->update_camera();
}

void BS_Game_Demo::render_game()
{
    my_first_object->draw();
}

void BS_Game_Demo::run_main_loop()
{
    while(BS_Engine::get_quit_game_status() == false)
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			current_time = SDL_GetTicks();

			update_game();
			render_game();

			current_time = SDL_GetTicks() - current_time;

			if(current_time < (double)(1000 / FPS))
			{
				SDL_Delay((double)(1000 / FPS) - current_time);
			}
		}
		SDL_GL_SwapBuffers();
    }
}

bool BS_Game_Demo::initialize_media()
{
//	iData[0] = 0;
//	iData[1] = 1;
//	iData[2] = 2;
//	iData[3] = 3;

//	SDL_PollEvent( &BS_Engine::_sdl_event );
//
//	BS_Renderer::set_vao_data(originalBody->vao, originalBody->vbo, originalBody->ibo, originalBody->data, 1.0f, 1.0f);
//	///obstacle->setTheVaoData(originalBody->vao, originalBody->vbo, originalBody->ibo, originalBody->data, 1.0f, 1.0f);
//
//	gOpenGLTexture2->loadTheLoadingImage();
//
//
//	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	gOpenGLTexture2->renderTheLoadingImage(1);
//	SDL_GL_SwapBuffers();

	my_first_object = new BS_Object(0, 0, 1, 1, "textures.png");



//	BS_Renderer::loadATexture("blank.png", blankTexture);


//	std::cout << "Began loading some textures..." << '\n' << '\n';
//	gOpenGLTexture2->loadTextures();
//	std::cout << '\n' << "Finished loading the textures!" << '\n' << '\n';

//	std::cout << "Began loading backgrounds..." << '\n' << '\n';
//	obstacle2->addBackgrounds();
//	std::cout << '\n' << "Finished loading backgrounds..." << '\n' << '\n';

//	std::cout << "Began loading the bodies..." << '\n' << '\n';
//	obstacle2->setUpBodies();
//	std::cout << '\n' << "Finished loading the bodies!" << '\n' << '\n';

//	std::cout << "Began loading the snow flakes..." << '\n' << '\n';
//	obstacle2->addSnowFlakeS();///functia incarcaHarta() trebuie sa fie interogata inainte!!! pentru CameraExtremeLeft,up,right,down
//	std::cout << '\n' << "Finished loading the snow flakes!" << '\n' << '\n';

	///std::cout<<"Began loading the mouse circles..."<<'\n'<<'\n';
	///obstacle->createMouseCircles();
	///std::cout<<'\n'<<"Finished loading the mouse circles!"<<'\n'<<'\n';

//	std::cout << "Began loading the cloudy particles..." << '\n' << '\n';
//	obstacle2->addCloudyParticles();
//	std::cout << '\n' << "Finished loading cloudy particles!" << '\n' << '\n';

//	std::cout << "Began loading the rain drops..." << '\n' << '\n';
//	gOpenGLTexture2->buildRainDrops();
//	std::cout << '\n' << "Finished loading rain drops!" << '\n' << '\n';

//	std::cout << "Began loading the stats of coins and deaths..." << '\n' << '\n';
//	loadCoinsAndDeaths();
//	std::cout << '\n' << "Finished loading the stats of coins and deaths!" << '\n' << '\n';

	///gOpenGLTexture->initVboAndVao();

//	std::cout << "Began loading the chapters..." << '\n' << '\n';
//	theLevels2->loadChapters();
//	std::cout << '\n' << "Finished loading the chapters!" << '\n' << '\n';

//	std::cout << "Began loading the menu buttons..." << '\n' << '\n';
//	gOpenGLTexture2->initMenuButtons();
//	std::cout << '\n' << "Finished loading the menu buttons!" << '\n' << '\n';

//	///theLevels->loadLevels();
//	std::cout << "Began loading the menu levels..." << '\n' << '\n';
//	gOpenGLTexture2->initMenuLevels();
//	std::cout << '\n' << "Finished loading the menu levels!" << '\n' << '\n';

//	std::cout<<"Initializing font: ";
//	gOpenGLTexture2->initTheFont();
//
//	std::cout<<"Done loading utilities!"<<'\n'<<'\n';
	return true;
}

void BS_Game_Demo::loadCoinsAndDeaths()
{
	std::ifstream coinsAndDeaths("coinsAndDeaths");
	coinsAndDeaths >> coinsTotalNo >> deathsTotalNo;
}
