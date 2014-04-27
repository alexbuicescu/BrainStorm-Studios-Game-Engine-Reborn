#include "BSInputClass.h"
///#include <SDL/SDL_image.h>


int nrOfGamepads = 0;
BSGamePadStructure *theGamepads[10];
BSTexture *someRender = new BSTexture();
Uint8 *keystates;

SDL_Joystick *joystick = NULL;

BSObstacles *objClassObstacle = new BSObstacles();
BSLevelMenu *objClassLevelMenu = new BSLevelMenu();
BSXmlFiles *objClassXml = new BSXmlFiles();
BSActionStuff *objClassAction = new BSActionStuff();

bool jumpPressed = false, enterPressed = false, escapePressed = false,
	backspacePressed = false, speedChangedBoostHigh = false;
bool pressedKeys[256];
bool pressedGamePadButtons[50];
bool pressedHats[5], canRefreshGame = false;

BSInputClass::BSInputClass()
{

}

void BSInputClass::setTheVideoDisplayAndWindow()
{
	//This will make possible the use of std::cout
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );

	if (SDL_Init( SDL_INIT_EVERYTHING ) < 0)
	{
		std::cout<<"Couldn't initialize SDL: "<<SDL_GetError()<<'\n';
	}

	SDL_EnableUNICODE( 1 );
	SDL_EnableKeyRepeat(0, 0);
	if(!isFullScreen)
	{
	    SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL);// | SDL_FULLSCREEN);
	}
	else
    {
	    SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL | SDL_FULLSCREEN);
    }

    cross_platform_scale = (float)SCREEN_HEIGHT / 19;

	SDL_WM_SetCaption( "BrainStorm Studios Game Engine", NULL );

//	SDL_ShowCursor(SDL_DISABLE);
}

void BSInputClass::initGamePad()
{
	nrOfGamepads = SDL_NumJoysticks();

	if(nrOfGamepads)
	{
		existsGamePad = true;
	}

	SDL_JoystickEventState(SDL_ENABLE);


//	char theNodes[15][100], theAttributes[14][100];
//	ListaDeCaractere *levelDetails[1000], *levelDetailsAttributes[1];
	std::vector<std::string>theNodes, theAttributes;
	std::vector<ListaDeCaractere*> levelDetails;
	std::vector<std::string> levelDetailsAttributes;

	theNodes.push_back("gamepad_or_keyboard"); theNodes.push_back("gamepad_index");
	theNodes.push_back("left_player"); theNodes.push_back("right_player"); theNodes.push_back("up_player"); theNodes.push_back("down_player");
	theNodes.push_back("left_mouse"); theNodes.push_back("right_mouse"); theNodes.push_back("up_mouse"); theNodes.push_back("down_mouse");
	theNodes.push_back("jump"); theNodes.push_back("run"); theNodes.push_back("teleport"); theNodes.push_back("pause"); theNodes.push_back("enter");

	theAttributes.push_back("nrOfGamepads");
	theAttributes.push_back("type1");
	theAttributes.push_back("type2"); theAttributes.push_back("type3"); theAttributes.push_back("type4"); theAttributes.push_back("type5");
	theAttributes.push_back("type6"); theAttributes.push_back("type7"); theAttributes.push_back("type8"); theAttributes.push_back("type9");
	theAttributes.push_back("type10"); theAttributes.push_back("type11"); theAttributes.push_back("type12"); theAttributes.push_back("type13");

	objClassXml->readAnyXML("Controllers/controllers.xml", "the_controllers", "controller", theNodes, theAttributes, levelDetails, levelDetailsAttributes);

	heroKeyboard->Left = (SDLKey)atof(levelDetails[0]->theCharArray2D[2].c_str());
	heroKeyboard->Right = (SDLKey)atof(levelDetails[0]->theCharArray2D[3].c_str());
	heroKeyboard->Up = (SDLKey)atof(levelDetails[0]->theCharArray2D[4].c_str());
	heroKeyboard->Down = (SDLKey)atof(levelDetails[0]->theCharArray2D[5].c_str());
	heroKeyboard->MouseLeft = (SDLKey)atof(levelDetails[0]->theCharArray2D[6].c_str());
	heroKeyboard->MouseRight = (SDLKey)atof(levelDetails[0]->theCharArray2D[7].c_str());
	heroKeyboard->MouseUp = (SDLKey)atof(levelDetails[0]->theCharArray2D[8].c_str());
	heroKeyboard->MouseDown = (SDLKey)atof(levelDetails[0]->theCharArray2D[9].c_str());
	heroKeyboard->Jump = (SDLKey)atof(levelDetails[0]->theCharArray2D[10].c_str());
	heroKeyboard->Run = (SDLKey)atof(levelDetails[0]->theCharArray2D[11].c_str());
	heroKeyboard->Teleport = (SDLKey)atof(levelDetails[0]->theCharArray2D[12].c_str());
	heroKeyboard->Escape = (SDLKey)atof(levelDetails[0]->theCharArray2D[13].c_str());
	heroKeyboard->Enter = (SDLKey)atof(levelDetails[0]->theCharArray2D[14].c_str());

	///initialize the remembered gamepads
	for(int i = 0 ; i < atof(levelDetailsAttributes[0].c_str()); i++)
	{
//		theGamepads[i] = new BSGamePadStructure;
//
//		theGamepads[i]->indexOfGamepad = i;
//		theGamepads[i]->joystick = SDL_JoystickOpen(i);
//		theGamepads[i]->nrButtons = SDL_JoystickNumButtons(theGamepads[i]->joystick);

//		theGamepads[i]->
	}

	///initialize the rest of gamepad, if any exists
	for(int i = atof(levelDetailsAttributes[0].c_str()) ; i < nrOfGamepads ; i++)
	{
		theGamepads[i] = new BSGamePadStructure;
		theGamepads[i]->indexOfGamepad = i;
		theGamepads[i]->joystick = SDL_JoystickOpen(i);
		theGamepads[i]->nrButtons = SDL_JoystickNumButtons(theGamepads[i]->joystick);
	}
}

