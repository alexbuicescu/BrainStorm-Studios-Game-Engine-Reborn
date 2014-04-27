#include "BSValues.h"
#include "BSUtilities.h"
#include <iostream>
#include <string>
#include <SOIL/SOIL.h>
#include "BSObstacles.h"
#include "BSTexture.h"
#include "BSActionStuff.h"
#include "BSTexturedPolygonProgram2D.h"
#include <ctime>
#include "BSLevelMenu.h"

#include "BSInputClass.h"

#include "My_Values.h"

#include "BS_Screen_Menu.h"

BS_Sound *_my_sound_device;
BSTexturedPolygonProgram2D *_my_shader_program;

float worldAngle = 0;
b2Vec2 worldGravity = b2Vec2(0, -40);
int gravityIsChanging = 0, startAngle = 0;


// comenteriu hack

GLuint blankTexture, whiteTexture;

BSBody *originalParticle = new BSBody, *originalTransparentMenu = new BSBody, *originalCoin = new BSBody, *originalFan = new BSBody,
*originalBody = new BSBody, *originalDarkLevel = new BSBody, *originalDarkMenu = new BSBody, *originalRainDrop = new BSBody,
*originalWormHole = new BSBody, *originalDustBody = new BSBody, *originalFanParticleX = new BSBody, *originalFanParticleY = new BSBody;
BSWormHole *lastWorm = NULL;

std::vector<BSSnowFlakes*>listOfRainDrops;
long long timer = 0;

BSTheSquareBodies *hero = new BSTheSquareBodies,
*Hero2nd = new BSTheSquareBodies;

BSLevelMenu *theLevelMenuObject = new BSLevelMenu();
BSUtilities *objBSUtils = new BSUtilities();
BSObstacles *objObstacles = new BSObstacles();
BSActionStuff *action = new BSActionStuff();
//	BSActionStuffRound *action2 = new BSActionStuffRound();
BSTexture* randareMenu = new BSTexture();
BSInputClass *ibjectInputMain = new BSInputClass();
BSTheBackgrounds *backgroundSky = new BSTheBackgrounds,
*backgroundMountain = new BSTheBackgrounds,
*backgroundBush = new BSTheBackgrounds,
*backgroundField = new BSTheBackgrounds,
*backgroundHighField = new BSTheBackgrounds;
BSAnimation *SoundButtonTexture = new BSAnimation;
BSTextureSquare *selectedTextureSquare = new BSTextureSquare,
*mouseTexture = new BSTextureSquare;
BSTextureRound *selectedTextureRound = new BSTextureRound;
BSTheSquare *theRenderedObstacleSquare = new BSTheSquare,
*theRenderedTextureSquare = new BSTheSquare;
BSTheCircle *theRenderedObstacleCircle = new BSTheCircle,
*theRenderedTextureCircle = new BSTheCircle;
BSSceneStructure* deadScene = new BSSceneStructure;
BSTheFontLetters *letters[256];
ChapterStructure *chaptere[100];
b2World* world = new b2World(b2Vec2(0, -40));
BSTexturedVertex2D mainDarkData[4], darkLevelData[4],
				   coinData[4], fnData[4], particleData[4], transparentMenuBlock[4];
GLuint mainDarkTexture, mainDarkVao, darkLevelTexture, darkLevelVao, vboMainDark, vboDarkLevel, iboMainDark, iboDarkLevel,
	   coinTexture, coinVao, coinVbo, coinIbo, fanTexture, fanVao, fanVbo, fanIbo, soundPlayerTexture,// mVBOID = 0, mIBOID = 0,
	   particleVaoBuffer, particleVboBuffer, particleIboBuffer,
	   whenHitsGroundVaoBuffer, whenHitsGroundVboBuffer, whenHitsGroundIboBuffer,
	   transparentMenuBlockVao, transparentMenuBlockVbo, transparentMenuBlockIbo, iData[4], gVertexBuffer = 0, texVertexBuffer = 0,
																							texture, textureMill, playerTexture, playerTextureDead, playerTextureRound,
																							starsTexture1, starsTexture2, starsTexture3, starsTexture4, backgroundTexture, clouds[6];

