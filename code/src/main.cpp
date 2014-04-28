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
#include "BS_Engine.h"
#include "BS_Game_Demo.h"
#include "BS_Object.h"

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
BSInputClass *objectInputMain = new BSInputClass();
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
	   transparentMenuBlockVao, transparentMenuBlockVbo, transparentMenuBlockIbo, gVertexBuffer = 0, texVertexBuffer = 0,
																							texture, textureMill, playerTexture, playerTextureDead, playerTextureRound,
																							starsTexture1, starsTexture2, starsTexture3, starsTexture4, backgroundTexture, clouds[6];
///GLuint iData[4];

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
///SDL_Event BS_Engine::_sdl_event;
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

int main( int argc, char* args[] )
{
	std::ifstream fin("date.txt");
	fin >> showBackgr >> isFullScreen;

	scaleSize = 11.34;
	CameraPosition->xCord = 0;
	CameraPosition->yCord = 0;
	CameraPosition->zCord = -scaleSize;

	if(BS_Engine::initialize_everything() == false)
	{
		return 1;
	}

	if(BS_Game_Demo::initialize_media() == false)
	{
		printf("Unable to load media!\n");
		///return false;
	}
	objectInputMain->initGamePad();

	srand(time(0));

	world->SetAllowSleeping(false);

	BS_Game_Demo::run_main_loop();

	BS_Engine::clear_engine();

	return 0;
}