void BSInputClass::getButtonsState(int gamePadIndex, SDL_Joystick *theJoystick)
{
	if(existsGamePad)
	{
		joystick = SDL_JoystickOpen(gamePadIndex);

		///See on which hat is pressing
		if(event.type == SDL_JOYHATMOTION)
		{
			if ( event.jhat.value & SDL_HAT_UP )
			{
				//theGamepads[gamePadIndex]->upHat = true;
			}
			else
			{
				//theGamepads[gamePadIndex]->upHat = false;
			}

			if ( event.jhat.value & SDL_HAT_DOWN )
			{
				//theGamepads[gamePadIndex]->downHat = true;
			}
			else
			{
				//theGamepads[gamePadIndex]->downHat = false;
			}

			if ( event.jhat.value & SDL_HAT_LEFT )
			{
				//theGamepads[gamePadIndex]->leftHat = true;
			}
			else
			{
				//theGamepads[gamePadIndex]->leftHat = false;
			}

			if ( event.jhat.value & SDL_HAT_RIGHT )
			{
				//theGamepads[gamePadIndex]->rightHat = true;
			}
			else
			{
				//theGamepads[gamePadIndex]->rightHat = false;
			}
		}

		///Control the joystick
		if((SDL_JoystickGetAxis(theGamepads[0]->joystick, 0) < -3200) || (SDL_JoystickGetAxis(theGamepads[0]->joystick, 0) > 3200))
		{
			float heroPosition = hero->theBody->GetPosition().x;

			mouseTexture->xCord += CameraPosition->xCord - CameraExtremeLeft;
			mouseTexture->yCord += CameraPosition->yCord - CameraExtremeDown;

			///Daca jucatorul e in partea din stanga a ecranului
			if (heroPosition <= CameraExtremeLeft && mouseTexture->xCord >= mouseTexture->width &&
					mouseTexture->xCord <= SCREEN_WIDTH / scaleSize - mouseTexture->width)
			{
				mouseTexture->xCord += SDL_JoystickGetAxis(theGamepads[0]->joystick, 0) / 32768.f * 1/4.f;

				///Daca a depasit limita, il aduc intre boundaries
				if(mouseTexture->xCord > SCREEN_WIDTH / scaleSize - mouseTexture->width)
				{
					mouseTexture->xCord = SCREEN_WIDTH / scaleSize - mouseTexture->width;
				}
				else
					if(mouseTexture->xCord < mouseTexture->width)
					{
						mouseTexture->xCord = mouseTexture->width;
					}
			}
			else
				if(heroPosition > CameraExtremeLeft && heroPosition <= CameraExtremeRight &&
						mouseTexture->xCord >= heroPosition - CameraExtremeLeft + mouseTexture->width &&
						mouseTexture->xCord <= heroPosition + CameraExtremeLeft - mouseTexture->width)
				{
					mouseTexture->xCord += SDL_JoystickGetAxis(theGamepads[0]->joystick, 0) / 32768.f * 1/4.f;

					///Daca a depasit limita, il aduc intre boundaries
					if(mouseTexture->xCord > heroPosition + CameraExtremeLeft - mouseTexture->width)
					{
						mouseTexture->xCord = heroPosition + CameraExtremeLeft - mouseTexture->width;
					}
					else
						if(mouseTexture->xCord < heroPosition - CameraExtremeLeft + mouseTexture->width)
						{
							mouseTexture->xCord = heroPosition - CameraExtremeLeft + mouseTexture->width;
						}
				}
				else
					if(heroPosition > CameraExtremeRight &&
							mouseTexture->xCord >= CameraExtremeRight - SCREEN_WIDTH / scaleSize / 2.f + mouseTexture->width &&
							mouseTexture->xCord <= CameraExtremeRight + SCREEN_WIDTH / scaleSize / 2.f - mouseTexture->width)
					{
						mouseTexture->xCord += SDL_JoystickGetAxis(theGamepads[0]->joystick, 0) / 32768.f * 1/4.f;

						///Daca a depasit limita, il aduc intre boundaries
						if(mouseTexture->xCord > CameraExtremeRight + SCREEN_WIDTH / scaleSize / 2.f - mouseTexture->width)
						{
							mouseTexture->xCord = CameraExtremeRight + SCREEN_WIDTH / scaleSize / 2.f - mouseTexture->width;
						}
						else
							if(mouseTexture->xCord < CameraExtremeRight - SCREEN_WIDTH / scaleSize / 2.f + mouseTexture->width)
							{
								mouseTexture->xCord = CameraExtremeRight - SCREEN_WIDTH / scaleSize / 2.f + mouseTexture->width;
							}
					}

			mouseTexture->xCord -= CameraPosition->xCord - CameraExtremeLeft;
			mouseTexture->yCord -= CameraPosition->yCord - CameraExtremeDown;
		}
	}

	if((SDL_JoystickGetAxis(theGamepads[0]->joystick, 1) < -3200) || (SDL_JoystickGetAxis(theGamepads[0]->joystick, 1) > 3200))
	{
		float heroPosition = hero->theBody->GetPosition().y;

		mouseTexture->xCord += CameraPosition->xCord - CameraExtremeLeft;
		mouseTexture->yCord += CameraPosition->yCord - CameraExtremeDown;

		///Daca jucatorul e in partea din stanga a ecranului
		if (heroPosition <= CameraExtremeDown && mouseTexture->yCord >= mouseTexture->height &&
				mouseTexture->yCord <= SCREEN_HEIGHT / scaleSize - mouseTexture->height)
		{
			mouseTexture->yCord -= SDL_JoystickGetAxis(theGamepads[0]->joystick, 1) / 32768.f * 1/4.f;

			///Daca a depasit limita, il aduc intre boundaries
			if(mouseTexture->yCord > SCREEN_HEIGHT / scaleSize - mouseTexture->height)
			{
				mouseTexture->yCord = SCREEN_HEIGHT / scaleSize - mouseTexture->height;
			}
			else
				if(mouseTexture->yCord < mouseTexture->height)
				{
					mouseTexture->yCord = mouseTexture->height;
				}
		}
		else
			if(heroPosition > CameraExtremeDown && heroPosition <= CameraExtremeUp &&
					mouseTexture->yCord >= heroPosition - CameraExtremeLeft + mouseTexture->height &&
					mouseTexture->yCord <= heroPosition + CameraExtremeLeft - mouseTexture->height)
			{
				mouseTexture->yCord -= SDL_JoystickGetAxis(theGamepads[0]->joystick, 1) / 32768.f * 1/4.f;

				///Daca a depasit limita, il aduc intre boundaries
				if(mouseTexture->yCord > heroPosition + CameraExtremeDown - mouseTexture->height)
				{
					mouseTexture->yCord = heroPosition + CameraExtremeDown - mouseTexture->height;
				}
				else
					if(mouseTexture->yCord < heroPosition - CameraExtremeDown + mouseTexture->height)
					{
						mouseTexture->yCord = heroPosition - CameraExtremeDown + mouseTexture->height;
					}
			}
			else
				if(heroPosition > CameraExtremeUp &&
						mouseTexture->yCord >= CameraExtremeUp - SCREEN_HEIGHT / scaleSize / 2.f + mouseTexture->height &&
						mouseTexture->yCord <= CameraExtremeUp + SCREEN_HEIGHT / scaleSize / 2.f - mouseTexture->height)
				{
					mouseTexture->yCord -= SDL_JoystickGetAxis(theGamepads[0]->joystick, 1) / 32768.f * 1/4.f;

					///Daca a depasit limita, il aduc intre boundaries
					if(mouseTexture->yCord > CameraExtremeUp + SCREEN_HEIGHT / scaleSize / 2.f - mouseTexture->height)
					{
						mouseTexture->yCord = CameraExtremeUp + SCREEN_HEIGHT / scaleSize / 2.f - mouseTexture->height;
					}
					else
						if(mouseTexture->yCord < CameraExtremeUp - SCREEN_HEIGHT / scaleSize / 2.f + mouseTexture->height)
						{
							mouseTexture->yCord = CameraExtremeUp - SCREEN_HEIGHT / scaleSize / 2.f + mouseTexture->height;
						}
				}

		mouseTexture->xCord -= CameraPosition->xCord - CameraExtremeLeft;
		mouseTexture->yCord -= CameraPosition->yCord - CameraExtremeDown;
	}

	if( event.jaxis.axis == 2)
	{
		if(event.jaxis.value < 0)
		{
			std::cout<<"right stick: left"<<'\n';
		}
		else
			if(event.jaxis.value > 0)
			{
				std::cout<<"right stick: right"<<'\n';
			}
	}

	if( event.jaxis.axis == 3)
	{
		if(event.jaxis.value < 0)
		{
			std::cout<<"right stick: up"<<'\n';
		}
		else
			if(event.jaxis.value > 0)
			{
				std::cout<<"right stick: down"<<'\n';
			}
	}

	SDL_JoystickClose(theJoystick);
}






