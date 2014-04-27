#pragma once
#ifndef BSInputClass_H
#define BSInputClass_H

#include <iostream>
#include <SDL/SDL.h>
#include "BSValues.h"
#include "BSObstacles.h"
#include "BSLevelMenu.h"
#include "BSXmlFiles.h"
#include "BSActionStuff.h"
#include "BSTexture.h"



class BSInputClass
{
public:

	BSInputClass();

	void setTheVideoDisplayAndWindow();

	void initGamePad();

	void getButtonsState(int gamePadIndex, SDL_Joystick *theJoystick);
	void getKeyboardState();

	void controlTeleportKeyboard();
	void handleInput();
	void handleKeyboard();
	void controlGamePad();

    void checkAllCoins();

	void handleMouse();

	void moveTheMouse();


	void refreshTheGame();
	void refreshLevel();
	void refreshShaders();




	void ControlAnyMenuEnter();
	void ControlAnyMenuEscape();
	void ControlAnyMenuLeftRightUpDown(int upOrDown);

	void cleanAndCloseSDL();
	float distBetween2Points(float xA, float yA, float xB, float yB);

};

#endif // BSInputClass_H