LevelChooserPlayerTexture *levelChoserPlayer = new LevelChooserPlayerTexture;
BSTexturedPolygonProgram2D* mainTexturedPolygonProgram2D = NULL;
BSPlayerControls *heroKeyboard = new BSPlayerControls;


BSMenuStructure *currentMenu = new BSMenuStructure;
BSButtonStructure *currentButton = new BSButtonStructure;

//Vertex buffer
//GLint mVertexPos2DLocation;
//GLint mTexCoordLocation;

glm::mat4 BSProjectionMatrix;
glm::mat4 BSModelViewMatrix;



long long timeSinceMovedMouse = 0, timeSinceMovedLittleCircle = 0;

std::vector<BSTheSquareBodies*> ListOfSquareBodies;
std::vector<BSTheSquareBodies*> ListOfSquareDustBodies;
std::vector<BSSnowFlakes*> listOfSnowFlakes;
std::vector<BSWorldOfGooMouse*> listOfMouseCircles;
std::vector<BSCloudyParticles*> listOfCloudyParticles;
std::vector<BSTheRoundBodies*> ListOfRoundBodies;
std::vector<BSTheParticles*> ListOfParticles;
std::vector<BSNewList*>listOfAnimations;
int nrOfListOfAnimations = 0;
///std::vector<BSNewList*> listOfAnimations;
std::vector<BSMenuStructure*> listOfMenus;
std::vector<BSTheMessages*>listOfMessages;
std::vector<BSFans*> listOfFans;
std::vector<BSCoins*> listOfCoins;
std::vector<BSTextureSquare*> ListOfSquareTextures;
std::vector<BSTextureRound*> ListOfRoundTextures;
std::vector<BSWormHole*> listOfWormHoles;
std::vector<BsActiveAnimations*>listOfActiveAnimations;
std::vector<BSAnimList*>listOfLoadedAnimations;

float theCoinX, theCoinY;

int NrOfTotalLetters, nrOfChapters = 0, heroFacesLeftOrRight = 1, nrOfMessageSign = 0,
					  coinsTotalNo = 0, coinsThisLevel = 0, deathsTotalNo = 0, deathsThisLevel = 0, variableWall = 0,
					  nrBodiesDistroyed = 0, indexOfSelectedBody = 0, nrParticlesDistroyed = 0,
					  nrParticles = 0, nrOfWhenItHits = 0, numberOfSquaresForPath = 0, nrBodies = 0;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
int SCREEN_FPS = 60;//usually 60
int SCREEN_BPP = 32, timeWhenTeleported = 0, muscicIndex = 0;



bool DarkLevels = false, canAddCoins = false, canAddFans = false, settingsFromFirstOrGame = false,
	 canChangeVolumeMusic = false, canChangeVolumeAudioSfx = false, canChangeResolution = false,
	 wasPressed[256], corpulEsteDistrus[1000], itMovedX = false, itMovedY = false, itMovedXLeft = false,
											   itMovedXRight = false, itTurnedX = false, canMoveCamera = false, canWriteInConsole = false,
											   canClickLeft = true, canClickRight = true, aSalvatHarta = false, canChangeScaleSize = false, canTeleportHero = false,
											   keyStates[256], canPressKeyAgain[256], existsGamePad = false, useKeyboardOrGamePad = true, theLevelHasFinished = false,
																					  clean = false, isPaused = true, canPressTheKey = true, ButtonUp = false, ButtonDown = false,
																					  ButtonRight = false, ButtonLeft = false, ButtonRun = false, ButtonJump = false,
																					  MouseLeft = false, MouseRight = false, MouseMoved = false,
																					  quit = false, moveLeftGround = false, moveRightGround = false, moveLeftAir = false, moveRightAir = false, isJumping = false,
																					  canJumpVertically = false, canJumpHorizontally = false, isInCorner = false, canClick = false, aDatClick = false,
																					  isOnGround = false, isOnLeftWall = false, isOnRightWall = false, isInAir = false, jumpsUpTheWall = false,
																					  pressedJumpSecond = false, canJumpSecond = true, jumpSecondSpinned = false, isTeleporting = false, isSpaceDown = false;
float jumpSecondSpinAngle = 0;
bool useSnow = true, useRain = false, useAssassinsCreed = false;
int levelHasScript = 0;


glm::mat4 LookAtMatrix;