void BSInputClass::controlTeleportKeyboard()
{
	float moveSpeed = 0.3;

	if(keystates[SDLK_KP4] || keystates[SDLK_4] || keystates[SDLK_KP6] || keystates[SDLK_6])
	{
		float heroPosition = hero->theBody->GetPosition().x;

		mouseTexture->xCord += CameraPosition->xCord - CameraExtremeLeft;
		mouseTexture->yCord += CameraPosition->yCord - CameraExtremeDown;

		///Daca jucatorul e in partea din stanga a ecranului
		if (heroPosition <= CameraExtremeLeft && mouseTexture->xCord >= mouseTexture->width &&
				mouseTexture->xCord <= SCREEN_WIDTH / scaleSize - mouseTexture->width)
		{
			if(keystates[SDLK_KP4] || keystates[SDLK_4])
			{
				mouseTexture->xCord -= moveSpeed;
			}
			else
				if(keystates[SDLK_KP6] || keystates[SDLK_6])
				{
					mouseTexture->xCord += moveSpeed;
				}

			///Daca a depasit limita, il aduc intre boundaries
			if(mouseTexture->xCord > SCREEN_WIDTH / scaleSize - mouseTexture->width)
			{
				mouseTexture->xCord = SCREEN_WIDTH / scaleSize - mouseTexture->width;
			}
			else
				if(mouseTexture->xCord < mouseTexture->width)
				{
					mouseTexture->xCord = mouseTexture->width;
				}
		}
		else
			if(heroPosition > CameraExtremeLeft && heroPosition <= CameraExtremeRight &&
					mouseTexture->xCord >= heroPosition - CameraExtremeLeft + mouseTexture->width &&
					mouseTexture->xCord <= heroPosition + CameraExtremeLeft - mouseTexture->width)
			{
				if(keystates[SDLK_KP4] || keystates[SDLK_4])
				{
					mouseTexture->xCord -= moveSpeed;
				}
				else
					if(keystates[SDLK_KP6] || keystates[SDLK_6])
					{
						mouseTexture->xCord += moveSpeed;
					}

				///Daca a depasit limita, il aduc intre boundaries
				if(mouseTexture->xCord > heroPosition + CameraExtremeLeft - mouseTexture->width)
				{
					mouseTexture->xCord = heroPosition + CameraExtremeLeft - mouseTexture->width;
				}
				else
					if(mouseTexture->xCord < heroPosition - CameraExtremeLeft + mouseTexture->width)
					{
						mouseTexture->xCord = heroPosition - CameraExtremeLeft + mouseTexture->width;
					}
			}
			else
				if(heroPosition > CameraExtremeRight &&
						mouseTexture->xCord >= CameraExtremeRight - SCREEN_WIDTH / scaleSize / 2.f + mouseTexture->width &&
						mouseTexture->xCord <= CameraExtremeRight + SCREEN_WIDTH / scaleSize / 2.f - mouseTexture->width)
				{
					if(keystates[SDLK_KP4] || keystates[SDLK_4])
					{
						mouseTexture->xCord -= moveSpeed;
					}
					else
						if(keystates[SDLK_KP6] || keystates[SDLK_6])
						{
							mouseTexture->xCord += moveSpeed;
						}

					///Daca a depasit limita, il aduc intre boundaries
					if(mouseTexture->xCord > CameraExtremeRight + SCREEN_WIDTH / scaleSize / 2.f - mouseTexture->width)
					{
						mouseTexture->xCord = CameraExtremeRight + SCREEN_WIDTH / scaleSize / 2.f - mouseTexture->width;
					}
					else
						if(mouseTexture->xCord < CameraExtremeRight - SCREEN_WIDTH / scaleSize / 2.f + mouseTexture->width)
						{
							mouseTexture->xCord = CameraExtremeRight - SCREEN_WIDTH / scaleSize / 2.f + mouseTexture->width;
						}
				}

		mouseTexture->xCord -= CameraPosition->xCord - CameraExtremeLeft;
		mouseTexture->yCord -= CameraPosition->yCord - CameraExtremeDown;
	}

	if(keystates[SDLK_KP2] || keystates[SDLK_2] || keystates[SDLK_KP8] || keystates[SDLK_8])
	{
		float heroPosition = hero->theBody->GetPosition().y;

		mouseTexture->xCord += CameraPosition->xCord - CameraExtremeLeft;
		mouseTexture->yCord += CameraPosition->yCord - CameraExtremeDown;

		///Daca jucatorul e in partea din stanga a ecranului
		if (heroPosition <= CameraExtremeDown && mouseTexture->yCord >= mouseTexture->height &&
				mouseTexture->yCord <= SCREEN_HEIGHT / scaleSize - mouseTexture->height)
		{
			if(keystates[SDLK_KP2] || keystates[SDLK_2])
			{
				mouseTexture->yCord -= moveSpeed;
			}
			else
				if(keystates[SDLK_KP8] || keystates[SDLK_8])
				{
					mouseTexture->yCord += moveSpeed;
				}

			///Daca a depasit limita, il aduc intre boundaries
			if(mouseTexture->yCord > SCREEN_HEIGHT / scaleSize - mouseTexture->height)
			{
				mouseTexture->yCord = SCREEN_HEIGHT / scaleSize - mouseTexture->height;
			}
			else
				if(mouseTexture->yCord < mouseTexture->height)
				{
					mouseTexture->yCord = mouseTexture->height;
				}
		}
		else
			if(heroPosition > CameraExtremeDown && heroPosition <= CameraExtremeUp &&
					mouseTexture->yCord >= heroPosition - CameraExtremeLeft + mouseTexture->height &&
					mouseTexture->yCord <= heroPosition + CameraExtremeLeft - mouseTexture->height)
			{
				if(keystates[SDLK_KP2] || keystates[SDLK_2])
				{
					mouseTexture->yCord -= moveSpeed;
				}
				else
					if(keystates[SDLK_KP8] || keystates[SDLK_8])
					{
						mouseTexture->yCord += moveSpeed;
					}

				///Daca a depasit limita, il aduc intre boundaries
				if(mouseTexture->yCord > heroPosition + CameraExtremeDown - mouseTexture->height)
				{
					mouseTexture->yCord = heroPosition + CameraExtremeDown - mouseTexture->height;
				}
				else
					if(mouseTexture->yCord < heroPosition - CameraExtremeDown + mouseTexture->height)
					{
						mouseTexture->yCord = heroPosition - CameraExtremeDown + mouseTexture->height;
					}
			}
			else
				if(heroPosition > CameraExtremeUp &&
						mouseTexture->yCord >= CameraExtremeUp - SCREEN_HEIGHT / scaleSize / 2.f + mouseTexture->height &&
						mouseTexture->yCord <= CameraExtremeUp + SCREEN_HEIGHT / scaleSize / 2.f - mouseTexture->height)
				{
					if(keystates[SDLK_KP2] || keystates[SDLK_2])
					{
						mouseTexture->yCord -= moveSpeed;
					}
					else
						if(keystates[SDLK_KP8] || keystates[SDLK_8])
						{
							mouseTexture->yCord += moveSpeed;
						}

					///Daca a depasit limita, il aduc intre boundaries
					if(mouseTexture->yCord > CameraExtremeUp + SCREEN_HEIGHT / scaleSize / 2.f - mouseTexture->height)
					{
						mouseTexture->yCord = CameraExtremeUp + SCREEN_HEIGHT / scaleSize / 2.f - mouseTexture->height;
					}
					else
						if(mouseTexture->yCord < CameraExtremeUp - SCREEN_HEIGHT / scaleSize / 2.f + mouseTexture->height)
						{
							mouseTexture->yCord = CameraExtremeUp - SCREEN_HEIGHT / scaleSize / 2.f + mouseTexture->height;
						}
				}

		mouseTexture->xCord -= CameraPosition->xCord - CameraExtremeLeft;
		mouseTexture->yCord -= CameraPosition->yCord - CameraExtremeDown;
	}
}

