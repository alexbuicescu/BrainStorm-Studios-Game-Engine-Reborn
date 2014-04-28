#include "BSLevelMenu.h"

void BSLevelMenu::loadLevels(std::string chapterName, ChapterLevelsStructure* theLevelsToAdd[], int nrOfLevels, int chapterIndex)
{
	BSXmlFiles *doc = new BSXmlFiles();
	std::vector<std::string> theNodes, theAttributes;
	std::vector<ListaDeCaractere*> levelDetails;
	std::vector<std::string> levelDetailsAttributes;

	theNodes.push_back("Level_index");
	theNodes.push_back("Level_texture_name");

	theAttributes.push_back("nr");

	doc->readAnyXML("The Levels/playerDetails" + BStoString(chapterIndex + 1) + ".xml", "Chapter_of_level", "Level_details",
					theNodes, theAttributes, levelDetails, levelDetailsAttributes);
	int i = 0;


	while(i < nrOfLevels)
	{
		std::string nameOfTheTexture = "";
		theLevelsToAdd[i] = new ChapterLevelsStructure;
		theLevelsToAdd[i]->indexOfLevel = atof(levelDetails[i]->theCharArray2D[0].c_str());///i;
		theLevelsToAdd[i]->width = Hero_size * 1.7f;// + 0.15;
		theLevelsToAdd[i]->height = Hero_size * 1.7f;// + 0.15;

		theLevelsToAdd[i]->textOnLevel =  BStoString(i + 1);

		chapterName = "Chapter" + BStoString(atof(levelDetailsAttributes[0].c_str())); /// BStoString(atof(levelDetails[i]->theCharArray2DAttributes[0]));

		///Incarc texturile de conexiune intre theLevelsToAdd, doar pentru primul nivel, ca sa nu incarc memoria ram
		if(i == 0)
		{
			std::string name1 = "Menu Of Levels/" + chapterName + "/connectionLeftRight.png",
						name2 = "Menu Of Levels/" + chapterName + "/connectionUpDown.png",
						name3 = "Menu Of Levels/" + chapterName + "/levelFinished.png";
			const char *name1Char = name1.c_str(), *name2Char = name2.c_str(), *name3Char = name3.c_str();

			BS_Renderer::load_texture(name1Char, theLevelsToAdd[0]->textureOfConnectionLeftRight);
			BS_Renderer::load_texture(name2Char, theLevelsToAdd[0]->textureOfConnectionUpDown);
			BS_Renderer::load_texture(name3Char, theLevelsToAdd[0]->textureOfFinishedLevel);
			BS_Renderer::load_texture("Menu Of Chapters/buttons menu background.png", theLevelsToAdd[0]->textureOfLevel);
		}


		float spaceBetweenLevels = 1.9;
		///Pozitionez nivelul
		if(i % 5 == 0)
		{
			///Il pun cat mai la stanga
			theLevelsToAdd[i]->xCord = (float) SCREEN_WIDTH / 2 / cross_platform_scale - 2 * (theLevelsToAdd[i]->width + spaceBetweenLevels);//2;

			///Verifc daca este pe primul rand sau nu
			if(i > 0)
			{
				theLevelsToAdd[i]->yCord = (float) theLevelsToAdd[i - 1]->yCord - theLevelsToAdd[i]->height - spaceBetweenLevels;
			}
			else
				///Daca este primul nivel
			{
				theLevelsToAdd[i]->yCord = (float) SCREEN_HEIGHT / 2 / cross_platform_scale + 2 * (theLevelsToAdd[i]->height + spaceBetweenLevels);//SCREEN_HEIGHT / 32 - 2;
			}
		}
		else
		{
			theLevelsToAdd[i]->xCord = theLevelsToAdd[i - 1]->xCord + theLevelsToAdd[i]->width + spaceBetweenLevels;
			theLevelsToAdd[i]->yCord = theLevelsToAdd[i - 1]->yCord;
		}

		theLevelsToAdd[i]->isFinished = 0;
		theLevelsToAdd[i]->canPlayIt = 0;
		nameOfTheTexture = levelDetails[i]->theCharArray2D[1];

		///Daca e terminat nivelul atunci il fac mai luminos(pe else)
		if(!theLevelsToAdd[i]->canPlayIt)
		{
			theLevelsToAdd[i]->color = {0.5f, 0.5f, 0.5f, 1.f};
		}
		else
		{
			theLevelsToAdd[i]->color = {1.f, 1.f, 1.f, 1.f};
		}

		theLevelsToAdd[i]->zCord = scaleSizeInitial;

		delete levelDetails[i];
		i++;
	}

	delete doc;
	levelDetailsAttributes.clear();

	///Set the animation of the texture that i am going to use to select the level i want to play
	levelChoserPlayer->width = (Hero_size - 0.4f) * 2.0f;
	levelChoserPlayer->height = (Hero_size - 0.4f) * 2.0f;
	levelChoserPlayer->xCord = theLevelsToAdd[0]->xCord;
	levelChoserPlayer->yCord = theLevelsToAdd[0]->yCord;
	levelChoserPlayer->color = {1.f, 1.f, 1.f, 1.f};
	levelChoserPlayer->indexOfLevelThatItIsOn = 0;
	levelChoserPlayer->canMove = true;

	std::string levelChooserTextureNameString = "Menu Of Levels/" + chapterName + "/playerTextureLevelChooser.png";
	const char *levelChooserTextureNameChar = levelChooserTextureNameString.c_str();
	BS_Renderer::load_texture(levelChooserTextureNameChar, levelChoserPlayer->texture);

	levelChoserPlayer->justFinished = false;
}