float HeroInitialX = 3, HeroInitialY = 3, newVolumeAudio = 1.f, newVolumeSFX = 1.f, currentVolumeAudio = 1.f,
	  currentVolumeSFX = 1.f, scaleSize = 32, theTimeHowFast = 60,
	  CameraExtremeLeft = SCREEN_WIDTH / 2 / (SCREEN_HEIGHT / 18.f),
	  CameraExtremeRight = SCREEN_WIDTH / 2 / (SCREEN_HEIGHT / 18.f) + 2000 / scaleSize,
	  CameraExtremeUp = SCREEN_HEIGHT / 2 / (SCREEN_HEIGHT / 18.f) + 230 / scaleSize,
	  CameraExtremeDown =  SCREEN_HEIGHT / 2 / (SCREEN_HEIGHT / 18.f),
	  MenuCoordinateX = SCREEN_WIDTH / 2 / scaleSize,
	  MenuCoordinateY = SCREEN_HEIGHT / 2 / scaleSize,
	  WorldBoundaryLeft = 0, WorldBoundaryDown = 0, WorldBoundaryUp = CameraExtremeUp, WorldBoundaryRight = CameraExtremeRight,
	  lastMovedX = 0, changeInPosition = 0, lastPositionOfHeroX = 0, lastPositionOfHeroY = 0,
	  startPositionCameraX = 0, sinX = 0, sinY = 0, iteratieSin = 1.f * 5,
	  widthOfObstacle = Hero_size, heightOfObstacle = Hero_size, createObstacleX = 0, createObstacleY = 0,
	  razaInitialaInner = 40, razaInitialaOuter = 40, MouseX = 0, MouseY = 0, MouseXLast = 0, MouseYLast = 0,
	  SursaDeLuminaX = 0, SursaDeLuminaY = 0, lastTime = 0,
	  speedWalk = 25, speedWalkFast = 50, speedRun = 1.2, speedJumpUp = 17.7f, speedJumpToSide = 17, speedGoDown = -20,
	  speedWalkInAir = 25, speedWalkInAirFast = 50, speedJumpToTheSameSide = 17.1, speedJumpUpOnWall = 21.f,
	  timeToStayInAirWhenJumpsOnTheSameWall = 175, timeToStayOnWall = 250,
	  buttonMenuWidth = 3 / 2.f, buttonMenuHeight = 1 / 2.f, moveStringX = 0, moveStringY = 0;



BSTextureSquare *theLoadingImage = new BSTextureSquare;
SDL_Event event;
bool doneLoadingTheGame = false, canRenderGameNow = false;

short BinaryPlayer = -1;//0x0001;  // 0000000000000001 in binary
short BinaryMonster = -2;//0x0002; // 0000000000000010 in binary
short BinaryScenery = 1;//0x0004; // 0000000000000100 in binary
short BinaryParticles = -1;//0x0008; // 0000000000000100 in binary


float gameBoundLeft, gameBoundRight, gameBoundDown, gameBoundUp;

BSBody *CameraPosition = new BSBody;

float playerAnimWidth, playerAnimHeight;


char initialSquareTexturePath[100], initialRoundTexturePath[100];
float cross_platform_scale = 0;

const char* SCREEN_TITLE = "BrainStorm Studios";
std::string consoleString = "";
std::string anotherString = "false";
std::string theString;

int showBackgr = false, isFullScreen = false;

void runMainLoop();
void renderTheMenusAndGame();
void renderTheLoadingState();
void renderTheMenuBeforeItBecomesFullyVisible();


long FPS = 0, lastFPS = 0, showFPS = 0;