void BSInputClass::getKeyboardState()
{
	controlTeleportKeyboard();

	if(keystates[SDLK_z] && !pressedKeys[SDLK_z])
    {
        if(gravityIsChanging == 0)
        {
            gravityIsChanging = 1;
            startAngle = worldAngle;
        }
        else
            if(gravityIsChanging == -1)
            {
                gravityIsChanging = 1;
                startAngle -= 90;
            }

        pressedKeys[SDLK_z] = true;
    }

	if(keystates[SDLK_x] && !pressedKeys[SDLK_x])
    {
        if(gravityIsChanging == 0)
        {
            gravityIsChanging = -1;
            startAngle = worldAngle;
        }
        else
            if(gravityIsChanging == 1)
            {
                gravityIsChanging = -1;
                startAngle += 90;
            }

        pressedKeys[SDLK_x] = true;
    }

    if(keystates[SDLK_PAGEUP])
    {
        CameraPosition->zCord++;
        scaleSize = CameraPosition->zCord;
        std::cout<<scaleSize<<'\n';
    }
    else
        if(keystates[SDLK_PAGEDOWN])
        {
            CameraPosition->zCord--;
            scaleSize = CameraPosition->zCord;
            std::cout<<scaleSize<<'\n';
        }

	if(keystates[SDLK_a])
	{
		useAssassinsCreed = true;
		useSnow = false;
		useRain = false;
	}
	else
		if(keystates[SDLK_s])
		{
			useAssassinsCreed = false;
			useSnow = true;
			useRain = false;
		}
		else
			if(keystates[SDLK_r])
			{
				useAssassinsCreed = false;
				useSnow = false;
				useRain = true;
			}

	if(keystates[SDLK_F5] && !pressedKeys[SDLK_F5])
	{
		canRefreshGame = true;
		pressedKeys[SDLK_F5] = true;
	}

	///Teleport
	if(keystates[SDLK_KP5] && !pressedKeys[SDLK_KP5])
	{
        objClassAction->ActionFuncTeleportHero(3, 3);
		pressedKeys[SDLK_KP5] = true;
	}
	else
		if(keystates[SDLK_5] && !pressedKeys[SDLK_5])
		{
			objClassAction->ActionFuncTeleportHero(3, 3);
			pressedKeys[SDLK_5] = true;
		}

	///Backspace
	if(keystates[SDLK_BACKSPACE] && !pressedKeys[SDLK_BACKSPACE])
	{
		//sterg ultimul caracter din consola
		if(canWriteInConsole)
		{
			if(consoleString.size() > 0)
			{
				consoleString.erase(consoleString.size() - 1);
				std::cout<<'\n'<<"Pressed Backspace:"<<'\n'<<consoleString;
			}
		}

		pressedKeys[SDLK_BACKSPACE] = true;
	}

	///Run
	if(keystates[heroKeyboard->Run] && !pressedKeys[heroKeyboard->Run]
			&& speedChangedBoostHigh == false)
	{
		speedWalk *= 2;
		speedWalkInAir *= 2;
		speedChangedBoostHigh = true;
	}
	else
		if(!keystates[heroKeyboard->Run] && speedChangedBoostHigh)
		{
			speedWalk /= 2;
			speedWalkInAir /= 2;

			speedChangedBoostHigh = false;
		}

	///Delete
	if(keystates[SDLK_DELETE] && !pressedKeys[SDLK_DELETE])
	{
		if(canWriteInConsole)
		{
			consoleString = "";
		}

		std::cout << '\n' << "Pressed Delete" << '\n';
		pressedKeys[SDLK_DELETE] = true;
	}

	///Time manipulation
	if(keystates[SDLK_1] && !pressedKeys[SDLK_1])
	{
		theTimeHowFast = 120;//slower

//		ALfloat Pitch = 0.7f;
		//alSourcef(SourceSoundMusic1, AL_PITCH, Pitch);
		//alSourcef(SourceSoundMusic2, AL_PITCH, Pitch);
		//alSourcef(SourceSoundMusic3, AL_PITCH, Pitch);
		//alSourcef(SourceSoundPowerUp, AL_PITCH, Pitch);
		//alSourcef(SourceSoundTeleport, AL_PITCH, Pitch + 0.5f);
		pressedKeys[SDLK_1] = true;
	}

	if(keystates[SDLK_2] && !pressedKeys[SDLK_2])
	{
		theTimeHowFast = 60;//faster

//		ALfloat Pitch = 1.f;
		//alSourcef(SourceSoundMusic1, AL_PITCH, Pitch);
		//alSourcef(SourceSoundMusic2, AL_PITCH, Pitch);
		//alSourcef(SourceSoundMusic3, AL_PITCH, Pitch);
		//alSourcef(SourceSoundPowerUp, AL_PITCH, Pitch);
		//alSourcef(SourceSoundTeleport, AL_PITCH, Pitch + 0.5f);
		pressedKeys[SDLK_2] = true;
	}

	if(keystates[SDLK_d])
	{
		DarkLevels = true;
	}
	else
		if(keystates[SDLK_l])
		{
			DarkLevels = false;
		}

    bool canPlayIt = false, finish = false;



	if(keystates[SDLK_b] && !pressedKeys[SDLK_b] && !hero->heroIsBerserk1 && !hero->heroIsBerserk2 && !hero->heroIsBerserk3)
	{
		hero->heroIsBerserk1 = true;
        hero->heroIsBerserk2 = false;
        hero->heroIsBerserk3 = false;

                listOfActiveAnimations[1]->indexOfAnimation = hero->animationIndexPlayerAura1;
                listOfActiveAnimations[1]->setPlayableStatus(true);

		hero->animationIndexCurrentAnimationBerserk = hero->animationIndexPlayerAura1;

		pressedKeys[SDLK_b] = true;
	}
	else
        if(keystates[SDLK_b] && !pressedKeys[SDLK_b] && hero->heroIsBerserk1)
        {
            hero->heroIsBerserk1 = false;
            hero->heroIsBerserk2 = true;
            hero->heroIsBerserk3 = false;

                ///play the last power up ending animation
                listOfActiveAnimations[4]->indexOfAnimation = hero->animationIndexPlayerAura2;
                listOfActiveAnimations[4]->setPlayableStatus(true);

                ///play the 2nd aura
                ///listOfActiveAnimations[1]->canPlayIt = true;
                listOfActiveAnimations[1]->finish = true;

                ///play the electricity
                listOfActiveAnimations[2]->indexOfAnimation = hero->animationIndexPlayerAuraElectricity;
                listOfActiveAnimations[2]->setPlayableStatus(true);

            hero->animationIndexCurrentAnimationBerserk = hero->animationIndexPlayerAura2;

            pressedKeys[SDLK_b] = true;
        }
        else
            if(keystates[SDLK_b] && !pressedKeys[SDLK_b] && hero->heroIsBerserk2)
            {
                listOfActiveAnimations[2]->setPlayableStatus(false);

                hero->heroIsBerserk1 = false;
                hero->heroIsBerserk2 = false;
                hero->heroIsBerserk3 = true;

                listOfActiveAnimations[4]->indexOfAnimation = hero->animationIndexPlayerAura2;
                ///listOfActiveAnimations[4]->canPlayIt = true;
                listOfActiveAnimations[4]->finish = true;

                listOfActiveAnimations[1]->indexOfAnimation = hero->animationIndexPlayerAura3;
                listOfActiveAnimations[1]->setPlayableStatus(true);

                hero->animationIndexCurrentAnimationBerserk = hero->animationIndexPlayerAura3;

                pressedKeys[SDLK_b] = true;
            }
            else
                if(keystates[SDLK_b] && !pressedKeys[SDLK_b] && hero->heroIsBerserk3)
                {
                    hero->heroIsBerserk1 = false;
                    hero->heroIsBerserk2 = false;
                    hero->heroIsBerserk3 = false;

					///listOfActiveAnimations[4]->indexOfAnimation = hero->animationIndexPlayerAura3;
					///listOfActiveAnimations[4]->canPlayIt = true;
					///listOfActiveAnimations[4]->finish = true;

                    ///listOfActiveAnimations[1]->indexOfAnimation = hero->animationIndexPlayerAura1;
                    listOfActiveAnimations[1]->finish = true;
                    hero->IsAtStepOfAnimation21 = false;
                    hero->IsAtStepOfAnimation22 = false;
                    hero->IsAtStepOfAnimation23 = true;

                    pressedKeys[SDLK_b] = true;
                }

	///Escape
	if(keystates[heroKeyboard->Escape] && !pressedKeys[heroKeyboard->Escape])
	{
		ControlAnyMenuEscape();
		pressedKeys[heroKeyboard->Escape] = true;
	}

	//daca se alfa in joc atunci intra in if
	if(currentMenu->indexOfMenu == 3)
	{
		//daca nu poate schimba marimea obiectelor
		if(canChangeScaleSize == false)
		{
			//reseteaza jucatorul la locul initial
			if(keystates[SDLK_F1] && !pressedKeys[SDLK_F1])
			{
				hero->theBody->SetTransform(b2Vec2(HeroInitialX, HeroInitialY), hero->theBody->GetAngle());
				pressedKeys[SDLK_F1] = true;
			}
		}
		//daca schimba marimea obiectelor
		else
		{
			if(!canWriteInConsole)
			{
				if(keystates[SDLK_UP] || keystates[SDLK_RIGHT])
				{
					scaleSize += 1.f;
				}

				if(scaleSize > 1)
				{
					if(keystates[SDLK_LEFT] || keystates[SDLK_DOWN])
					{
						scaleSize -= 1.f;
					}
				}

				consoleString = "change scale size " + BStoString(scaleSize);
			}
		}

	}
	else
	{
		///Check if presses left or right, so I can change sound
		ControlAnyMenuLeftRightUpDown(0);

		if(keystates[heroKeyboard->Up] && !pressedKeys[heroKeyboard->Up])
		{
			ControlAnyMenuLeftRightUpDown(1);
			///In caz ca se misca in nivele
			objClassLevelMenu->walkThroughLevels(0, 1, chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter);
			///In caz ca se misca in chaptere
			objClassLevelMenu->walkThroughChapters(0, 1);
			pressedKeys[heroKeyboard->Up] = true;
		}
		else
			if(keystates[heroKeyboard->Down] && !pressedKeys[heroKeyboard->Down])
			{
				ControlAnyMenuLeftRightUpDown(-1);
				///In caz ca se misca in meniu
				objClassLevelMenu->walkThroughLevels(0, -1, chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter);
				///In caz ca se misca in chaptere
				objClassLevelMenu->walkThroughChapters(0, -1);
				pressedKeys[heroKeyboard->Down] = true;
			}

		///Schimb volumul sunetului
		if(keystates[heroKeyboard->Right] && !pressedKeys[heroKeyboard->Right])
		{
			///In caz ca se misca in meniu
			objClassLevelMenu->walkThroughLevels(1, 0, chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter);
			///In caz ca se misca in chaptere
			objClassLevelMenu->walkThroughChapters(1, 0);

			if(canChangeVolumeMusic && newVolumeAudio < 1)
			{
				newVolumeAudio += 0.1f;
				//alSourcef(SourceSoundMusic1, AL_GAIN, newVolumeAudio);
				//alSourcef(SourceSoundMusic2, AL_GAIN, newVolumeAudio);
				//alSourcef(SourceSoundMusic3, AL_GAIN, newVolumeAudio);
			}
			else
				if(canChangeVolumeAudioSfx && newVolumeSFX < 1)
				{
					newVolumeSFX += 0.1f;
					//alSourcef(SourceSoundDies, AL_GAIN, newVolumeSFX);
					//alSourcef(SourceSoundJump, AL_GAIN, newVolumeSFX);
					//alSourcef(SourceSoundPowerUp, AL_GAIN, newVolumeSFX);
					//alSourcef(SourceSoundBeginningGame, AL_GAIN, newVolumeSFX );
					//alSourcef(SourceSoundTeleport, AL_GAIN, newVolumeSFX );
					//alSourcePlay(SourceSoundJump);
				}

			pressedKeys[heroKeyboard->Right] = true;
		}
		else
			if(keystates[heroKeyboard->Left] && !pressedKeys[heroKeyboard->Left])
			{
				///In caz ca se misca in meniu
				objClassLevelMenu->walkThroughLevels(-1, 0, chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter);
				///In caz ca se misca in chaptere
				objClassLevelMenu->walkThroughChapters(-1, 0);

				if(canChangeVolumeMusic && newVolumeAudio > 0)
				{
					newVolumeAudio -= 0.1f;
					//alSourcef(SourceSoundMusic1, AL_GAIN, newVolumeAudio);
					//alSourcef(SourceSoundMusic2, AL_GAIN, newVolumeAudio);
					//alSourcef(SourceSoundMusic3, AL_GAIN, newVolumeAudio);
				}
				else
					if(canChangeVolumeAudioSfx && newVolumeSFX > 0)
					{
						newVolumeSFX -= 0.1f;
						//alSourcef(SourceSoundDies, AL_GAIN, newVolumeSFX);
						//alSourcef(SourceSoundJump, AL_GAIN, newVolumeSFX);
						//alSourcef(SourceSoundPowerUp, AL_GAIN, newVolumeSFX);
						//alSourcef(SourceSoundBeginningGame, AL_GAIN, newVolumeSFX );
						//alSourcef(SourceSoundTeleport, AL_GAIN, newVolumeSFX );
						//alSourcePlay(SourceSoundJump);
					}

				pressedKeys[heroKeyboard->Left] = true;
			}
	}

	///Left - Right - Jump
	if(currentMenu->indexOfMenu == 3 && !canWriteInConsole)
	{
		if(keystates[heroKeyboard->Up])
		{
			ButtonUp = true;
		}
		else
			if(!keystates[heroKeyboard->Up])
			{
				ButtonUp = false;
			}

		if(keystates[heroKeyboard->Left])
		{
			ButtonLeft = true;
		}
		else
			if(!keystates[heroKeyboard->Left])
			{
				ButtonLeft = false;
			}

		if(keystates[heroKeyboard->Right])
		{
			ButtonRight = true;
		}
		else
			if(!keystates[heroKeyboard->Right])
			{
				ButtonRight = false;
			}

		///if presses jump the first time
		if(keystates[heroKeyboard->Jump] && !jumpPressed && (isOnGround || isOnLeftWall || isOnRightWall))
		{
            isSpaceDown = true;
			ButtonJump = true;
			jumpPressed = true;
		}
		else
			///if he doesn't press space
			if(!keystates[heroKeyboard->Jump])
			{
                isSpaceDown = false;
				jumpPressed = false;
                ButtonJump = false;
			}
			else
				///if he presses jump the second time
				if(keystates[heroKeyboard->Jump] && !jumpPressed &&
                        isInAir && !isInCorner && !isOnLeftWall && !isOnRightWall &&
                        ((!pressedJumpSecond && hero->heroIsBerserk1) || hero->heroIsBerserk2 || hero->heroIsBerserk3 ))
				{
                    isSpaceDown = true;
					pressedJumpSecond = true;
					jumpSecondSpinned = false;
                    ButtonJump = true;
                    jumpPressed = true;
				}
				else
                    ///if he presses jump, but is not the first time
                    ///put !jumpPressed for unlimited jumps
                    if(keystates[heroKeyboard->Jump] && !jumpPressed && pressedJumpSecond)
                    {
                        isSpaceDown = true;
                        ButtonJump = false;
                        jumpPressed = true;
                    }
                    else
                        if(keystates[heroKeyboard->Jump])
                        {
                            isSpaceDown = true;
                            ButtonJump = false;
                            jumpPressed = true;
                        }

	}


	///Enter
	if(keystates[heroKeyboard->Enter] && !pressedKeys[heroKeyboard->Enter])
	{
		//daca se afla in joc atunci poate introduce coduri
		if(currentMenu->indexOfMenu == 3)
		{
			if(canWriteInConsole)
			{
				canWriteInConsole = false;
				std::cout << '\n' << "Not can write in console! " <<consoleString<< '\n';
			}
			else
			{
				canWriteInConsole = true;
				std::cout << '\n' << "Can write in console!" << '\n';
			}
		}
		else
		{
			canWriteInConsole = false;
			ControlAnyMenuEnter();
		}

		pressedKeys[heroKeyboard->Enter] = true;
	}

	if(event.type == SDL_KEYDOWN)
	{
		///Daca e orice altceva in afara de enter, scriu in consola
		if(canWriteInConsole && !pressedKeys[event.key.keysym.sym])
		{
			consoleString += (char)event.key.keysym.sym;

			if(event.key.keysym.sym == SDLK_SPACE)
			{
				std::cout<<'_';
			}
			else
			{
				std::cout<<(char)event.key.keysym.sym;
			}

			pressedKeys[event.key.keysym.sym] = true;
		}
	}
	else
		if(event.type == SDL_KEYUP)
		{
			pressedKeys[event.key.keysym.sym] = false;
		}
}