void BSLevelMenu::loadChapters()
{
	std::string nameOfChapter = "";

	BSXmlFiles *doc = new BSXmlFiles();
	std::vector<std::string> theNodes, theAttributes;
	std::vector<ListaDeCaractere*> levelDetails;
	std::vector<std::string> levelDetailsAttributes;

	theNodes.push_back("Chapter_name");
	theNodes.push_back("Chapter_nr_of_levels");
	theNodes.push_back("Chapter_is_finished");
	theNodes.push_back("Chapter_is_available");
	theNodes.push_back("Chapter_x_coord");
	theNodes.push_back("Chapter_y_coord");
	theNodes.push_back("Chapter_width");
	theNodes.push_back("Chapter_height");

	theAttributes.push_back("nr_of_chapters");

	doc->readAnyXML("Menu Of Chapters/gameDetails.xml", "Game_details", "Chapter_details",
					theNodes, theAttributes, levelDetails, levelDetailsAttributes);

	nrOfChapters = atof(levelDetailsAttributes[0].c_str());

	for(int i = 0 ; i < nrOfChapters ; i++)
	{
		chaptere[i] = new ChapterStructure;

		nameOfChapter = levelDetails[i]->theCharArray2D[0];
		chaptere[i]->nrOfLevels = atof(levelDetails[i]->theCharArray2D[1].c_str());
		chaptere[i]->isFinishedChapter = atof(levelDetails[i]->theCharArray2D[2].c_str());
		chaptere[i]->isAvailableChapter = atof(levelDetails[i]->theCharArray2D[3].c_str());
		chaptere[i]->xCord = atof(levelDetails[i]->theCharArray2D[4].c_str());
		chaptere[i]->yCord = atof(levelDetails[i]->theCharArray2D[5].c_str());
		chaptere[i]->width = (atof(levelDetails[i]->theCharArray2D[6].c_str())) * 1.8f;
		chaptere[i]->height = (atof(levelDetails[i]->theCharArray2D[7].c_str())) * 1.8f;

		chaptere[i]->textOnChapter =  BStoString(i + 1);

		chaptere[i]->xCord *= (float) (SCREEN_WIDTH / 37);
		chaptere[i]->xCord += 1;

		chaptere[i]->yCord *= (float) (SCREEN_HEIGHT / 37);
		chaptere[i]->zCord = scaleSizeInitial;

		chaptere[i]->color = {1.f, 1.f, 1.f, 1.f};



		loadLevels(nameOfChapter, chaptere[i]->levelFromChapter, chaptere[i]->nrOfLevels, i);

		std::string textureName = "Menu Of Chapters/The Chapters textures/chapterTexture" + BStoString(i + 1) + ".png";
		BS_Renderer::load_texture(textureName.c_str(), chaptere[i]->texture);

		levelChoserPlayer->indexOfChapterThatItIsOn = 0;

		BS_Renderer::load_texture("Menu Of Chapters/connectionLeftRight.png", chaptere[i]->levelFromChapter[0]->textureOfConnectionLeftRightChapter);

		BS_Renderer::load_texture("Menu Of Chapters/connectionUpDown.png", chaptere[i]->levelFromChapter[0]->textureOfConnectionUpDownChapter);

		levelChoserPlayer->xCord = chaptere[0]->xCord;
		levelChoserPlayer->yCord = chaptere[0]->yCord;
		levelChoserPlayer->zCord = scaleSizeInitial;

		delete levelDetails[i];
	}

	delete doc;
	levelDetailsAttributes.clear();

	loadUserData();
}