int main( int argc, char* args[] )
{
	std::ifstream fin("date.txt");
	fin >> showBackgr >> isFullScreen;
	iData[0] = 0;
	iData[1] = 1;
	iData[2] = 2;
	iData[3] = 3;

	scaleSize = 11.34;
	CameraPosition->xCord = 0;
	CameraPosition->yCord = 0;
	CameraPosition->zCord = -scaleSize;
	ibjectInputMain->setTheVideoDisplayAndWindow();
	ibjectInputMain->initGamePad();


	initialSquareTexturePath[0] = NULL, initialRoundTexturePath[0] = NULL;
	strcpy(initialSquareTexturePath, "Animations/textureAnimation/texture.png");
	strcpy(initialRoundTexturePath, "Animations/roundAnimation/windmillPropeller.png");




	_my_sound_device = new BS_Sound();
	if(!_my_sound_device->get_OpenAL_status())
	{
		printf( "Unable to initialize audio library!\n" );
		//return 1;
	}

//	pthread_t threadInput;
//	pthread_create(&threadInput, NULL, &VideoAndInput, &objBSUtils);
//	pthread_exit(NULL);

	_my_shader_program = new BSTexturedPolygonProgram2D();
	if( !objBSUtils->initGL())
	{
		printf( "Unable to initialize graphics library!\n" );
		//return 1;
	}
	//Load graphics programs
	if( !objBSUtils->loadGP() )
	{
		printf( "Unable to load shader programs!\n" );
		//return 1;
	}
	//objObstacles->setUpBodies();
	if( !objBSUtils->loadMedia() )
	{
		printf( "Unable to load media!\n" );
		//return 2;
	}




	for(int i = 0; i <= 255 ; i++)
	{
		canPressKeyAgain[i] = true;
		wasPressed[i] = false;
	}

	lastTime = SDL_GetTicks();


	lastMovedX = hero->theBody->GetPosition().x;


	srand(time(0));

	///objObstacles->setTheVaoData(originalBody->vao, originalBody->vbo, originalBody->ibo, originalBody->data, 1.0f, 1.0f);
//randareMenu->loadTheLoadingImage();
//	randareMenu->buildRainDrops();
	quit = false;
	lastFPS = SDL_GetTicks();
	world->SetAllowSleeping(false);
	while(!canRenderGameNow)
	{
		renderTheMenuBeforeItBecomesFullyVisible();
	}
	long long currentTime = 0, previousTime = 0, maxFPS = 60;
	while(!quit)
	{
//		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        SDL_PollEvent( &event );
//
//        if(event.type == SDL_QUIT)
//        {
//            quit = true;
//        }
//
//		///randareMenu->renderTheLoadingImage(1);
//		randareMenu->renderRainDrops();
//		SDL_GL_SwapBuffers();

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			currentTime = SDL_GetTicks();
			runMainLoop();

			currentTime = SDL_GetTicks() - currentTime;

			if(currentTime < (double)(1000 / maxFPS))
			{
				SDL_Delay((double)(1000 / maxFPS) - currentTime);
			}
		}
		SDL_GL_SwapBuffers();
	}


	SDL_Quit();
	exit(0);


	return 0;
}


void play_music_continuosly()
{
	///Check if the music is still playing (you got 3 melodies that play through out the game, and you want to play it one after another)
	if(muscicIndex == 1 && !_my_sound_device->isPlaying(BS_Available_Sounds::melody_1()))
	{
		_my_sound_device->play_sound(BS_Available_Sounds::melody_2());
		muscicIndex = 2;
	}
	else
		if(muscicIndex == 2 && !_my_sound_device->isPlaying(BS_Available_Sounds::melody_2()))
		{
			_my_sound_device->play_sound(BS_Available_Sounds::melody_3());
			muscicIndex = 3;
		}
		else
			if(muscicIndex == 3 && !_my_sound_device->isPlaying(BS_Available_Sounds::melody_3()))
			{
				_my_sound_device->play_sound(BS_Available_Sounds::melody_1());
				muscicIndex = 1;
			}
}