void BSInputClass::controlGamePad()
{
	///Enter
	if(SDL_JoystickGetButton(theGamepads[0]->joystick, 0) && !theGamepads[0]->buttonIsPressed[0])
	{
		//daca se afla in joc atunci poate introduce coduri
		if(currentMenu->indexOfMenu != 3)
		{
			canWriteInConsole = false;
			ControlAnyMenuEnter();
		}

		theGamepads[0]->buttonIsPressed[0] = true;
	}
	else
		if(!SDL_JoystickGetButton(theGamepads[0]->joystick, 0))
		{
			theGamepads[0]->buttonIsPressed[0] = false;
		}


	///Teleport
	if(SDL_JoystickGetButton(theGamepads[0]->joystick, 2) && !theGamepads[0]->buttonIsPressed[2])
	{
        objClassAction->ActionFuncTeleportHero(3, 3);
		theGamepads[0]->buttonIsPressed[2] = true;
	}
	else
		if(!SDL_JoystickGetButton(theGamepads[0]->joystick, 2))
		{
			theGamepads[0]->buttonIsPressed[2] = false;
		}

	///Run
	if(SDL_JoystickGetButton(theGamepads[0]->joystick, 7) && speedChangedBoostHigh == false)
	{
		speedWalk = speedWalkFast;
		speedWalkInAir = speedWalkInAirFast;
		speedChangedBoostHigh = true;
	}
	else
		if(!SDL_JoystickGetButton(theGamepads[0]->joystick, 7) && speedChangedBoostHigh)
		{
			speedWalk = speedWalkFast / 2;
			speedWalkInAir = speedWalkInAirFast / 2;

			speedChangedBoostHigh = false;
		}


	///Time manipulation
	if(SDL_JoystickGetButton(theGamepads[0]->joystick, 6))
	{
		theTimeHowFast = 120;//slower

//		ALfloat Pitch = 1.f;
		//alSourcef(SourceSoundMusic1, AL_PITCH, Pitch);
		//alSourcef(SourceSoundMusic2, AL_PITCH, Pitch);
		//alSourcef(SourceSoundMusic3, AL_PITCH, Pitch);
		//alSourcef(SourceSoundPowerUp, AL_PITCH, Pitch);
		//alSourcef(SourceSoundTeleport, AL_PITCH, Pitch + 0.5f);
		//pressedKeys[SDLK_1] = true;
	}

	if(SDL_JoystickGetButton(theGamepads[0]->joystick, 4))
	{
		theTimeHowFast = 60;//faster

//		ALfloat Pitch = 1.5f;
		//alSourcef(SourceSoundMusic1, AL_PITCH, Pitch);
		//alSourcef(SourceSoundMusic2, AL_PITCH, Pitch);
		//alSourcef(SourceSoundMusic3, AL_PITCH, Pitch);
		//alSourcef(SourceSoundPowerUp, AL_PITCH, Pitch);
		//alSourcef(SourceSoundTeleport, AL_PITCH, Pitch + 0.5f);
		//pressedKeys[SDLK_2] = true;
	}

	///Escape
	if(SDL_JoystickGetButton(theGamepads[0]->joystick, 9) && !theGamepads[0]->buttonIsPressed[9])
	{
		ControlAnyMenuEscape();
		theGamepads[0]->buttonIsPressed[9] = true;
	}
	else
		if(!SDL_JoystickGetButton(theGamepads[0]->joystick, 9))
		{
			theGamepads[0]->buttonIsPressed[9] = false;
		}

	///Up - Down - Left - Right in the Menus
	//daca se alfa in joc atunci intra in if
	if(currentMenu->indexOfMenu != 3)
	{
		///Check if presses left or right, so I can change sound
		ControlAnyMenuLeftRightUpDown(0);

		if((event.jhat.value & SDL_HAT_UP) && !theGamepads[0]->upHat)
		{
			ControlAnyMenuLeftRightUpDown(1);
			///In caz ca se misca in nivele
			objClassLevelMenu->walkThroughLevels(0, 1, chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter);
			///In caz ca se misca in chaptere
			objClassLevelMenu->walkThroughChapters(0, 1);
			theGamepads[0]->upHat = true;
			theGamepads[0]->downHat = false;
			theGamepads[0]->leftHat = false;
			theGamepads[0]->rightHat = false;
		}
		else
			if((event.jhat.value & SDL_HAT_DOWN) && !theGamepads[0]->downHat)
			{
				ControlAnyMenuLeftRightUpDown(-1);
				///In caz ca se misca in meniu
				objClassLevelMenu->walkThroughLevels(0, -1, chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter);
				///In caz ca se misca in chaptere
				objClassLevelMenu->walkThroughChapters(0, -1);
				theGamepads[0]->upHat = false;
				theGamepads[0]->downHat = true;
				theGamepads[0]->leftHat = false;
				theGamepads[0]->rightHat = false;
			}

		///Schimb volumul sunetului
		if((event.jhat.value & SDL_HAT_RIGHT) && !theGamepads[0]->rightHat)
		{
			///In caz ca se misca in meniu
			objClassLevelMenu->walkThroughLevels(1, 0, chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter);
			///In caz ca se misca in chaptere
			objClassLevelMenu->walkThroughChapters(1, 0);

			if(canChangeVolumeMusic && newVolumeAudio < 1)
			{
				newVolumeAudio += 0.1f;
				//alSourcef(SourceSoundMusic1, AL_GAIN, newVolumeAudio);
				//alSourcef(SourceSoundMusic2, AL_GAIN, newVolumeAudio);
				//alSourcef(SourceSoundMusic3, AL_GAIN, newVolumeAudio);
			}
			else
				if(canChangeVolumeAudioSfx && newVolumeSFX < 1)
				{
					newVolumeSFX += 0.1f;
					//alSourcef(SourceSoundDies, AL_GAIN, newVolumeSFX);
					//alSourcef(SourceSoundJump, AL_GAIN, newVolumeSFX);
					//alSourcef(SourceSoundPowerUp, AL_GAIN, newVolumeSFX);
					//alSourcef(SourceSoundBeginningGame, AL_GAIN, newVolumeSFX );
					//alSourcef(SourceSoundTeleport, AL_GAIN, newVolumeSFX );
					//alSourcePlay(SourceSoundJump);
				}

			theGamepads[0]->upHat = false;
			theGamepads[0]->downHat = false;
			theGamepads[0]->leftHat = false;
			theGamepads[0]->rightHat = true;
		}
		else
			if((event.jhat.value & SDL_HAT_LEFT) && !theGamepads[0]->leftHat)
			{
				///In caz ca se misca in meniu
				objClassLevelMenu->walkThroughLevels(-1, 0, chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter);
				///In caz ca se misca in chaptere
				objClassLevelMenu->walkThroughChapters(-1, 0);

				if(canChangeVolumeMusic && newVolumeAudio > 0)
				{
					newVolumeAudio -= 0.1f;
					//alSourcef(SourceSoundMusic1, AL_GAIN, newVolumeAudio);
					//alSourcef(SourceSoundMusic2, AL_GAIN, newVolumeAudio);
					//alSourcef(SourceSoundMusic3, AL_GAIN, newVolumeAudio);
				}
				else
					if(canChangeVolumeAudioSfx && newVolumeSFX > 0)
					{
						newVolumeSFX -= 0.1f;
						//alSourcef(SourceSoundDies, AL_GAIN, newVolumeSFX);
						//alSourcef(SourceSoundJump, AL_GAIN, newVolumeSFX);
						//alSourcef(SourceSoundPowerUp, AL_GAIN, newVolumeSFX);
						//alSourcef(SourceSoundBeginningGame, AL_GAIN, newVolumeSFX );
						//alSourcef(SourceSoundTeleport, AL_GAIN, newVolumeSFX );
						//alSourcePlay(SourceSoundJump);
					}

				theGamepads[0]->upHat = false;
				theGamepads[0]->downHat = false;
				theGamepads[0]->leftHat = true;
				theGamepads[0]->rightHat = false;
			}
			else
				if(!(event.jhat.value & SDL_HAT_UP) && !(event.jhat.value & SDL_HAT_DOWN) &&
						!(event.jhat.value & SDL_HAT_LEFT) && !(event.jhat.value & SDL_HAT_RIGHT))
				{
					theGamepads[0]->upHat = false;
					theGamepads[0]->downHat = false;
					theGamepads[0]->leftHat = false;
					theGamepads[0]->rightHat = false;
				}

	}


	///Left - Right - Jump in the game
	if(currentMenu->indexOfMenu == 3)
	{
		if(SDL_JoystickGetHat(theGamepads[0]->joystick, 0) & SDL_HAT_UP)
		{
			ButtonUp = true;
		}
		else
			if(!(SDL_JoystickGetHat(theGamepads[0]->joystick, 0) & SDL_HAT_UP))
			{
				ButtonUp = false;
			}

		if(SDL_JoystickGetHat(theGamepads[0]->joystick, 0) & SDL_HAT_LEFT)
		{
			ButtonLeft = true;
		}
		else
			if(!(SDL_JoystickGetHat(theGamepads[0]->joystick, 0) & SDL_HAT_LEFT))
			{
				ButtonLeft = false;
			}

		if(SDL_JoystickGetHat(theGamepads[0]->joystick, 0) & SDL_HAT_RIGHT)
		{
			ButtonRight = true;
		}
		else
			if(!(SDL_JoystickGetHat(theGamepads[0]->joystick, 0) & SDL_HAT_RIGHT))
			{
				ButtonRight = false;
			}


		if(SDL_JoystickGetButton(theGamepads[0]->joystick, 3)
                && !jumpPressed && (isOnGround || isOnLeftWall || isOnRightWall))
		{
            isSpaceDown = true;
			ButtonJump = true;
			jumpPressed = true;
		}
		else
			///if he doesn't press space
			if(!SDL_JoystickGetButton(theGamepads[0]->joystick, 3))
			{
                isSpaceDown = false;
//                        std::cout<<"lala3"<<'\n';
				jumpPressed = false;
                ButtonJump = false;
			}
			else
				///if he presses jump the second time
				if(SDL_JoystickGetButton(theGamepads[0]->joystick, 3) && !jumpPressed &&
                        isInAir && !isInCorner && !isOnLeftWall && !isOnRightWall &&
                        ((!pressedJumpSecond && hero->heroIsBerserk1) || hero->heroIsBerserk2 || hero->heroIsBerserk3 ))
				{
                    isSpaceDown = true;
					pressedJumpSecond = true;
					jumpSecondSpinned = false;
                    ButtonJump = true;
                    jumpPressed = true;
				}
				else
                    ///if he presses jump, but is not the first time
                    ///put !jumpPressed for unlimited jumps
                    if(SDL_JoystickGetButton(theGamepads[0]->joystick, 3) && !jumpPressed && pressedJumpSecond)
                    {
                        isSpaceDown = true;
                        ButtonJump = false;
                        jumpPressed = true;
                    }
                    else
                        if(SDL_JoystickGetButton(theGamepads[0]->joystick, 3))
                        {
                            isSpaceDown = true;
                            ButtonJump = false;
                            jumpPressed = true;
                        }
	}
}