void BSLevelMenu::loadUserData()
{
	for(int i = 0; i < 6; i++)
	{
		std::string file = "Saves/playerData" + BStoString(i + 1) + ".txt";
		std::ifstream fin(file.c_str());
//		std::string someFile = getFile(file.c_str());
//		std::stringstream fin;
//		fin << someFile;

		int chapterIndex, nrOfCurrentLevels, levelIndex;///, done, available;
		fin >> chapterIndex >> nrOfCurrentLevels;

		for(int j = 0 ; j < nrOfCurrentLevels; j++)
		{
			fin >> levelIndex;
			fin >> chaptere[i]->levelFromChapter[j]->isFinished >>
				chaptere[i]->levelFromChapter[j]->canPlayIt;
			if(chaptere[i]->levelFromChapter[j]->canPlayIt)
			{
				chaptere[i]->levelFromChapter[j]->color = {1, 1, 1, 1};
			}
		}

		//fin.close();
	}
}


void BSLevelMenu::walkThroughLevels(int leftRight, int UpDown, ChapterLevelsStructure* theLevelsToAdd[])
{
	///In caz ca se afla in meniul de nivele, si jucatorul nu se misca (ceea ce inseamna ca poate alege un nivel)
	if(currentMenu->indexOfMenu == 4 && levelChoserPlayer->canMove)
	{
		///In caz ca merge la stanga
		if(leftRight == -1 && levelChoserPlayer->indexOfLevelThatItIsOn > 0)
		{
			///In caz ca nu este primul de pe rand
			if( levelChoserPlayer->indexOfLevelThatItIsOn % 5 != 0
					&& theLevelsToAdd[levelChoserPlayer->indexOfLevelThatItIsOn - 1]->canPlayIt)
			{
				levelChoserPlayer->indexOfLevelThatItIsOn--;
				levelChoserPlayer->canMove = false;
				levelChoserPlayer->isMovingLeftRight = -1;
				levelChoserPlayer->isMovingUpDown = 0;
			}
		}
		else
			///In caz ca merge la stanga
			if(leftRight == 1 && levelChoserPlayer->indexOfLevelThatItIsOn < 25
					&& theLevelsToAdd[levelChoserPlayer->indexOfLevelThatItIsOn + 1]->canPlayIt)
			{
				if( (levelChoserPlayer->indexOfLevelThatItIsOn + 1) % 5 != 0 )
				{
					levelChoserPlayer->indexOfLevelThatItIsOn++;
					levelChoserPlayer->canMove = false;
					levelChoserPlayer->isMovingLeftRight = 1;
					levelChoserPlayer->isMovingUpDown = 0;
				}
			}
			else
				///In caz ca merge sus
				if(UpDown == 1 && levelChoserPlayer->indexOfLevelThatItIsOn > 4
						&& theLevelsToAdd[levelChoserPlayer->indexOfLevelThatItIsOn - 5]->canPlayIt)
				{
					levelChoserPlayer->indexOfLevelThatItIsOn -= 5;
					levelChoserPlayer->canMove = false;
					levelChoserPlayer->isMovingLeftRight = 0;
					levelChoserPlayer->isMovingUpDown = 1;
				}
				else
					///In caz ca merge jos
					if(UpDown == -1 && levelChoserPlayer->indexOfLevelThatItIsOn < 20
							&& theLevelsToAdd[levelChoserPlayer->indexOfLevelThatItIsOn + 5]->canPlayIt)
					{
						levelChoserPlayer->indexOfLevelThatItIsOn += 5;
						levelChoserPlayer->canMove = false;
						levelChoserPlayer->isMovingLeftRight = 0;
						levelChoserPlayer->isMovingUpDown = -1;
					}
	}
	else
		///In caz ca jucatorul inca se misca ca sa ajunga la nivel
		if(!levelChoserPlayer->canMove && currentMenu->indexOfMenu == 4)
		{
			int indexOfWantedLevel = levelChoserPlayer->indexOfLevelThatItIsOn;
			walkingAnimation(theLevelsToAdd[indexOfWantedLevel]->xCord,
							 theLevelsToAdd[indexOfWantedLevel]->yCord);
		}
}