BSColorRGBA black = {0.f, 0.f, 0.f, 1};
//val este pentru a da un delay la timer, gen daca este 10, sa astepte 10 ms
void runMainLoop()
{
//    uint32 curFPS = SDL_GetTicks() - lastFPS;
	long curFPS = SDL_GetTicks();
	if(curFPS % 1000 < 100 && lastFPS % 1000 > 100)
	{
		showFPS = FPS;
		FPS = 1;
		lastFPS = curFPS;
	}
	else
	{
		FPS++;
		lastFPS = curFPS;
	}
//    lastFPS = SDL_GetTicks();
//
//    if(curFPS)
//    {
//        showFPS = (float)1000/curFPS;
//    }

	///Check if the music is still playing (you got 3 melodies that play through out the game, and you want to play it one after another)
	play_music_continuosly();

	///ii dau sunet in caz ca e in power up
	if(!isPaused)
	{
		if(hero->heroIsBerserk1 || hero->heroIsBerserk2 || hero->heroIsBerserk3)
		{
			if(!_my_sound_device->isPlaying(BS_Available_Sounds::power_up()))
			{
				_my_sound_device->play_sound(BS_Available_Sounds::power_up());
			}
		}
	}
	///scot sunetul daca nu e in power up
	if((!hero->heroIsBerserk1 && !hero->heroIsBerserk2 && !hero->heroIsBerserk3) || isPaused)
	{
		_my_sound_device->stop_playing_sound(BS_Available_Sounds::power_up());
	}

	///the rendering function
	renderTheMenusAndGame();

//        randareMenu->renderMyFont("FPS " + BStoString(showFPS), originalCoin->xCord + CameraPosition->xCord - CameraExtremeLeft,
//                        originalCoin->yCord + CameraPosition->yCord - CameraExtremeDown - 1.2f, 1, 1, black);

	///Get the keyboard or gamepad input
	ibjectInputMain->handleInput();
	action->heroTouchesObjects();
	action->moveThePlayer();
}

float toRadians(float x)
{
	float pi = 3.1415;
	return (float) (pi / 2 * x) / 90;
}