void BSInputClass::handleInput()
{
	SDL_PollEvent( &event );

	if(event.type == SDL_QUIT)
	{
		quit = true;
	}

	keystates = SDL_GetKeyState( NULL );

	if(keystates[SDLK_PLUS] || keystates[SDLK_KP_PLUS] || keystates[SDLK_k])
	{
		useKeyboardOrGamePad = true;
	}
	else
		if((keystates[SDLK_MINUS] || keystates[SDLK_KP_MINUS] || keystates[SDLK_g]) && existsGamePad)
		{
			useKeyboardOrGamePad = false;
		}

    checkAllCoins();
	refreshTheGame();

	if(useKeyboardOrGamePad)
	{
		getKeyboardState();
	}
	else
	{
		for(int i = 0 ; i < nrOfGamepads; i++)
		{
			getButtonsState(theGamepads[i]->indexOfGamepad,
							theGamepads[i]->joystick);
		}
		controlGamePad();
	}
	///handleMouse();
}

void BSInputClass::checkAllCoins()
{
    if(keystates[SDLK_0])
    {
        for(int it = 0; it < listOfCoins.size(); it++)
        {
            listOfCoins[it]->alreadyHadIt = false;
            listOfCoins[it]->available = true;
            listOfCoins[it]->deleteIt = false;
            coinsThisLevel = 0;
            listOfCoins[it]->sinValue = 90;
            listOfCoins[it]->sinValue2 = 0;
            listOfCoins[it]->color.a = 1.0f;
            listOfCoins[it]->xCord = listOfCoins[it]->xInit;
            listOfCoins[it]->yCord = listOfCoins[it]->yInit;
        }
    }
}