void BSLevelMenu::walkThroughChapters(int leftRight, int UpDown)
{
	///In caz ca se afla in meniul de nivele, si jucatorul nu se misca (ceea ce inseamna ca poate alege un nivel)
	if(currentMenu->indexOfMenu == 5 && levelChoserPlayer->canMove)
	{
		///In caz ca merge la stanga
		if(leftRight == -1)/// || UpDown == -1)
		{
			int theNextIndex = levelChoserPlayer->indexOfChapterThatItIsOn - 1;
			///In caz ca are unde sa se duca la stanga
			///In caz ca cel din stanga e pe acelasi Y dar cu X mai mic
			if(theNextIndex >= 0 && chaptere[theNextIndex]->xCord < chaptere[theNextIndex + 1]->xCord
					&& chaptere[theNextIndex]->isAvailableChapter)
			{
				levelChoserPlayer->indexOfChapterThatItIsOn--;
				levelChoserPlayer->canMove = false;
				levelChoserPlayer->isMovingLeftRight = -1;
				levelChoserPlayer->isMovingUpDown = 0;
			}
			else
				if(theNextIndex + 2 < nrOfChapters && chaptere[theNextIndex + 2]->xCord < chaptere[theNextIndex + 1]->xCord
						&& chaptere[theNextIndex + 2]->isAvailableChapter)
				{
					levelChoserPlayer->indexOfChapterThatItIsOn++;
					levelChoserPlayer->canMove = false;
					levelChoserPlayer->isMovingLeftRight = -1;
					levelChoserPlayer->isMovingUpDown = 0;
				}
		}
		else
			///In caz ca merge la dreapta
			if(leftRight == 1)/// || UpDown == -1)
			{
				int theNextIndex = levelChoserPlayer->indexOfChapterThatItIsOn + 1;
				///In caz ca are unde sa se duca la stanga
				///In caz ca cel din stanga e pe acelasi Y dar cu X mai mic
				if(theNextIndex < nrOfChapters && chaptere[theNextIndex]->xCord > chaptere[theNextIndex - 1]->xCord
						&& chaptere[theNextIndex]->isAvailableChapter)
				{
					levelChoserPlayer->indexOfChapterThatItIsOn++;
					levelChoserPlayer->canMove = false;
					levelChoserPlayer->isMovingLeftRight = 1;
					levelChoserPlayer->isMovingUpDown = 0;
				}
				else
					if(theNextIndex - 2 >= 0 && chaptere[theNextIndex - 2]->xCord > chaptere[theNextIndex - 1]->xCord
							&& chaptere[theNextIndex - 2]->isAvailableChapter)
					{
						levelChoserPlayer->indexOfChapterThatItIsOn--;
						levelChoserPlayer->canMove = false;
						levelChoserPlayer->isMovingLeftRight = 1;
						levelChoserPlayer->isMovingUpDown = 0;
					}
			}
			else
				///Daca se duce in jos
				if(UpDown == -1)/// || UpDown == -1)
				{
					int theNextIndex = levelChoserPlayer->indexOfChapterThatItIsOn - 1;
					///In caz ca are unde sa se duca la stanga
					///In caz ca cel din stanga e pe acelasi Y dar cu X mai mic
					if(theNextIndex >= 0 && chaptere[theNextIndex]->yCord < chaptere[theNextIndex + 1]->yCord
							&& chaptere[theNextIndex]->isAvailableChapter)
					{
						levelChoserPlayer->indexOfChapterThatItIsOn--;
						levelChoserPlayer->canMove = false;
						levelChoserPlayer->isMovingLeftRight = 0;
						levelChoserPlayer->isMovingUpDown = -1;
					}
					else
						if(theNextIndex + 2 < nrOfChapters && chaptere[theNextIndex + 2]->yCord < chaptere[theNextIndex + 1]->yCord
								&& chaptere[theNextIndex + 2]->isAvailableChapter)
						{
							levelChoserPlayer->indexOfChapterThatItIsOn++;
							levelChoserPlayer->canMove = false;
							levelChoserPlayer->isMovingLeftRight = 0;
							levelChoserPlayer->isMovingUpDown = -1;
						}
				}
				else
					///Daca se duce in sus
					if(UpDown == 1)/// || UpDown == -1)
					{
						int theNextIndex = levelChoserPlayer->indexOfChapterThatItIsOn + 1;
						///In caz ca are unde sa se duca la stanga
						///In caz ca cel din stanga e pe acelasi Y dar cu X mai mic
						if(theNextIndex < nrOfChapters && chaptere[theNextIndex]->yCord > chaptere[theNextIndex - 1]->yCord && chaptere[theNextIndex]->isAvailableChapter)
						{
							levelChoserPlayer->indexOfChapterThatItIsOn++;
							levelChoserPlayer->canMove = false;
							levelChoserPlayer->isMovingLeftRight = 0;
							levelChoserPlayer->isMovingUpDown = 1;
						}
						else
							if(theNextIndex - 2 >= 0 && chaptere[theNextIndex - 2]->yCord > chaptere[theNextIndex - 1]->yCord && chaptere[theNextIndex - 2]->isAvailableChapter)
							{
								levelChoserPlayer->indexOfChapterThatItIsOn--;
								levelChoserPlayer->canMove = false;
								levelChoserPlayer->isMovingLeftRight = 0;
								levelChoserPlayer->isMovingUpDown = 1;
							}
					}
	}
	else
		///In caz ca jucatorul inca se misca ca sa ajunga la nivel
		if(!levelChoserPlayer->canMove && currentMenu->indexOfMenu == 5)
		{
			int indexOfWantedChapter = levelChoserPlayer->indexOfChapterThatItIsOn;

			walkingAnimation(chaptere[indexOfWantedChapter]->xCord,
							 chaptere[indexOfWantedChapter]->yCord);
		}
}


