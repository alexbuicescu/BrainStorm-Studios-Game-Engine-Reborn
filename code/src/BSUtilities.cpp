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



//bool BSUtilities::loadMedia()
//{
//	iData[0] = 0;
//	iData[1] = 1;
//	iData[2] = 2;
//	iData[3] = 3;
//
//	SDL_PollEvent( &event );
//
//	obstacle->setTheVaoData(originalBody->vao, originalBody->vbo, originalBody->ibo, originalBody->data, 1.0f, 1.0f);
//
//	gOpenGLTexture->loadTheLoadingImage();
//
//
//	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	gOpenGLTexture->renderTheLoadingImage(1);
//	SDL_GL_SwapBuffers();
//
//	BS_Renderer::loadATexture("blank.png", blankTexture);
//
//
//	std::cout << "Began loading some textures..." << '\n' << '\n';
//	gOpenGLTexture->loadTextures();
//	std::cout << '\n' << "Finished loading the textures!" << '\n' << '\n';
//
//	std::cout << "Began loading backgrounds..." << '\n' << '\n';
//	obstacle->addBackgrounds();
//	std::cout << '\n' << "Finished loading backgrounds..." << '\n' << '\n';
//
//	std::cout << "Began loading the bodies..." << '\n' << '\n';
//	obstacle->setUpBodies();
//	std::cout << '\n' << "Finished loading the bodies!" << '\n' << '\n';
//
//	std::cout << "Began loading the snow flakes..." << '\n' << '\n';
//	obstacle->addSnowFlakeS();///functia incarcaHarta() trebuie sa fie interogata inainte!!! pentru CameraExtremeLeft,up,right,down
//	std::cout << '\n' << "Finished loading the snow flakes!" << '\n' << '\n';
//
//	///std::cout<<"Began loading the mouse circles..."<<'\n'<<'\n';
//	///obstacle->createMouseCircles();
//	///std::cout<<'\n'<<"Finished loading the mouse circles!"<<'\n'<<'\n';
//
//	std::cout << "Began loading the cloudy particles..." << '\n' << '\n';
//	obstacle->addCloudyParticles();
//	std::cout << '\n' << "Finished loading cloudy particles!" << '\n' << '\n';
//
//	std::cout << "Began loading the rain drops..." << '\n' << '\n';
//	gOpenGLTexture->buildRainDrops();
//	std::cout << '\n' << "Finished loading rain drops!" << '\n' << '\n';
//
//	std::cout << "Began loading the stats of coins and deaths..." << '\n' << '\n';
//	loadCoinsAndDeaths();
//	std::cout << '\n' << "Finished loading the stats of coins and deaths!" << '\n' << '\n';
//
//	///gOpenGLTexture->initVboAndVao();
//
//	std::cout << "Began loading the chapters..." << '\n' << '\n';
//	theLevels->loadChapters();
//	std::cout << '\n' << "Finished loading the chapters!" << '\n' << '\n';
//
//	std::cout << "Began loading the menu buttons..." << '\n' << '\n';
//	gOpenGLTexture->initMenuButtons();
//	std::cout << '\n' << "Finished loading the menu buttons!" << '\n' << '\n';
//
//	///theLevels->loadLevels();
//	std::cout << "Began loading the menu levels..." << '\n' << '\n';
//	gOpenGLTexture->initMenuLevels();
//	std::cout << '\n' << "Finished loading the menu levels!" << '\n' << '\n';
//
//	std::cout << "Began loading the font..." << '\n' << '\n';
//	gOpenGLTexture->initTheFont();
//	std::cout << '\n' << "Finished loading the font!" << '\n' << '\n';
//
//	std::cout << "done utilities" << '\n';
//	return true;
//}

void BSUtilities::loadCoinsAndDeaths()
{
	std::ifstream coinsAndDeaths("coinsAndDeaths");
	coinsAndDeaths >> coinsTotalNo >> deathsTotalNo;
}

void BSUtilities::update()
{
	if(!isTeleporting && gravityIsChanging == 0)
	{
		world->Step((float) 1 / theTimeHowFast , 8, 3); //1/60, 8, 3
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