float BSInputClass::distBetween2Points(float xA, float yA, float xB, float yB)
{
	return sqrt(double (xA - xB) * (xA - xB) + (yA - yB) * (yA - yB));
}

void BSInputClass::handleMouse()
{
//	float lastFX = MouseX, lastFY = MouseY;
	int newPosX, newPosY;
	SDL_GetMouseState(&newPosX, &newPosY);

	if(SDL_GetTicks() - timeSinceMovedLittleCircle >= 100)
	{
		timeSinceMovedMouse = SDL_GetTicks();
	}
	else
	{
		timeSinceMovedMouse = 0;
	}

	MouseX = newPosX;
	MouseY = newPosY;

	///Mouse animation***********************************************************************
	listOfMouseCircles.back()->xCord = (float) MouseX / cross_platform_scale + CameraPosition->xCord - CameraExtremeLeft;
	listOfMouseCircles.back()->yCord = (float) (SCREEN_HEIGHT - MouseY) / cross_platform_scale + CameraPosition->yCord - CameraExtremeDown;

	float lastX = listOfMouseCircles.back()->xCord, lastY = listOfMouseCircles.back()->yCord, dist, index = 0;
//	long long currTime = SDL_GetTicks();

	for(int i = 20; i >= 0 ; i-=5)
	{
		bool exittt = false;
		for(int it = listOfMouseCircles.size() - 1; it >= 0; it--)
		{
			if(listOfMouseCircles[it] != listOfMouseCircles.back())///check if this line is correct
			{
				index++;
				dist = distBetween2Points(listOfMouseCircles[it]->xCord, listOfMouseCircles[it]->yCord, lastX, lastY);
				///if mouse is moved
				if((int) (dist * 100) > i && (int) dist * 100 != i)
				{
					listOfMouseCircles[it]->xCord = double ((dist - i / 100.0f) * lastX + i / 100.0f * listOfMouseCircles[it]->xCord) / dist;
					listOfMouseCircles[it]->yCord = double ((dist - i / 100.0f) * lastY + i / 100.0f * listOfMouseCircles[it]->yCord) / dist;

					if(listOfMouseCircles[it] == listOfMouseCircles.front())
					{
						timeSinceMovedLittleCircle = SDL_GetTicks();
					}
					exittt = true;
				}
			}

			lastX = listOfMouseCircles[it]->xCord;
			lastY = listOfMouseCircles[it]->yCord;
		}
		if(exittt)
		{
			break;
		}
	}

	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		if(event.button.button == 1)
		{
			std::cout<<"left"<<'\n';
			MouseLeft = true;
		}
		else
			if(event.button.button == 2)
			{
				std::cout<<"middle"<<'\n';
				//MouseLeft
			}
			else
				if(event.button.button == 3)
				{
					std::cout<<"right"<<'\n';
					MouseRight = true;
				}
	}
	else
		if(event.type == SDL_MOUSEBUTTONUP)
		{
			if(event.button.button == 1)
			{
				MouseLeft = false;
				canClickLeft = true;
			}

			if(event.button.button == 2)
			{
				//MouseLeft
			}

			if(event.button.button == 3)
			{
				MouseRight = false;
				canClickRight = true;
			}
		}
}



void BSInputClass::ControlAnyMenuEnter()
{
	///Daca nu se afla in meniul de selectat nivele atunci poate face orice
	if(currentMenu->indexOfMenu != 4 && currentMenu->indexOfMenu != 5)
	{
		///Verific daca sunt in meniul de SETTINGS ca sa stiu daca salvez sau nu datele
		if(currentMenu->indexOfMenu != 2)
		{
			currentVolumeAudio = newVolumeAudio;
			currentVolumeSFX = newVolumeSFX;
		}
		else
		{
			///Verific daca apasa pe SAVE sau CANCEL
			if(currentButton->indexOfButton == 3)//Cancel
			{
				newVolumeAudio = currentVolumeAudio;
				newVolumeSFX = currentVolumeSFX;

				//alSourcef(SourceSoundMusic1, AL_GAIN, newVolumeAudio);
				//alSourcef(SourceSoundMusic2, AL_GAIN, newVolumeAudio);
				//alSourcef(SourceSoundMusic3, AL_GAIN, newVolumeAudio);
				//alSourcef(SourceSoundJump, AL_GAIN, newVolumeSFX);
				//alSourcef(SourceSoundDies, AL_GAIN, newVolumeSFX);
				//alSourcef(SourceSoundBeginningGame, AL_GAIN, newVolumeSFX);
                //alSourcef(SourceSoundPowerUp, AL_GAIN, newVolumeSFX);
                //alSourcef(SourceSoundTeleport, AL_GAIN, newVolumeSFX );
			}
			else
				if(currentButton->indexOfButton == 4)//Save
				{
					currentVolumeAudio = newVolumeAudio;
					currentVolumeSFX = newVolumeSFX;
				}
		}

		///Daca apasa pe butonul de EXIT
		if(currentButton->NextMenuIndex == -3)
		{
			quit = true;
		}
		else
			///Daca se plimba prin meniuri
			if(currentButton->NextMenuIndex >= 0)
			{
				///Vad din ce meniu ajunge in SETTINGS ca sa stiu unde ma intorc
				if(currentMenu->indexOfMenu == 0 && currentButton->indexOfButton == 1)//Din First Menu
				{
					settingsFromFirstOrGame = true;
				}
				else
					if(currentMenu->indexOfMenu == 1 && currentButton->indexOfButton == 2)//Din Pause Menu
					{
						settingsFromFirstOrGame = false;
					}
					else
						if(currentMenu->indexOfMenu == 1 && currentButton->indexOfButton == 1)//Daca da Restart
						{
								for(int it = 0; it < listOfCoins.size(); it++)
								{
									listOfCoins[it]->available = true;
									listOfCoins[it]->deleteIt = false;
									coinsThisLevel = 0;
												listOfCoins[it]->sinValue = 90;
												listOfCoins[it]->sinValue2 = 0;
												listOfCoins[it]->color.a = 1.0f;
												listOfCoins[it]->xCord = listOfCoins[it]->xInit;
												listOfCoins[it]->yCord = listOfCoins[it]->yInit;
								}

                            someRender->resetDustBodies();

							hero->heroIsDead = false;
							hero->texture = playerTexture;
							canJumpHorizontally = false;
							canJumpVertically = false;
							isOnGround = false;
							isInAir = true;
							isOnLeftWall = false;
							isOnRightWall = false;
							hero->theBody->SetTransform(b2Vec2(HeroInitialX, HeroInitialY), 0);
							hero->theBody->ApplyLinearImpulse(b2Vec2(0, 0.1f), hero->theBody->GetWorldCenter());
						}

				///Setez in ce meniu se duce din cel de SETTINGS, exemplu: ori in First Menu, ori Pause Menu
				if(currentMenu->indexOfMenu == 2 && (currentButton->indexOfButton == 3 || currentButton->indexOfButton == 4))
				{
					for(int i = 0; i < listOfMenus.size(); i++)
					{
						if(settingsFromFirstOrGame)
						{
							if(listOfMenus[i]->indexOfMenu == 0)
							{
								currentMenu = listOfMenus[i];
								currentButton = listOfMenus[i]->listOfButtons.front();
								break;
							}
						}
						else
						{
							if(listOfMenus[i]->indexOfMenu == 1)
							{
								currentMenu = listOfMenus[i];
								currentButton = listOfMenus[i]->listOfButtons.front();
								break;
							}

						}
					}
				}
				else
					///Daca se afla in oricare parte a meniului
				{
					std::cout<<"the Menu: "<<currentMenu->indexOfMenu<<"; current button: "<<currentButton->indexOfButton<<
							 "; next menu from button: "<<currentButton->NextMenuIndex<<"; found: ";

					//daca e in pause menu
					if(currentMenu->indexOfMenu == 1 && currentButton->NextMenuIndex == 4)
					{
                        //alSourcePlay(SourceSoundBeginningGame);
                        //refac coinsurile
                        for(int it = 0; it < listOfCoins.size(); it++)
                        {
                            if(listOfCoins[it]->alreadyHadIt == false)
                            {
                                listOfCoins[it]->available = true;
                                listOfCoins[it]->deleteIt = false;
                                coinsThisLevel = 0;
                                            listOfCoins[it]->sinValue = 90;
                                            listOfCoins[it]->sinValue2 = 0;
                                            listOfCoins[it]->color.a = 1.0f;
                                            listOfCoins[it]->xCord = listOfCoins[it]->xInit;
                                            listOfCoins[it]->yCord = listOfCoins[it]->yInit;
                            }
                        }
					}

					for(int i = 0; i < listOfMenus.size(); i++)
					{
						if(listOfMenus[i]->indexOfMenu == currentButton->NextMenuIndex)
						{
							currentMenu = listOfMenus[i];
							if(listOfMenus[i]->listOfButtons.size())
							{
							    currentButton = listOfMenus[i]->listOfButtons.front();
							}
							else
                            {
                                currentButton = NULL;
                            }
							break;
						}
					}
				}
			}
	}
	else
		if(currentMenu->indexOfMenu == 4 && levelChoserPlayer->canMove)
		{
			std::string nameString = "Menu Of Levels/Chapter" +
									 BStoString(levelChoserPlayer->indexOfChapterThatItIsOn + 1) + "/The Levels objects/level" +
									 BStoString(levelChoserPlayer->indexOfLevelThatItIsOn + 1) + ".xml";
			char nameChar[100] = "";

			strcpy(nameChar, nameString.c_str());

			//alSourcePlay(SourceSoundBeginningGame);

			objClassObstacle->destroyLevel();
			objClassObstacle->incarcaHarta(nameChar);

			for(int i = 0; i < listOfMenus.size(); i++)
			{
				if(listOfMenus[i]->indexOfMenu == 3)
				{
					currentMenu = listOfMenus[i];

					for(int j = 0; j < currentMenu->listOfButtons.size(); j++)
					{
						if(currentMenu->listOfButtons[j]->indexOfButton == 0)
						{
							currentButton = currentMenu->listOfButtons[j];
						}

						break;
					}
					break;
				}
			}
		}
		else
			if(currentMenu->indexOfMenu == 5 && levelChoserPlayer->canMove)
			{
				for(int i = 0; i < listOfMenus.size(); i++)
				{
					if(listOfMenus[i]->indexOfMenu == 4)
					{
						currentMenu = listOfMenus[i];

						for(int j = 0; j < currentMenu->listOfButtons.size(); j++)
						{
							if(currentMenu->listOfButtons[j]->indexOfButton == 0)
							{
								currentButton = currentMenu->listOfButtons[j];
							}

							break;
						}

						break;
					}
				}

				levelChoserPlayer->xCord = chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter[0]->xCord;
				levelChoserPlayer->yCord = chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter[0]->yCord;
			}
}