void BSLevelMenu::walkingAnimation(float xDestination, float yDestination)
{
	float moveSpeed = 0.43f;

	if(levelChoserPlayer->xCord < xDestination && levelChoserPlayer->isMovingLeftRight == 1)
	{
		levelChoserPlayer->xCord += moveSpeed;
		///Daca a trecut de nivel, se intoarce la el
		if(levelChoserPlayer->xCord > xDestination)
		{
			levelChoserPlayer->xCord = xDestination;
		}
	}
	else
		if(levelChoserPlayer->xCord > xDestination && levelChoserPlayer->isMovingLeftRight == -1)
		{
			levelChoserPlayer->xCord -= moveSpeed;
			///Daca a trecut de nivel, se intoarce la el
			if(levelChoserPlayer->xCord < xDestination)
			{
				levelChoserPlayer->xCord = xDestination;
			}
		}
		else
			if(levelChoserPlayer->yCord > yDestination && levelChoserPlayer->isMovingUpDown == -1)
			{
				levelChoserPlayer->yCord -= moveSpeed;
				///Daca a trecut de nivel, se intoarce la el
				if(levelChoserPlayer->yCord < yDestination)
				{
					levelChoserPlayer->yCord = yDestination;
				}
			}
			else
				if(levelChoserPlayer->yCord < yDestination && levelChoserPlayer->isMovingUpDown == 1)
				{
					levelChoserPlayer->yCord += moveSpeed;
					///Daca a trecut de nivel, se intoarce la el
					if(levelChoserPlayer->yCord > yDestination)
					{
						levelChoserPlayer->yCord = yDestination;
					}
				}
				else
					if(levelChoserPlayer->xCord == xDestination && levelChoserPlayer->yCord == yDestination)
					{
						levelChoserPlayer->canMove = true;
					}
}


void BSLevelMenu::chooseLevel(bool pressedEnter)
{
}
