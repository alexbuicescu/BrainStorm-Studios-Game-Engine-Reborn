#pragma once
#ifndef LEVELMENU_H_INCLUDED
#define LEVELMENU_H_INCLUDED

#include <iostream>
#include <fstream>
#include "BSValues.h"
#include "BSObstacles.h"

class BSLevelMenu
{
public:

	void loadLevels(std::string chapterName, ChapterLevelsStructure* theLevelsToAdd[],
					int nrOfLevels, int chapterIndex);

	void loadChapters();
	void loadUserData();

	void walkThroughLevels(int leftRight, int UpDown, ChapterLevelsStructure* theLevelsToAdd[]);
	void walkThroughChapters(int leftRight, int UpDown);

	void walkingAnimation(float xDestination, float yDestination);
	void chooseLevel(bool pressedEnter);
};


#endif // LEVELMENU_H_INCLUDED