int sign(float nr)
{
	if(nr > 0)
	{
		return 1;
	}
	else
		if(nr < 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
}
float modul(float nr)
{
	if(nr > 0)
	{
		return nr;
	}
	else
		return -nr;
}

void renderTheMenusAndGame()
{
	if(gravityIsChanging == 1)
	{
		if(worldAngle < startAngle + 90)
		{
			worldAngle += 2.2f;
		}
		else
		{
			worldAngle = startAngle + 90;
			gravityIsChanging = 0;
			worldGravity = b2Vec2((int)(-sin(toRadians(worldAngle)) * 40), (int)(cos(toRadians(worldAngle)) * -40));

			if(worldGravity.x != 0)
			{
				speedWalk = modul(speedWalk) * sign(worldGravity.x);
				speedWalkInAir = modul(speedWalkInAir) * sign(worldGravity.x);
				speedJumpToSide = modul(speedJumpToSide) * sign(worldGravity.x);
				speedJumpToTheSameSide = modul(speedJumpToTheSameSide) * sign(worldGravity.x);
			}
			else
			{
				speedWalk = modul(speedWalk) * -sign(worldGravity.y);
				speedWalkInAir = modul(speedWalkInAir) * -sign(worldGravity.y);
				speedJumpToSide = modul(speedJumpToSide) * -sign(worldGravity.y);
				speedJumpToTheSameSide = modul(speedJumpToTheSameSide) * -sign(worldGravity.y);
			}
			world->SetGravity(worldGravity);

			hero->theBody->SetLinearVelocity(b2Vec2(0, 0));
			world->Step((float) 1 / theTimeHowFast , 8, 3); //1/60, 8, 3
			world->ClearForces();
		}
//        hero->theBody->SetTransform(hero->theBody->GetPosition(), toRadians(-worldAngle));
		hero->xAngle = -worldAngle;
	}
	else
		if(gravityIsChanging == -1)
		{
			if(worldAngle > startAngle - 90)
			{
				worldAngle -= 2.2f;
			}
			else
			{
				worldAngle = startAngle - 90;
				gravityIsChanging = 0;
				worldGravity = b2Vec2((int)(-sin(toRadians(worldAngle)) * 40), (int)(cos(toRadians(worldAngle)) * -40));

				if(worldGravity.x != 0)
				{
					speedWalk = modul(speedWalk) * sign(worldGravity.x);
					speedWalkInAir = modul(speedWalkInAir) * sign(worldGravity.x);
					speedJumpToSide = modul(speedJumpToSide) * sign(worldGravity.x);
					speedJumpToTheSameSide = modul(speedJumpToTheSameSide) * sign(worldGravity.x);
				}
				else
				{
					speedWalk = modul(speedWalk) * -sign(worldGravity.y);
					speedWalkInAir = modul(speedWalkInAir) * -sign(worldGravity.y);
					speedJumpToSide = modul(speedJumpToSide) * -sign(worldGravity.y);
					speedJumpToTheSameSide = modul(speedJumpToTheSameSide) * -sign(worldGravity.y);
				}
				world->SetGravity(worldGravity);

				hero->theBody->SetLinearVelocity(b2Vec2(0, 0));
				world->Step((float) 1 / theTimeHowFast , 8, 3); //1/60, 8, 3
				world->ClearForces();
			}
//            hero->theBody->SetTransform(hero->theBody->GetPosition(), toRadians(-worldAngle));
			hero->xAngle = -worldAngle;
		}
//        std::cout<<worldGravity.x<<' '<<worldGravity.y<<'\n';

	///render the game if it's not in any other menu
	if(currentMenu->indexOfMenu == 3)
	{
		isPaused = false;
		randareMenu->renderVboAndVao();
		bool doesItNeedsUpdateNext = false, doesItNeedsUpdateDead = false;

		///Set the Next Level Scene
		if(levelChoserPlayer->justFinished || deadScene->CosValueColorOfTheScene)
		{
			randareMenu->renderNextLevelSceneChange();
			if(deadScene->CosValueColorOfTheScene == 0)
			{
				deadScene->colorGoesUpOrDown = 1;
			}
		}
		else
		{
			doesItNeedsUpdateNext = true;
			hero->heroWasToNextLevel = false;
		}

		///Set the deadScene
		if(hero->heroIsDead || deadScene->SinValueColorOfTheScene)
		{
			randareMenu->renderDeadSceneChange();
			if(deadScene->SinValueColorOfTheScene == 0)
			{
				deadScene->colorGoesUpOrDown = 1;
			}
		}
		else
			///if he isn't dead
		{
			hero->heroWasDead = false;
			doesItNeedsUpdateDead = true;
		}
		if(doesItNeedsUpdateDead && doesItNeedsUpdateNext)
		{
			objBSUtils->update();

			action->generateWhenHitsGroundParticles();
		}
	}
	else
		///Restul meniurilor
	{
		isPaused = true;
		///Daca se afla in meniul de levele, randeaza levelele
		if(currentMenu->indexOfMenu == 4)
		{
			randareMenu->renderVboAndVao();
			randareMenu->renderMenuLevels(chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->nrOfLevels,
										  chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter);
		}
		else
			if(currentMenu->indexOfMenu == 5)
			{
				randareMenu->renderVboAndVao();
				randareMenu->renderMenuChapters();
			}
			else
			{
				randareMenu->renderVboAndVao();
				randareMenu->renderAnyButtons(currentMenu);
			}
	}
}






float alphaColorSin = 90;
void renderTheLoadingState()
{
//	if(alphaColorSin > 0)
//	{
//		alphaColorSin -= 1.0f;
//		randareMenu->renderTheLoadingImage(sin(randareMenu->toRadians(alphaColorSin)));
//
//        SDL_PollEvent( &event );
//
//        if(event.type == SDL_QUIT)
//        {
//            quit = true;
//        }
//	}
//	else
//	{
	canRenderGameNow = true;
//	}
}

void renderTheMenuBeforeItBecomesFullyVisible()
{
	play_music_continuosly();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(currentMenu->indexOfMenu == 3)
	{
		randareMenu->renderVboAndVao();
//			bool doesItNeedsUpdateNext = false, doesItNeedsUpdateDead = false;
	}
	else
		///Restul meniurilor
	{
		///Daca se afla in meniul de levele, randeaza levelele
		if(currentMenu->indexOfMenu == 4)
		{
			randareMenu->renderVboAndVao();
			randareMenu->renderMenuLevels(chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->nrOfLevels,
										  chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter);

		}
		else
			if(currentMenu->indexOfMenu == 5)
			{
				randareMenu->renderVboAndVao();
				randareMenu->renderMenuChapters();
			}
			else
			{
				randareMenu->renderVboAndVao();
				randareMenu->renderAnyButtons(currentMenu);
			}
	}


	if(!canRenderGameNow)
	{
		renderTheLoadingState();

		world->Step((float) 1 / theTimeHowFast , 8, 3);
		world->ClearForces();
		action->heroTouchesObjects();

		hero->animationIndexCurrentAnimation = hero->animationIndexPlayerStand;
		hero->IsAtStepOfAnimation1 = true;
		hero->IsAtStepOfAnimation2 = false;
		hero->IsAtStepOfAnimation3 = false;
	}
	SDL_GL_SwapBuffers();
}