void changeMenuAndButtonsIndexes(int wantedMenuIndex, int wantedButtonIndex)
{
    for(int i = 0; i < listOfMenus.size(); i++)
    {
        if(listOfMenus[i]->indexOfMenu == wantedMenuIndex)
        {
            currentMenu = listOfMenus[i];

            for(int j = 0; j < currentMenu->listOfButtons.size(); j++)
            {
                if(currentMenu->listOfButtons[j]->indexOfButton == wantedButtonIndex)
                {
                    currentButton = currentMenu->listOfButtons[j];
                }
                break;
            }
            break;
        }
    }
}

void BSInputClass::ControlAnyMenuEscape()
{
	///Daca e in joc, trece in Pause Menu
	if(currentMenu->indexOfMenu == 3)
	{
	    changeMenuAndButtonsIndexes(1, 0);
	}
	else
		///Daca e in Levels, trece in Chapters
		if(currentMenu->indexOfMenu == 4)
		{
		    changeMenuAndButtonsIndexes(5, 0);

			levelChoserPlayer->xCord = chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->xCord;
			levelChoserPlayer->yCord = chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->yCord;

			levelChoserPlayer->indexOfLevelThatItIsOn = 0;
		}
		else
			///Daca e in Chapters, trece in Levels
			if(currentMenu->indexOfMenu == 5)
			{
			    changeMenuAndButtonsIndexes(0, 0);
			}
			else
				///Daca e in Pause Menu, trece in joc
				if(currentMenu->indexOfMenu == 1)
				{
				    changeMenuAndButtonsIndexes(3, 0);
				}
				else
					///Daca e in Settings
					if(currentMenu->indexOfMenu == 2)
					{
                        if(settingsFromFirstOrGame)
                        {
                            changeMenuAndButtonsIndexes(0, 0);
                        }
                        else
                        {
                            changeMenuAndButtonsIndexes(1, 0);
                        }

						newVolumeAudio = currentVolumeAudio;
						newVolumeSFX = currentVolumeSFX;

						//alSourcef(SourceSoundMusic1, AL_GAIN, newVolumeAudio);
						//alSourcef(SourceSoundMusic2, AL_GAIN, newVolumeAudio);
						//alSourcef(SourceSoundMusic3, AL_GAIN, newVolumeAudio);
						//alSourcef(SourceSoundJump, AL_GAIN, newVolumeSFX);
						//alSourcef(SourceSoundDies, AL_GAIN, newVolumeSFX);
						//alSourcef(SourceSoundBeginningGame, AL_GAIN, newVolumeSFX);
						//alSourcef(SourceSoundPowerUp, AL_GAIN, newVolumeSFX);
                        //alSourcef(SourceSoundTeleport, AL_GAIN, newVolumeSFX );
					}
}

void BSInputClass::ControlAnyMenuLeftRightUpDown(int upOrDown)
{
	///Daca nu se afla in meniul de selectat nivele atunci poate face orice
	if(currentMenu->indexOfMenu != 4 && currentMenu->indexOfMenu != 5)
	{
		if(upOrDown == 1)
		{
            currentButton->value1 = 0;
            currentButton->value2 = 0;

			if(currentButton->indexOfButton > 0)
			{
				for(int i = 0; i < currentMenu->listOfButtons.size(); i++)
				{
					if(currentMenu->listOfButtons[i]->indexOfButton == currentButton->indexOfButton - 1)
					{
						currentButton = currentMenu->listOfButtons[i];
						break;
					}
				}
			}
		}
		else
			if(upOrDown == -1)
			{
                currentButton->value1 = 0;
                currentButton->value2 = 0;

				if(currentButton->indexOfButton < currentMenu->numberOfButtons - 1)
				{
					for(int i = 0; i < currentMenu->listOfButtons.size(); i++)
					{
						if(currentMenu->listOfButtons[i]->indexOfButton == currentButton->indexOfButton + 1)
						{
							currentButton = currentMenu->listOfButtons[i];
							break;
						}
					}
				}
			}

		///Daca vrea sa modifice volumul sau rezolutia
		if(currentButton->NextMenuIndex == -2)
		{
			if(currentButton->indexOfButton == 0)
			{
				canChangeVolumeMusic = true;
				canChangeVolumeAudioSfx = false;
				canChangeResolution = false;
			}
			else
				if(currentButton->indexOfButton == 1)
				{
					canChangeVolumeMusic = false;
					canChangeVolumeAudioSfx = true;
					canChangeResolution = false;
				}
				else
					if(currentButton->indexOfButton == 2)
					{
						canChangeVolumeMusic = false;
						canChangeVolumeAudioSfx = false;
						canChangeResolution = true;
					}
		}
	}
}

void BSInputClass::refreshTheGame()
{
	if(canRefreshGame)
	{
		refreshLevel();

		refreshShaders();

		canRefreshGame = false;
	}
}

void BSInputClass::refreshLevel()
{
	objClassObstacle->destroyLevel();

	std::string nameString = "Menu Of Levels/Chapter" +
							 BStoString(levelChoserPlayer->indexOfChapterThatItIsOn + 1) + "/The Levels objects/level" +
							 BStoString(levelChoserPlayer->indexOfLevelThatItIsOn + 1) + ".xml";
	char nameChar[100] = "";
	strcpy(nameChar, nameString.c_str());

	objClassObstacle->incarcaHarta(nameChar);


	hero->theBody->SetTransform(b2Vec2(HeroInitialX, HeroInitialY), 0);



	///Set the extremes of the camera
	std::string nameOfLevel = "Menu Of Levels/Chapter" +
							  BStoString(levelChoserPlayer->indexOfChapterThatItIsOn + 1) + "/The Levels details/level" +
							  BStoString(levelChoserPlayer->indexOfLevelThatItIsOn + 1) + ".txt";
	const char *nameOfLevelChar = nameOfLevel.c_str();

	std::ifstream citesteLevelulCurent(nameOfLevelChar);
	float a1, a2, l, d, u, r;
	std::string cacat;
	citesteLevelulCurent>>a1>>a2>>cacat>>l>>d>>u>>r;

	CameraExtremeDown = d + SCREEN_HEIGHT / 2.f / scaleSize;
	CameraExtremeLeft = l + SCREEN_WIDTH / 2.f / scaleSize;
	CameraExtremeUp = u - SCREEN_HEIGHT / 2.f / scaleSize;
	CameraExtremeRight = r - SCREEN_WIDTH / 2.f / scaleSize;

	std::cout<<"extremes: "<<l<<" "<<d<<" "<<u<<" "<<r<<'\n';
}

void BSInputClass::refreshShaders()
{
	BSUtilities *util = new BSUtilities();
	util->loadGP();
	delete util;
}

void cleanAndCloseSDL()
{
	SDL_Quit();
}
