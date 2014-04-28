#include "BSTexture.h"
#include "My_Values.h"

BSLevelMenu *theLevelMenuPlayer = new BSLevelMenu();
BSInputClass *theInputForMouse = new BSInputClass();

BSActionStuff *actionObj = new BSActionStuff();

GLuint theTextureToRender = 0;
BSColorRGBA theColorToRender = { 1.f, 1.f, 1.f, 1.f }; //transparent color

float colorOfDeadScene = 0;
int colorGoesUpOrDown = 0; ///Este -1 pentru cand culoarea de la deadScene a ajuns un negru complet, si 1, cand abia a murit si incepe sa
///innegreasca ecranul

float redSinForCoins = 0;

int nrEnding = 0;

glm::vec3 m_position, m_direction = glm::vec3(0, 0, -1), m_up = glm::vec3(0, 1, 0);


BSObstacles* addObstacle = new BSObstacles();


void BSTexture::loadTheLoadingImage()
{
	setTheValuesForShaders();

	m_position = glm::vec3(CameraPosition->xCord, CameraPosition->yCord, -CameraPosition->zCord);
//	m_direction = glm::vec3(0, 0, -1);
//	m_up = glm::vec3(0, 1, 0);
	LookAtMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);

	theLoadingImage->width = (float) SCREEN_WIDTH / cross_platform_scale + 1;
	theLoadingImage->height = (float) SCREEN_HEIGHT / cross_platform_scale + 1;
	BS_Renderer::set_vao_data(theLoadingImage->vao, theLoadingImage->vbo, theLoadingImage->ibo,
							   theLoadingImage->data, theLoadingImage->width, theLoadingImage->height);

	BS_Renderer::loadATexture("smb loading.png", theLoadingImage->texture);
	theLoadingImage->color.setColor(1, 1, 1, 1);

}

void BSTexture::renderTheLoadingImage(float alphaColor)
{
	_my_shader_program->set_uniform(BS_Available_Shaders::scale_size(), scaleSize);
	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 0);
	_my_shader_program->set_uniform(BS_Available_Shaders::circle_radius(), 0);

//	glUniform1f(ShaderScaleSize, scaleSize);
//	glUniform1f(ShaderCameraExtremeLeft, 0);
//	glUniform1f(ShaderCameraExtremeDown, 0);
//	glUniform1f(ShaderCameraExtremeUp, 0);
//	glUniform1f(ShaderCameraExtremeRight, 0);
//	glUniform1f(ShaderPlayerCoordinatesX, 0);
//	glUniform1f(ShaderPlayerCoordinatesY, 0);
//	glUniform1f(ShaderRotationAngle, 0);
//	glUniform1f(ShaderIsText, 0);
//	glUniform1f(ShaderIsCircle, 0);
//	glUniform1f(ShaderCircleRadius, 0);

	theLoadingImage->color.a = alphaColor;

	float scaleRatio = (float)(SCREEN_HEIGHT / 18.0f);

	BS_Renderer::renderSomeBody((float)SCREEN_WIDTH / scaleRatio / 2.0f + CameraPosition->xCord - CameraExtremeLeft,
								(float)SCREEN_HEIGHT / scaleRatio / 2.0f + CameraPosition->yCord - CameraExtremeDown,
								scaleSizeInitial, 0, 0 , 0,
								theLoadingImage->texture,
								theLoadingImage->color,
								theLoadingImage->vao, theLoadingImage->width, theLoadingImage->height, theLoadingImage->depth, false);
}





void BSTexture::initMenuButtons()
{
	///Adaug butoanele din meniuri
	std::ifstream citesteMeniurile("Menus/menus details.txt");
	int nrMeniuri, nrButtons;
	std::string namesOfMenus = "";
	citesteMeniurile >> nrMeniuri;

	///Set the Menu Transparent Block
	originalTransparentMenu->width = SCREEN_WIDTH / cross_platform_scale;
	originalTransparentMenu->height = SCREEN_HEIGHT / cross_platform_scale;
	originalTransparentMenu->depth = 0.0f;

	///Set the mouse texture
//	addObstacle->setTheVaoData(mouseTexture->vao, mouseTexture->vbo, mouseTexture->ibo, mouseTexture->data, Hero_size / 4.f, Hero_size / 4.f);
//
//	mouseTexture->color.setColor(1, 1, 1, 1);
//
//	mouseTexture->nameOfTexture = "Animations/mouseImage.png";
//	BS_Renderer::loadATexture(mouseTexture->nameOfTexture, mouseTexture->texture);
//	mouseTexture->width = Hero_size / 4.f;
//	mouseTexture->height = Hero_size / 4.f;
//	mouseTexture->xCord = hero->theBody->GetPosition().x;
//	mouseTexture->yCord = hero->theBody->GetPosition().y;

	float width, height, xCord, yCord, lastH;///, lastY;

	for(int i = 0 ; i < nrMeniuri ; i++)
	{
		BSMenuStructure *newMenu = new BSMenuStructure;
		citesteMeniurile >> newMenu->MenuIndex >> namesOfMenus >> nrButtons;
		newMenu->numberOfButtons = nrButtons;
		newMenu->indexOfMenu = i;
		///std::cout<<"The "<<namesOfMenus<<" is loading: index: "<<newMenu->MenuIndex<<"; number of buttons: "<<nrButtons<<'\n';

		///BS_Renderer::loadATexture("Menus/first.png", newMenu->textureOfMenu);

		for(int j = 0 ; j < nrButtons ; j++)
		{
			BSButtonStructure *newButton = new BSButtonStructure;
			citesteMeniurile >> width >> height >> newButton->NextMenuIndex;

			newButton->indexOfButton = j;
			newButton->value1 = 0;
			newButton->value2 = 0;
			newButton->width = width;
			newButton->height = height;
			width = width / 2;
			height = height / 2;
			newButton->color.setColor(1, 1, 1, 1);

			///Dau coordonatele primului buton
			if(!j)
			{
				xCord = SCREEN_WIDTH / 2 / cross_platform_scale;
				yCord = SCREEN_HEIGHT / 2 / cross_platform_scale;
				lastH = height * 2;
			}
			else
				///Dau coordonatele urmatoarelor butoane
			{
				///Setez pozitia urmatorului buton, in functie de cel de dinainte
				float theHeight = lastH + 0.3;
				yCord -= theHeight;
				lastH = height * 2;
			}

			newButton->xCord = xCord;
			newButton->yCord = yCord;

			BS_Renderer::loadATexture("Button images/New Buttons/" + namesOfMenus + "/The Unnamed Buttons/picture" + BStoString(j) + ".png", newButton->textureOfButtons[0]);
			BS_Renderer::loadATexture("Button images/New Buttons/" + namesOfMenus + "/The Unnamed Buttons/picture0" + BStoString(j) + ".png", newButton->textureOfButtons[1]);

			///Set the data for the menu buttons
			BS_Renderer::set_vao_data(newButton->vaoOfButtons[0], newButton->vbo[0], newButton->ibo[0], newButton->dataOfButtons[0], width * 2.0f, height * 2.0f);
			///Set the data for the menu buttons
			BS_Renderer::set_vao_data(newButton->vaoOfButtons[1], newButton->vbo[1], newButton->ibo[1], newButton->dataOfButtons[1], (width + 0.1f) * 2.0f, (height + 0.1f) * 2.0f);

			newMenu->listOfButtons.push_back(newButton);

			if(!i && !j)
			{
				currentMenu = newMenu;
				currentButton = newButton;
			}
		}
		listOfMenus.push_back(newMenu);
	}
}

void BSTexture::initMenuLevels()
{
	for(int i = 0 ; i < nrOfChapters; i++)
	{
		///Set the Menu Chapters
		BS_Renderer::set_vao_data(chaptere[i]->vao, chaptere[i]->vbo, chaptere[i]->ibo, chaptere[i]->data, chaptere[i]->width, chaptere[i]->height);

		for(int j = 0 ; j < chaptere[i]->nrOfLevels ; j++)
		{
			///Set the Menu Levels
			BS_Renderer::set_vao_data(chaptere[i]->levelFromChapter[j]->vaoBufferOfLevel, chaptere[i]->levelFromChapter[j]->vboBufferOfLevel,
									   chaptere[i]->levelFromChapter[j]->iboBufferOfLevel, chaptere[i]->levelFromChapter[j]->dataOfLevel,
									   chaptere[i]->levelFromChapter[j]->width, chaptere[i]->levelFromChapter[j]->height);
		}

		///Setez texturile de conexiune dintre nivele
		chaptere[i]->levelFromChapter[0]->widthConnectionLeftRightLevel = (chaptere[i]->levelFromChapter[0]->width / 1.4) * 2.0f;
		chaptere[i]->levelFromChapter[0]->heightConnectionLeftRightLevel = chaptere[i]->levelFromChapter[0]->height / 5;

		chaptere[i]->levelFromChapter[0]->widthConnectionUpDownLevel = chaptere[i]->levelFromChapter[0]->width / 5;
		chaptere[i]->levelFromChapter[0]->heightConnectionUpDownLevel = (chaptere[i]->levelFromChapter[0]->height / 1.4) * 2.0f;
		BS_Renderer::set_vao_data(chaptere[i]->levelFromChapter[0]->vaoBufferOfConnectionLeftRight, chaptere[i]->levelFromChapter[0]->vboBufferOfConnectionLeftRight,
								   chaptere[i]->levelFromChapter[0]->iboBufferOfConnectionLeftRight, chaptere[i]->levelFromChapter[0]->dataOfConnectionLeftRight,
								   (chaptere[i]->levelFromChapter[0]->width / 1.4) * 2.0f, chaptere[i]->levelFromChapter[0]->height / 5);

		BS_Renderer::set_vao_data(chaptere[i]->levelFromChapter[0]->vaoBufferOfConnectionUpDown, chaptere[i]->levelFromChapter[0]->vboBufferOfConnectionUpDown,
								   chaptere[i]->levelFromChapter[0]->iboBufferOfConnectionUpDown, chaptere[i]->levelFromChapter[0]->dataOfConnectionUpDown,
								   chaptere[i]->levelFromChapter[0]->width / 5, (chaptere[i]->levelFromChapter[0]->height / 1.4) * 2.0f);

		///Setez textura pentru Finished Level
		chaptere[i]->levelFromChapter[0]->widthFinishedLevel = chaptere[i]->levelFromChapter[0]->width;
		chaptere[i]->levelFromChapter[0]->heightFinishedLevel = chaptere[i]->levelFromChapter[0]->height;
		BS_Renderer::set_vao_data(chaptere[i]->levelFromChapter[0]->vaoBufferOfFinishedLevel, chaptere[i]->levelFromChapter[0]->vboBufferOfFinishedLevel,
								   chaptere[i]->levelFromChapter[0]->iboBufferOfFinishedLevel, chaptere[i]->levelFromChapter[0]->dataOfFinishedLevel,
								   chaptere[i]->levelFromChapter[0]->width, chaptere[i]->levelFromChapter[0]->height);

	}

	///Setez data si vao al animatiei cu care ma voi plimba prin nivele ca sa il aleg pe cel care vreau
	BS_Renderer::set_vao_data(levelChoserPlayer->vao, levelChoserPlayer->vbo, levelChoserPlayer->ibo,
							   levelChoserPlayer->data, levelChoserPlayer->width, levelChoserPlayer->height);

	///Setez texturile de conexiune dintre chaptere
	float sizeOfConnectionStringX = abs(chaptere[0]->xCord - chaptere[1]->xCord) - chaptere[0]->width / 2.0f;
	float sizeOfConnectionStringY = abs(chaptere[0]->yCord - chaptere[4]->yCord) - chaptere[0]->height / 2.0f;

	chaptere[0]->levelFromChapter[0]->widthConnectionLeftRightChapter = sizeOfConnectionStringX;
	chaptere[0]->levelFromChapter[0]->heightConnectionLeftRightChapter = chaptere[0]->height / 4.0f;

	chaptere[0]->levelFromChapter[0]->widthConnectionUpDownChapter = chaptere[0]->width / 4.0f;
	chaptere[0]->levelFromChapter[0]->heightConnectionUpDownChapter = sizeOfConnectionStringY;

	chaptere[0]->levelFromChapter[0]->widthFinishedChapter = chaptere[0]->levelFromChapter[0]->width;
	chaptere[0]->levelFromChapter[0]->heightFinishedChapter = chaptere[0]->levelFromChapter[0]->height;


	BS_Renderer::set_vao_data(chaptere[0]->levelFromChapter[0]->vaoBufferOfConnectionLeftRightChapter,
							   chaptere[0]->levelFromChapter[0]->vboBufferOfConnectionLeftRightChapter,
							   chaptere[0]->levelFromChapter[0]->iboBufferOfConnectionLeftRightChapter,
							   chaptere[0]->levelFromChapter[0]->dataOfConnectionLeftRightChapter,
							   sizeOfConnectionStringX, chaptere[0]->height / 4.0f);

	BS_Renderer::set_vao_data(chaptere[0]->levelFromChapter[0]->vaoBufferOfConnectionUpDownChapter,
							   chaptere[0]->levelFromChapter[0]->vboBufferOfConnectionUpDownChapter,
							   chaptere[0]->levelFromChapter[0]->iboBufferOfConnectionUpDownChapter,
							   chaptere[0]->levelFromChapter[0]->dataOfConnectionUpDownChapter,
							   chaptere[0]->width / 4.0f, sizeOfConnectionStringY);

	BS_Renderer::set_vao_data(chaptere[0]->levelFromChapter[0]->vaoBufferOfFinishedLevelChapter,
							   chaptere[0]->levelFromChapter[0]->vboBufferOfFinishedLevelChapter,
							   chaptere[0]->levelFromChapter[0]->iboBufferOfFinishedLevelChapter,
							   chaptere[0]->levelFromChapter[0]->dataOfFinishedLevelChapter,
							   chaptere[0]->width, chaptere[0]->height);
}

BSTexture::BSTexture()
{
}

void BSTexture::setTheValuesForShaders()
{
	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 0);
//	glUniform1f(ShaderPlayerCoordinatesX, 0);
//	glUniform1f(ShaderPlayerCoordinatesY, 0);
//	glUniform1f(ShaderRotationAngle, 0);
//	glUniform1f(ShaderIsText, 0);
//	glUniform1f(ShaderIsCircle, 0);
}


void rotates(float amount, glm::vec3& axis)
{
	///glm::rotate()
	float m_speed = 1.0f;
//    m_direction = glm::rotate(amount * m_speed, axis) * m_direction;
}
void BSTexture::renderVboAndVao()
{
	setCameraPosition();

	m_position = glm::vec3(CameraPosition->xCord, CameraPosition->yCord, -CameraPosition->zCord);
	LookAtMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);

	if(showBackgr)
	{
		renderBackgroundNo1();
	}

	renderTheObstacles();
	renderDustBodies();
	renderCoins();
	renderFans();
	renderWormHoles();
	renderAllAnimations();
	renderMessages();

	if(DarkLevels)
	{
		renderDarkLevel();
	}

	if(useSnow)
	{
		renderSnowFlakes();
	}
	else
		if(useRain)
		{
			renderRainDrops();
		}
		else
			if(useAssassinsCreed)
			{
				///renderAssassinsCreed();
			}


//renderMouseTexture();

	if(!DarkLevels)
	{
		BSColorRGBA cul = {0, 0, 0, 1};
		renderMyFont(BStoString(coinsTotalNo + coinsThisLevel), originalCoin->xCord + CameraPosition->xCord - CameraExtremeLeft + 1,
					 originalCoin->yCord + CameraPosition->yCord - CameraExtremeDown, 1, 1, cul, false);
	}
	else
	{
		BSColorRGBA cul = {0.8f, 0.8f, 0.8f, 1};
		renderMyFont(BStoString(coinsTotalNo + coinsThisLevel), originalCoin->xCord + CameraPosition->xCord - CameraExtremeLeft + 1,
					 originalCoin->yCord + CameraPosition->yCord - CameraExtremeDown, 1, 1, cul, false);
	}
	renderTheCoin();
}







void BSTexture::buildRainDrops()
{
	originalRainDrop->width = 1;
	originalRainDrop->height = 1;
	originalRainDrop->depth = 0;
	BS_Renderer::set_vao_data(originalRainDrop->vao, originalRainDrop->vbo, originalRainDrop->ibo, originalRainDrop->data, 1, 1);

	float maximX = 50;
	float minimX = -3.0f;

	float maximY = 40;
	float minimY = 0.0f;

	float maximZ = 1;///-scaleSizeInitial + 1;
	float minimZ = -0.5f;///2 * scaleSizeInitial;

	float posX, posY, posZ, xSpeed, ySpeed;///, xSpeedMin, xSpeedMax, ySpeedMin, ySpeedMax;
//			int leftOrRight = 0;

	for(int i = 0 ; i < 1000; i++)
	{
		posX = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximX - minimX + 1) + minimX;
		posY = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximY - minimY + 1) + minimY;
		posZ = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximZ - minimZ + 1) + minimZ;

		xSpeed = 0.07f;
		ySpeed = -0.25f;

		addARainDrop(posX, posY, posZ, xSpeed, ySpeed);
	}
}

void BSTexture::resetRainDrops()
{
	for(int i = 0; i < listOfRainDrops.size(); i++)
	{
		float maximX = 50;
		float minimX = -3.0f;

		float maximY = 40;
		float minimY = 0.0f;

		listOfRainDrops[i]->xCord = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximX - minimX + 1) + minimX;
		listOfRainDrops[i]->yCord = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximY - minimY + 1) + minimY;
	}
}

void BSTexture::addARainDrop(float x, float y, float z, float xPower, float yPower)
{
	BSSnowFlakes *newDrop = new BSSnowFlakes;
	newDrop->color.setColor(0, 0, 255, 0.2f);

	if(z > -3)
	{
		newDrop->width = 0.02f;
	}
	else
	{
		newDrop->width = 0.04f;
	}
	newDrop->height = 0.4f;
	newDrop->depth = 0;
	newDrop->xCord = x;
	newDrop->yCord = y;
	newDrop->zCord = z;
	newDrop->xPower = xPower;
	newDrop->yPower = yPower;
	newDrop->xInit = x;
	newDrop->yInit = y;
	listOfRainDrops.push_back(newDrop);
}

void BSTexture::renderRainDrops()
{
//    float maximX = 50;
//    float minimX = -3.0f;

	float maximY = 40;
//    float minimY = 0.0f;

//    float posX, posY, posZ;
	BSColorRGBA color = (listOfRainDrops.front())->color;
	_my_shader_program->set_uniform(BS_Available_Shaders::texture_color(), color.r, color.g, color.b, color.a );
	//_my_shader_program->set_uniform(BS_Available_Shaders::texture_color(), color.r, color.g, color.b, color.a );
	glBindVertexArray(originalRainDrop->vao);
	glBindTexture(GL_TEXTURE_2D, blankTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR);

	m_position = glm::vec3(CameraPosition->xCord, CameraPosition->yCord, -CameraPosition->zCord);
	LookAtMatrix = LookAtMatrix * glm::rotate( 10.0f, 0.0f, 0.0f, 1.0f );

	for(int i = 0; i < listOfRainDrops.size(); i++)
	{
		listOfRainDrops[i]->xCord += listOfRainDrops[i]->xPower;
		listOfRainDrops[i]->yCord += listOfRainDrops[i]->yPower;

		renderChuncksOfObjects(listOfRainDrops[i]->xCord, listOfRainDrops[i]->yCord, listOfRainDrops[i]->zCord, listOfRainDrops[i]->width, listOfRainDrops[i]->height);

		if(listOfRainDrops[i]->yCord < 0)
		{
//			posX = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximX - minimX + 1) + minimX;
			listOfRainDrops[i]->yCord = maximY;
			listOfRainDrops[i]->xCord = listOfRainDrops[i]->xInit;
		}
	}
	m_position = glm::vec3(CameraPosition->xCord, CameraPosition->yCord, -CameraPosition->zCord);
	LookAtMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);
}






void BSTexture::renderCoins()
{
	if(redSinForCoins < 180)
	{
		redSinForCoins += 2.0f;
	}
	else
	{
		redSinForCoins = 0;
	}
	for(int it = 0; it < listOfCoins.size(); it++)
	{
		if(listOfCoins[it]->available)
		{
			///Daca e in limite
			if(( (hero->theBody->GetPosition().x + hero->width / 2.f >= listOfCoins[it]->xCord - listOfCoins[it]->width / 2.f &&
					hero->theBody->GetPosition().x + hero->width / 2.f <= listOfCoins[it]->xCord + listOfCoins[it]->width / 2.f) ||
					(hero->theBody->GetPosition().x - hero->width / 2.f <= listOfCoins[it]->xCord + listOfCoins[it]->width / 2.f &&
					 hero->theBody->GetPosition().x - hero->width / 2.f >= listOfCoins[it]->xCord - listOfCoins[it]->width / 2.f) )
					&&
					((hero->theBody->GetPosition().y + hero->height / 2.f >= listOfCoins[it]->yCord - listOfCoins[it]->height / 2.f &&
					  hero->theBody->GetPosition().y + hero->height / 2.f <= listOfCoins[it]->yCord + listOfCoins[it]->height / 2.f) ||
					 (hero->theBody->GetPosition().y - hero->height / 2.f <= listOfCoins[it]->yCord + listOfCoins[it]->height / 2.f &&
					  hero->theBody->GetPosition().y - hero->height / 2.f >= listOfCoins[it]->yCord - listOfCoins[it]->height / 2.f)))
			{
				listOfCoins[it]->available = false;
				listOfCoins[it]->sinValue = 90;
				listOfCoins[it]->sinValue2 = 0;
				coinsThisLevel++;
				_my_sound_device->play_sound(BS_Available_Sounds::coin());
			}
		}
		else
		{
			///Daca nu e transparent de tot
			if(listOfCoins[it]->sinValue > 0)
			{
				listOfCoins[it]->sinValue -= 4.5f;
				listOfCoins[it]->color.a = sin(toRadians(listOfCoins[it]->sinValue));
				listOfCoins[it]->sinValue2 += 0.5f;
				listOfCoins[it]->yCord += sin(toRadians(listOfCoins[it]->sinValue2));

				///Daca e transparent de tot
				if(listOfCoins[it]->sinValue <= 0)
				{
					listOfCoins[it]->deleteIt = true;
				}
			}
		}
		///Daca il poate desena
		if(!listOfCoins[it]->deleteIt)
		{
			listOfCoins[it]->color.r = 1 + sin(toRadians(redSinForCoins));
			listOfCoins[it]->draw(originalCoin->texture, originalCoin->vao);
		}
	}
}

void BSTexture::renderFans()
{
	for(int it = 0; it < listOfFans.size(); it++)
	{
		if(listOfFans[it]->powerY)
		{
			///Daca e in limite
			if(( (hero->theBody->GetPosition().x + hero->width / 2.f >= listOfFans[it]->xCord - listOfFans[it]->width / 2.f &&
					hero->theBody->GetPosition().x + hero->width / 2.f <= listOfFans[it]->xCord + listOfFans[it]->width / 2.f) ||
					(hero->theBody->GetPosition().x - hero->width / 2.f <= listOfFans[it]->xCord + listOfFans[it]->width / 2.f &&
					 hero->theBody->GetPosition().x - hero->width / 2.f >= listOfFans[it]->xCord - listOfFans[it]->width / 2.f) ) &&
					(hero->theBody->GetPosition().y - hero->height / 2.f <= listOfFans[it]->yCord + listOfFans[it]->lenthOfPower ) )
			{
				hero->theBody->ApplyForce(b2Vec2(0, listOfFans[it]->powerY), hero->theBody->GetWorldCenter());
			}
		}
		else
			if(listOfFans[it]->powerX)
			{
				///Daca e in limite
				if(( (hero->theBody->GetPosition().x + hero->width / 2.f >= listOfFans[it]->xCord - listOfFans[it]->width / 2.f &&
						hero->theBody->GetPosition().x + hero->width / 2.f <= listOfFans[it]->xCord + listOfFans[it]->width / 2.f) ||
						(hero->theBody->GetPosition().x - hero->width / 2.f <= listOfFans[it]->xCord + listOfFans[it]->width / 2.f &&
						 hero->theBody->GetPosition().x - hero->width / 2.f >= listOfFans[it]->xCord - listOfFans[it]->width / 2.f) ) &&
						(hero->theBody->GetPosition().y - hero->height / 2.f <= listOfFans[it]->yCord + listOfFans[it]->lenthOfPower ) )
				{
					hero->theBody->ApplyForce(b2Vec2(listOfFans[it]->powerX, 0), hero->theBody->GetWorldCenter());
				}
			}

		listOfFans[it]->draw(originalFan->texture, originalFan->vao);

		if(listOfFans[it]->powerY)
		{
			_my_shader_program->set_uniform(BS_Available_Shaders::texture_color(), 0, 0, 0, 1 );

			glBindVertexArray(originalFanParticleY->vao);
			glBindTexture(GL_TEXTURE_2D, originalFan->texture);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR);

			for(int i = 0; i < listOfFans[it]->particles.size(); i++)
			{
				if(!isPaused)
				{
					listOfFans[it]->particles[i]->yCord += listOfFans[it]->particles[i]->value2;
					if(listOfFans[it]->particles[i]->yCord > listOfFans[it]->yMax + 4)
					{
						listOfFans[it]->particles[i]->yCord = listOfFans[it]->yMin;
					}
				}
				renderChuncksOfObjects(listOfFans[it]->particles[i]->xCord, listOfFans[it]->particles[i]->yCord, listOfFans[it]->particles[i]->zCord, 1, 1);
			}
		}
		else
			if(listOfFans[it]->powerX)
			{
				_my_shader_program->set_uniform(BS_Available_Shaders::texture_color(), 0, 0, 0, 1 );

				glBindVertexArray(originalFanParticleX->vao);
				glBindTexture(GL_TEXTURE_2D, originalFan->texture);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR);

				for(int i = 0; i < listOfFans[it]->particles.size(); i++)
				{
					listOfFans[it]->particles[i]->xCord += listOfFans[it]->particles[i]->value1;
					if(listOfFans[it]->particles[i]->xCord > listOfFans[it]->xMax + 4)
					{
						listOfFans[it]->particles[i]->xCord = listOfFans[it]->xMin;
					}
					renderChuncksOfObjects(listOfFans[it]->particles[i]->xCord, listOfFans[it]->particles[i]->yCord, listOfFans[it]->particles[i]->zCord, 1, 1);
				}
			}
	}
}

void BSTexture::renderDarkLevel()
{
	_my_shader_program->set_uniform(BS_Available_Shaders::is_dark(), 1);
	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 1);
	_my_shader_program->set_uniform(BS_Available_Shaders::circle_radius(), Hero_size / 3.f);
//	glUniform1f(ShaderDark, 1);
//	glUniform1f(ShaderIsCircle, 1);
//	glUniform1f(ShaderCircleRadius, Hero_size / 3.f);

//	float xCordL, xCordR, xCordU, xCordD, yCordL, yCordR, yCordU, yCordD;

//	xCordL = hero->theBody->GetPosition().x - SCREEN_WIDTH / 32.0f + Hero_size * 6.f;
//	xCordD = hero->theBody->GetPosition().x;
//	xCordU = hero->theBody->GetPosition().x;
//	xCordR = hero->theBody->GetPosition().x + SCREEN_WIDTH / 32.0f - Hero_size * 6.f;
//
//	yCordL = hero->theBody->GetPosition().y;
//	yCordD = hero->theBody->GetPosition().y - SCREEN_HEIGHT / 32.0f + Hero_size * 6.f;
//	yCordU = hero->theBody->GetPosition().y + SCREEN_HEIGHT / 32.0f - Hero_size * 6.f;
//	yCordR = hero->theBody->GetPosition().y;

	BSColorRGBA mainColor = {0.f, 0.f, 0.f, 1.f};
	BS_Renderer::renderSomeBody(hero->theBody->GetPosition().x, hero->theBody->GetPosition().y, hero->zCord, 0, 0, 0,
								blankTexture, mainColor, originalDarkMenu->vao, originalDarkMenu->width, originalDarkMenu->height, originalDarkMenu->depth, false);
	BS_Renderer::renderSomeBody(hero->theBody->GetPosition().x, hero->theBody->GetPosition().y, hero->zCord, 0, 0, 0,
								blankTexture, mainColor, originalDarkMenu->vao, originalDarkMenu->width, originalDarkMenu->height, originalDarkMenu->depth, false);

	_my_shader_program->set_uniform(BS_Available_Shaders::is_dark(), 0);
	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 0);
//	glUniform1f(ShaderDark, 0);
//	glUniform1f(ShaderIsCircle, 0);
}


void BSTexture::renderTheSquareObstacles()
{
	for(int i = 0; i < ListOfSquareBodies.size(); i++)
	{
		if(ListOfSquareBodies[i]->ObstacleCanKillHero)
		{
			ListOfSquareBodies[i]->color.setColor(2, 0.3, 0, 1);
		}
		else
			if(ListOfSquareBodies[i]->ObstacleFinishesLevel)
			{
				ListOfSquareBodies[i]->color.setColor(0, 2, 0, 1);
			}
			else
			{
				ListOfSquareBodies[i]->color.setColor(1, 1, 1, 1);
			}

		checkBodyScript(ListOfSquareBodies[i], NULL);

		///Daca are textura sau culoare, il desenez
		if((ListOfSquareBodies[i]->color.r != 1.0 || ListOfSquareBodies[i]->color.g != 1.0 || ListOfSquareBodies[i]->color.b != 1.0 || ListOfSquareBodies[i]->color.a != 1.0)
				|| ListOfSquareBodies[i]->texture)
		{
			ListOfSquareBodies[i]->xCord = ListOfSquareBodies[i]->theBody->GetPosition().x;
			ListOfSquareBodies[i]->yCord = ListOfSquareBodies[i]->theBody->GetPosition().y;
			ListOfSquareBodies[i]->draw();
//			BS_Renderer::renderSomeBody((*i)->theBody->GetPosition().x, (*i)->theBody->GetPosition().y,
//                                    (*i)->zCord, 0, 0, 0,
//									(*i)->texture, (*i)->color, (*i)->vao, (*i)->width, (*i)->height, (*i)->depth);
		}
	}
}

void BSTexture::renderTheCircleObstacles()
{
	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 1);
//	glUniform1f(ShaderIsCircle, 1);

	for(int i = 0; i < ListOfRoundBodies.size(); i++)
	{
		float theAngle = ListOfRoundBodies[i]->theBody->GetAngle();
		if(ListOfRoundBodies[i]->isRotating && !isPaused && !isTeleporting)
		{
			if((ListOfRoundBodies[i]->theBody->GetAngle() < 0 && ListOfRoundBodies[i]->theBody->GetAngle() <= -2 * 180) || (ListOfRoundBodies[i]->theBody->GetAngle() > 0 && ListOfRoundBodies[i]->theBody->GetAngle() >= 2 * 180))
			{
				theAngle = 0;

				if(ListOfRoundBodies[i]->theBody->GetAngle() < 0)
				{
					theAngle *= -1;
				}
			}

			theAngle -= 5.1 * (60 / theTimeHowFast);
			ListOfRoundBodies[i]->theBody->SetTransform(b2Vec2(ListOfRoundBodies[i]->theBody->GetPosition().x, ListOfRoundBodies[i]->theBody->GetPosition().y), theAngle);
		}
		_my_shader_program->set_uniform(BS_Available_Shaders::circle_radius(), ListOfRoundBodies[i]->width);
//		glUniform1f(ShaderCircleRadius, ListOfRoundBodies[i]->width);

		if(ListOfRoundBodies[i]->ObstacleCanKillHero)
		{
			ListOfRoundBodies[i]->color.setColor(120, 0.3f, 0, 1);
		}
		else
			if(ListOfRoundBodies[i]->ObstacleFinishesLevel)
			{
				ListOfRoundBodies[i]->color.setColor(0, 20, 0, 1);
			}

		checkBodyScript(NULL, ListOfRoundBodies[i]);

		///Daca are textura sau culoare, il desenez
		if( (ListOfRoundBodies[i]->color.r != 1.0 || ListOfRoundBodies[i]->color.g != 1.0 || ListOfRoundBodies[i]->color.b != 1.0 || ListOfRoundBodies[i]->color.a != 1.0)
				|| ListOfRoundBodies[i]->texture)
		{
			ListOfRoundBodies[i]->xCord = ListOfRoundBodies[i]->theBody->GetPosition().x;
			ListOfRoundBodies[i]->yCord = ListOfRoundBodies[i]->theBody->GetPosition().y;
			ListOfRoundBodies[i]->xAngle = theAngle;
			ListOfRoundBodies[i]->draw();
		}
	}
	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 0);
//	glUniform1f(ShaderIsCircle, 0);
}


void BSTexture::checkBodyScript(BSTheSquareBodies *square, BSTheRoundBodies *round)
{
	BSBodyScript *script = NULL;
	float xCord = 0, yCord = 0;
	float xSpeed = 0, ySpeed = 0;
	float slowSpeed = 6;

	if(square)
	{
		script = square->script;
		xCord = square->theBody->GetPosition().x;
		yCord = square->theBody->GetPosition().y;
	}
	else
		if(round)
		{
			script = round->script;
			xCord = round->theBody->GetPosition().x;
			yCord = round->theBody->GetPosition().y;
		}

	if(script)
	{
		float maxRight = (float) (script->xInitial + (script->xLength / 2));
		float maxLeft = (float) (script->xInitial - (script->xLength / 2));
		float maxUp = (float) (script->yInitial + (script->yLength / 2));
		float maxDown = (float) (script->yInitial - (script->yLength / 2));

		if(script->value2 >= 360)
		{
			script->value2 = script->value2 - 360;
		}

		if(script->xPower)
		{
			///if he is moving right and didn't reach the border
			if(xCord < maxRight && script->value1 > 0)
			{
				///daca mai are putin si ajunge la border, il incetinesc
				if(xCord > maxRight - 1)
				{
					///ii las viteza mica (nu il opresc complet)
					if(script->value2 < 170)
					{
						script->value2 += slowSpeed;
					}
				}
				else
					///daca tocmai a cotit la dreapta
					if(xCord < maxRight - 1)
					{
						///ii las viteza mica (nu il opresc complet)
						if(script->value2 < 90)
						{
							script->value2 += slowSpeed;
						}
					}
					///daca nu trebuie sa incetineasca sau sa accelereze
					else
					{
						if(sin(toRadians(script->value2)) > 0)
						{
							script->value2 = 90;
						}
						else
							if(sin(toRadians(script->value2)) < 0)
							{
								script->value2 = 270;
							}
					}
			}
			else
				///if he is moving left and didn't reach the border
				if(xCord > maxLeft && script->value1 < 0)
				{
					///daca tocmai a cotit la stanga
					if(xCord > maxRight - 1)
					{
						///ii las viteza mica (nu il opresc complet)
						if(script->value2 < 270)
						{
							script->value2 += slowSpeed;
						}
					}
					else
						///daca mai are putin si ajunge la border, il incetinesc
						if(xCord < maxLeft + 1)
						{
							///ii las viteza mica (nu il opresc complet)
							if(script->value2 < 350)
							{
								script->value2 += slowSpeed;
							}
						}
						///daca nu trebuie sa incetineasca sau sa accelereze
						else
						{
							if(sin(toRadians(script->value2)) > 0)
							{
								script->value2 = 90;
							}
							else
								if(sin(toRadians(script->value2)) < 0)
								{
									script->value2 = 270;
								}
						}
				}
				else
					///if he got out of borders
					if(xCord >= maxRight)
					{
						script->value1 = -1;
						script->value2 = 190;
					}
					else
						if(xCord <= maxLeft)
						{
							script->value1 = 1;
							script->value2 = 10;
						}

			xSpeed = script->xPower * sin(toRadians(script->value2));
		}
		else
			if(script->yPower)
			{
				///if he is moving right and didn't reach the border
				if(yCord < maxUp && script->value1 > 0)
				{
					///daca mai are putin si ajunge la border, il incetinesc
					if(yCord > maxUp - 1)
					{
						///ii las viteza mica (nu il opresc complet)
						if(script->value2 < 170)
						{
							script->value2 += slowSpeed;
						}
					}
					else
						///daca tocmai a cotit la dreapta
						if(yCord < maxUp - 1)
						{
							///ii las viteza mica (nu il opresc complet)
							if(script->value2 < 90)
							{
								script->value2 += slowSpeed;
							}
						}
						///daca nu trebuie sa incetineasca sau sa accelereze
						else
						{
							if(sin(toRadians(script->value2)) > 0)
							{
								script->value2 = 90;
							}
							else
								if(sin(toRadians(script->value2)) < 0)
								{
									script->value2 = 270;
								}
						}
				}
				else
					///if he is moving left and didn't reach the border
					if(yCord > maxDown && script->value1 < 0)
					{
						///daca tocmai a cotit la stanga
						if(yCord > maxUp - 1)
						{
							///ii las viteza mica (nu il opresc complet)
							if(script->value2 < 270)
							{
								script->value2 += slowSpeed;
							}
						}
						else
							///daca mai are putin si ajunge la border, il incetinesc
							if(yCord < maxDown + 1)
							{
								///ii las viteza mica (nu il opresc complet)
								if(script->value2 < 350)
								{
									script->value2 += slowSpeed;
								}
							}
							///daca nu trebuie sa incetineasca sau sa accelereze
							else
							{
								if(sin(toRadians(script->value2)) > 0)
								{
									script->value2 = 90;
								}
								else
									if(sin(toRadians(script->value2)) < 0)
									{
										script->value2 = 270;
									}
							}
					}
					else
						///if he got out of borders
						if(yCord >= maxUp)
						{
							script->value1 = -1;
							script->value2 = 190;
						}
						else
							if(yCord <= maxDown)
							{
								script->value1 = 1;
								script->value2 = 10;
							}

				ySpeed = script->yPower * sin(toRadians(script->value2));
			}
	}

	if(square)
	{
		square->theBody->SetLinearVelocity(b2Vec2(xSpeed, ySpeed));
	}
	else
		if(round)
		{
			round->theBody->SetLinearVelocity(b2Vec2(xSpeed, ySpeed));
		}
}

void BSTexture::renderOnlyTheTextures()
{
// 	BS_Renderer::renderSomeBody(mouseTexture->xCord + CameraPosition->xCord - CameraExtremeLeft,
//                            mouseTexture->yCord + CameraPosition->yCord - CameraExtremeDown,
//                            scaleSizeInitial, 0, 0, 0,
//							mouseTexture->texture, mouseTexture->color, mouseTexture->vao,
//							mouseTexture->width, mouseTexture->height, mouseTexture->depth);

	for(int i = 0; i < ListOfSquareTextures.size(); i++)
	{
		ListOfSquareTextures[i]->draw();
	}

	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 1);
//	glUniform1f(ShaderIsCircle, 1);
//	glUniform1f(ShaderRotated, 1);

	for(int i = 0; i < ListOfRoundTextures.size(); i++)
	{
		ListOfRoundTextures[i]->draw();
	}

	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 0);
//	glUniform1f(ShaderIsCircle, 0);
//	glUniform1f(ShaderRotated, 0);
}





void BSTexture::renderTheObstacles()
{
	///Render the round bodies
	renderTheCircleObstacles();
	///Render the square bodies
	renderTheSquareObstacles();
	renderOnlyTheTextures();

	///Render Particles
	theColorToRender = { 0.f, 0.f, 0.0f, 1.f };

	for(int i = 0; i < ListOfParticles.size(); i++)
	{
		if(ListOfParticles[i]->isDestroyed == false)
		{

			ListOfParticles[i]->theBodyOfParticle->SetTransform(b2Vec2(ListOfParticles[i]->theBodyOfParticle->GetPosition().x, ListOfParticles[i]->theBodyOfParticle->GetPosition().y),
					ListOfParticles[i]->theBodyOfParticle->GetAngle() * 1.2);

			if(ListOfParticles[i]->BumTime < SDL_GetTicks() + 50)
			{
				ListOfParticles[i]->color.setColor(ListOfParticles[i]->color.r + 0.15f, ListOfParticles[i]->color.g + 0.15f, 0, 1);
			}
			ListOfParticles[i]->xCord = ListOfParticles[i]->theBodyOfParticle->GetPosition().x;
			ListOfParticles[i]->yCord = ListOfParticles[i]->theBodyOfParticle->GetPosition().y;
			ListOfParticles[i]->width = originalParticle->width;
			ListOfParticles[i]->height = originalParticle->height;
			ListOfParticles[i]->draw(blankTexture, originalParticle->vao);

			if(ListOfParticles[i]->BumTime <= SDL_GetTicks())
			{
				ListOfParticles[i]->isDestroyed = true;
				ListOfParticles[i]->theBodyOfParticle->SetTransform(b2Vec2(ListOfParticles[i]->theBodyOfParticle->GetPosition().x,
						ListOfParticles[i]->theBodyOfParticle->GetPosition().y), 0);
			}
		}
	}
}

void BSTexture::setCameraPosition()
{
	if(hero->theBody->GetPosition().x <= CameraExtremeLeft)
	{
		CameraPosition->xCord = CameraExtremeLeft;
	}
	else
		if(hero->theBody->GetPosition().x > CameraExtremeLeft && hero->theBody->GetPosition().x < CameraExtremeRight)
		{
			CameraPosition->xCord = hero->theBody->GetPosition().x;
		}
		else
			if(hero->theBody->GetPosition().x >= CameraExtremeRight)
			{
				CameraPosition->xCord = CameraExtremeRight;
			}

	if(hero->theBody->GetPosition().y <= CameraExtremeDown)
	{
		CameraPosition->yCord = CameraExtremeDown;
	}
	else
		if(hero->theBody->GetPosition().y > CameraExtremeDown && hero->theBody->GetPosition().y < CameraExtremeUp)
		{
			CameraPosition->yCord = hero->theBody->GetPosition().y;
		}
		else
			if(hero->theBody->GetPosition().y >= CameraExtremeUp)
			{
				CameraPosition->yCord = CameraExtremeUp;
			}
	CameraPosition->xCord = hero->theBody->GetPosition().x;
	CameraPosition->yCord = hero->theBody->GetPosition().y;
}

void BSTexture::renderDeadSceneChange()
{
	if(deadScene->SinValueColorOfTheScene < 90 && deadScene->colorGoesUpOrDown == 1)
	{
		deadScene->SinValueColorOfTheScene += 0.9f;
	}
	else
		if(deadScene->SinValueColorOfTheScene >= 90 && deadScene->colorGoesUpOrDown == 1)
		{
			_my_sound_device->play_sound(BS_Available_Sounds::melody_beginning_game());

			for(int i = 0; i < listOfCoins.size(); i++)
			{
				if(listOfCoins[i]->alreadyHadIt == false)
				{
					coinsThisLevel = 0;
					listOfCoins[i]->available = true;
					listOfCoins[i]->deleteIt = false;
					listOfCoins[i]->sinValue = 90;
					listOfCoins[i]->sinValue2 = 0;
					listOfCoins[i]->color.a = 1.0f;
					listOfCoins[i]->xCord = listOfCoins[i]->xInit;
					listOfCoins[i]->yCord = listOfCoins[i]->yInit;
				}
			}

			deadScene->colorGoesUpOrDown = -1;
			deadScene->SinValueColorOfTheScene -= 1.9f;
			hero->theBody->SetTransform(b2Vec2(HeroInitialX, HeroInitialY), 0);
			hero->heroIsDead = false;
			hero->texture = playerTexture;
			canJumpHorizontally = false;
			canJumpVertically = false;
			isOnGround = false;
			isInAir = true;
			isOnLeftWall = false;
			isOnRightWall = false;

			hero->theBody->SetTransform(b2Vec2(HeroInitialX, HeroInitialY), 0);
			world->Step((float) 1 / theTimeHowFast , 8, 3);
			world->ClearForces();
			actionObj->heroTouchesObjects();

			hero->heroIsBerserk1 = false;
			hero->heroIsBerserk2 = false;
			hero->heroIsBerserk3 = false;

			hero->animationIndexCurrentAnimation = hero->animationIndexPlayerStand;
			hero->IsAtStepOfAnimation1 = true;
			hero->IsAtStepOfAnimation2 = false;
			hero->IsAtStepOfAnimation3 = false;

			std::ofstream coinsAndDeaths("coinsAndDeaths");
			coinsAndDeaths << coinsTotalNo << ' ' << deathsTotalNo;
		}
		else
			if(deadScene->SinValueColorOfTheScene > 0 && deadScene->colorGoesUpOrDown == -1)
			{
				if(deadScene->SinValueColorOfTheScene < 60 && hero->theBody->GetLinearVelocity().y == 0)
				{
					hero->theBody->SetLinearVelocity(b2Vec2(0, 0.1));
					hero->theBody->ApplyLinearImpulse(b2Vec2(0, 0.1), hero->theBody->GetWorldCenter());
				}

				deadScene->SinValueColorOfTheScene -= 0.9f;
			}
			else
			{
				deadScene->SinValueColorOfTheScene = 0.f;
				deadScene->colorGoesUpOrDown = 0;
			}

	deadScene->color.a = sin(toRadians(deadScene->SinValueColorOfTheScene));
	deadScene->xCord = CameraPosition->xCord - CameraExtremeLeft;
	deadScene->yCord = CameraPosition->yCord - CameraExtremeDown;
	deadScene->zCord = scaleSizeInitial;

	deadScene->draw();
}




void BSTexture::renderNextLevelSceneChange()
{
	if(deadScene->CosValueColorOfTheScene < 90 && deadScene->colorGoesUpOrDown == 1)
	{
		deadScene->CosValueColorOfTheScene += 0.9f;
	}
	else
		if(deadScene->CosValueColorOfTheScene >= 90 && deadScene->colorGoesUpOrDown == 1)
		{
			consoleSalveazaHarta();
			coinsTotalNo += coinsThisLevel;
			coinsThisLevel = 0;
			std::ofstream fout("coinsAndDeaths");
			fout << coinsTotalNo << ' ' << deathsTotalNo;

			_my_sound_device->play_sound(BS_Available_Sounds::melody_beginning_game());

			deadScene->colorGoesUpOrDown = -1;
			deadScene->CosValueColorOfTheScene -= 1.9f;

			std::cout << "next level" << '\n';
			///Vad daca a terminat nivelul
			levelChoserPlayer->justFinished = false;
			levelChoserPlayer->indexOfLevelThatItIsOn++;

			addObstacle->destroyLevel();
			addObstacle->incarcaHarta("Menu Of Levels/Chapter" + BStoString(levelChoserPlayer->indexOfChapterThatItIsOn + 1) + "/The Levels objects/level" +
									  BStoString(levelChoserPlayer->indexOfLevelThatItIsOn + 1) + ".xml");
			levelChoserPlayer->xCord = chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter[levelChoserPlayer->indexOfLevelThatItIsOn]->xCord;
			levelChoserPlayer->yCord = chaptere[levelChoserPlayer->indexOfChapterThatItIsOn]->levelFromChapter[levelChoserPlayer->indexOfLevelThatItIsOn]->yCord;

			hero->theBody->SetTransform(b2Vec2(HeroInitialX, HeroInitialY), 0);
		}
		else
			if(deadScene->CosValueColorOfTheScene > 0 && deadScene->colorGoesUpOrDown == -1)
			{
				if(deadScene->CosValueColorOfTheScene < 60 && hero->theBody->GetLinearVelocity().y == 0)
				{
					hero->theBody->SetLinearVelocity(b2Vec2(0, 0.1));
					hero->theBody->ApplyLinearImpulse(b2Vec2(0, 0.1), hero->theBody->GetWorldCenter());
				}
				deadScene->CosValueColorOfTheScene -= 0.9f;
			}
			else
			{
				deadScene->CosValueColorOfTheScene = 0.f;
				deadScene->colorGoesUpOrDown = 0;
			}

	deadScene->color.a = sin(toRadians(deadScene->CosValueColorOfTheScene));
	deadScene->xCord = CameraPosition->xCord - CameraExtremeLeft;
	deadScene->yCord = CameraPosition->yCord - CameraExtremeDown;
	deadScene->zCord = scaleSizeInitial;

	deadScene->draw();
}



void BSTexture::renderLevelSceneChangeFromMenuLevels()
{
	if(deadScene->CosValueColorOfTheScene < 90 && deadScene->colorGoesUpOrDown == 1)
	{
		deadScene->CosValueColorOfTheScene += 0.9f;
	}
	else
		if(deadScene->CosValueColorOfTheScene >= 90 && deadScene->colorGoesUpOrDown == 1)
		{
			_my_sound_device->play_sound(BS_Available_Sounds::melody_beginning_game());

			deadScene->colorGoesUpOrDown = -1;
			deadScene->CosValueColorOfTheScene -= 1.9f;

			///Vad daca a terminat nivelul
			levelChoserPlayer->justFinished = false;

			addObstacle->destroyLevel();

			addObstacle->incarcaHarta("Menu Of Levels/Chapter" + BStoString(levelChoserPlayer->indexOfChapterThatItIsOn + 1) + "/The Levels objects/level" +
									  BStoString(levelChoserPlayer->indexOfLevelThatItIsOn + 1) + ".xml");

			hero->theBody->SetTransform(b2Vec2(HeroInitialX, HeroInitialY), 0);

			///Set the extremes of the camera
			std::string nameOfLevel = "Menu Of Levels/Chapter" +
									  BStoString(levelChoserPlayer->indexOfChapterThatItIsOn + 1) + "/The Levels details/level" +
									  BStoString(levelChoserPlayer->indexOfLevelThatItIsOn + 1) + ".txt";

			std::ifstream citesteLevelulCurent(nameOfLevel.c_str());
			float a1, a2, l, d, u, r;
			std::string cacat;
			citesteLevelulCurent >> a1 >> a2 >> cacat >> l >> d >> u >> r;

			CameraExtremeDown = d + SCREEN_HEIGHT / 2.f / scaleSize;
			CameraExtremeLeft = l + SCREEN_WIDTH / 2.f / scaleSize;
			CameraExtremeUp = u - SCREEN_HEIGHT / 2.f / scaleSize;
			CameraExtremeRight = r - SCREEN_WIDTH / 2.f / scaleSize;

			std::cout << "extremes: " << l << " " << d << " " << u << " " << r << '\n';
		}
		else
			if(deadScene->CosValueColorOfTheScene > 0 && deadScene->colorGoesUpOrDown == -1)
			{
				if(deadScene->CosValueColorOfTheScene < 60 && hero->theBody->GetLinearVelocity().y == 0)
				{
					hero->theBody->SetLinearVelocity(b2Vec2(0, 0.1));
					hero->theBody->ApplyLinearImpulse(b2Vec2(0, 0.1), hero->theBody->GetWorldCenter());
				}

				deadScene->CosValueColorOfTheScene -= 0.9f;
			}
			else
			{
				deadScene->CosValueColorOfTheScene = 0.f;
				deadScene->colorGoesUpOrDown = 0;
			}

	deadScene->color.a = sin(toRadians(deadScene->CosValueColorOfTheScene));
	deadScene->xCord = CameraPosition->xCord - CameraExtremeLeft;
	deadScene->yCord = CameraPosition->yCord - CameraExtremeDown;
	deadScene->zCord = scaleSizeInitial;

	deadScene->draw();
}



///This is deep wizzardry, do NOT touch, unless you have a staff, a grey pointy hat and your name is Gandalf
void BSTexture::renderAllAnimations()
{
	for(int i = 0; i < 5; i++)
	{
		int indexOfAnimation = listOfActiveAnimations[i]->indexOfAnimation,
			indexOfTexture = listOfActiveAnimations[i]->indexOfTexture,
			nr1 = listOfLoadedAnimations[indexOfAnimation]->nr1,
			nr2 = listOfLoadedAnimations[indexOfAnimation]->nr2,
			nr3 = listOfLoadedAnimations[indexOfAnimation]->nr3;

		///if we can play the animation
		if(listOfActiveAnimations[i]->canPlayIt)
		{
			///if it is time to show a new texture
			if(SDL_GetTicks() > listOfActiveAnimations[i]->timeToShow && !isPaused && gravityIsChanging == 0)
			{
				///if we don't need to finish the animation
				if(!listOfActiveAnimations[i]->finish)
				{
					///if the first part is on loop, then when it gets to the second part of the animation, we restart the animation
					if(listOfLoadedAnimations[indexOfAnimation]->loop1 == true && indexOfTexture == nr1 - 1)
					{
						indexOfTexture = 0;
					}
					else
						///if the second part is on loop, then when it gets to the third part of the animation, we restart the animation
						if(listOfLoadedAnimations[indexOfAnimation]->loop2 == true && indexOfTexture == nr1 + nr2 - 1)
						{
							indexOfTexture = nr1;
						}
						else
							///if the third part is on loop, then when it finishes the animation, we restart the animation
							///beware if he animation has its 3rd part on a loop, the animation will loop to infinity, until the end of time and the hell will freeze over
							if(listOfLoadedAnimations[indexOfAnimation]->loop3 == true && indexOfTexture == nr1 + nr2 + nr3 - 1)
							{
								indexOfTexture = nr1 + nr2;
							}
							///we just iterate through the animations
							else
							{
								indexOfTexture++;
							}
				}
				///if we need to finish the animation
				else
					if(listOfActiveAnimations[i]->finish)
					{
						///if we didn't get to the 3rd part of the animation, or if we got the 1st or 2nd parts on loop, then we get to the first texture of the 3rd part
						if(indexOfTexture < nr1 + nr2)
						{
							indexOfTexture = nr1 + nr2;
						}
						else
						{
							indexOfTexture++;
						}
					}
				if(indexOfTexture != nr1 + nr2 + nr3)
				{
					listOfActiveAnimations[i]->timeToShow = listOfLoadedAnimations[indexOfAnimation]->animation[indexOfTexture]->timeUntilNext + SDL_GetTicks();
				}
			}

			///if the index of the current texture is out of bounds (it is bigger than > nr1 + nr2 + nr3), it means the animation ended, so we set it to unplayable
			if(indexOfTexture == nr1 + nr2 + nr3)
			{
				listOfActiveAnimations[i]->setPlayableStatus(false);
			}
			///else we render the current texture
			else
			{
				listOfActiveAnimations[i]->indexOfTexture = indexOfTexture;
				listOfLoadedAnimations[indexOfAnimation]->animation[indexOfTexture]->xCord = listOfActiveAnimations[i]->xCord;
				listOfLoadedAnimations[indexOfAnimation]->animation[indexOfTexture]->yCord = listOfActiveAnimations[i]->yCord;
				listOfLoadedAnimations[indexOfAnimation]->animation[indexOfTexture]->xAngle = hero->xAngle;
				listOfLoadedAnimations[indexOfAnimation]->animation[indexOfTexture]->draw();
			}
		}
	}
}


void BSTexture::renderDustBodies()
{
	for(int i = 0; i < ListOfSquareDustBodies.size(); i++)
	{
		BSColorRGBA color = {1, 1, 1, 0};
		float decal = 0;
		USERDATA *userData = NULL;
		if(ListOfSquareDustBodies[i]->theBody)
		{
			userData = (USERDATA*)ListOfSquareDustBodies[i]->theBody->GetUserData();
		}
		if(userData && !userData->isDust)
		{
			color.a = 1.f;
		}
		else
		{
			///if it's not invisible
			if(ListOfSquareDustBodies[i]->value1 < 90)
			{
				ListOfSquareDustBodies[i]->value1 += 1.f;

				///destroy the body
				if(ListOfSquareDustBodies[i]->value1 > 30 && ListOfSquareDustBodies[i]->theBody)
				{
					world->DestroyBody(ListOfSquareDustBodies[i]->theBody);
					ListOfSquareDustBodies[i]->theBody = NULL;
				}
			}
			else
			{
				ListOfSquareDustBodies[i]->value1 = 90;
			}

			if(ListOfSquareDustBodies[i]->value1 > 30)
			{
				decal = 2 * sin(toRadians(ListOfSquareDustBodies[i]->value1 - 30));
			}
			color.a = cos(toRadians(ListOfSquareDustBodies[i]->value1));
		}

		if(color.a > 0)
		{
			BS_Renderer::renderSomeBody(ListOfSquareDustBodies[i]->xCord, ListOfSquareDustBodies[i]->yCord - decal, originalCoin->zCord, 0, 0, 0,
										originalDustBody->texture, color, originalDustBody->vao,
										originalDustBody->width, originalDustBody->height, originalDustBody->depth, true);
		}
	}
}

void BSTexture::resetDustBodies()
{
	for(int i = 0; i < ListOfSquareDustBodies.size(); i++)
	{
		if(ListOfSquareDustBodies[i]->theBody == NULL)
		{
			ListOfSquareDustBodies[i]->theBody = addObstacle->createBodies(b2_staticBody, ListOfSquareDustBodies[i]->xInit, ListOfSquareDustBodies[i]->yInit,
												 ListOfSquareDustBodies[i]->width, ListOfSquareDustBodies[i]->height, "object",
												 nrBodies, 0, false);
		}

		ListOfSquareDustBodies[i]->value1 = 0;
		ListOfSquareDustBodies[i]->value2 = 0;

		USERDATA* dataForUser = new USERDATA;

		dataForUser->isDustAble = 1;
		dataForUser->isDust = 0;

		dataForUser->canKillHero = false;
		dataForUser->isToNextLevel = false;

		dataForUser->name = "object";
		ListOfSquareDustBodies[i]->theBody->SetUserData(dataForUser);
	}
}

void BSTexture::renderTheCoin()
{
	BSColorRGBA color = {1, 1, 1, 1};
	BS_Renderer::renderSomeBody(originalCoin->xCord + CameraPosition->xCord - CameraExtremeLeft, originalCoin->yCord + CameraPosition->yCord - CameraExtremeDown,
								originalCoin->zCord, 0, 0, 0, originalCoin->texture, color, originalCoin->vao, originalCoin->width, originalCoin->height, originalCoin->depth, false);
}


void BSTexture::renderWormHoles()
{
	BSColorRGBA color = {1, 1, 1, 1};
	checkCollisionsWormHoles();

	if(!listOfWormHoles.empty())
	{
		for(int i = 0; i < listOfWormHoles.size(); i++)
		{
			if(!isPaused)
			{
				listOfWormHoles[i]->value1 -= 4;
			}

			if(listOfWormHoles[i]->value1 < -360)
			{
				listOfWormHoles[i]->value1 = listOfWormHoles[i]->value1 + 360;
			}
			BS_Renderer::renderSomeBody(listOfWormHoles[i]->xCord, listOfWormHoles[i]->yCord, listOfWormHoles[i]->zCord, listOfWormHoles[i]->value1, 0, 0,
										originalWormHole->texture, color, originalWormHole->vao,
										originalWormHole->width, originalWormHole->height, originalWormHole->depth, true);
		}
	}
	else
	{
		listOfActiveAnimations[3]->canPlayIt = false;
	}
}

void BSTexture::checkCollisionsWormHoles()
{
	///daca nu se teleporteaza, si nici nu a terminat de rematerializat la destinatie
	if(!isTeleporting && lastWorm == NULL)
	{
		for(int i = 0; i < listOfWormHoles.size(); i++)
		{
			///daca a intra intr-o gaura neagra, ii caut destinatia
			if( hero->theBody->GetPosition().x - (hero->width / 8.0f) > listOfWormHoles[i]->xCord - (originalWormHole->width / 2.0f) && ///collision body left - worm left
					hero->theBody->GetPosition().x + (hero->width / 8.0f) < listOfWormHoles[i]->xCord + (originalWormHole->width / 2.0f) && ///collision body right - worm right
					hero->theBody->GetPosition().y - (hero->height / 8.0f) > listOfWormHoles[i]->yCord - (originalWormHole->height / 2.0f) && ///collision body down - worm down
					hero->theBody->GetPosition().y + (hero->height / 8.0f) < listOfWormHoles[i]->yCord + (originalWormHole->height / 2.0f)) ///collision body up - worm up
			{
				///look for destination
				for(int j = 0; j < listOfWormHoles.size(); j++)
				{
					if(listOfWormHoles[j]->indexOfWorm == listOfWormHoles[i]->connectionIndex)
					{
						_my_sound_device->play_sound(BS_Available_Sounds::player_teleport());
						actionObj->ActionFuncTeleportHero(listOfWormHoles[j]->xCord, listOfWormHoles[j]->yCord);
						lastWorm = listOfWormHoles[j];
					}
				}
			}
		}
	}
	else
		///daca a inceput rematerializarea, verifica daca a iesit din worm hole pentru a nu se teleporta la nesfarsit intre cele 2
		if(lastWorm != NULL && hero->animationIndexCurrentAnimation != hero->animationIndexPlayerTeleportedIn)
		{
			if( hero->theBody->GetPosition().x - (hero->width / 8.0f) > lastWorm->xCord - (originalWormHole->width / 2.0f) && ///collision body left - worm left
					hero->theBody->GetPosition().x + (hero->width / 8.0f) < lastWorm->xCord + (originalWormHole->width / 2.0f) && ///collision body right - worm right
					hero->theBody->GetPosition().y - (hero->height / 8.0f) > lastWorm->yCord - (originalWormHole->height / 2.0f) && ///collision body down - worm down
					hero->theBody->GetPosition().y + (hero->height / 8.0f) < lastWorm->yCord + (originalWormHole->height / 2.0f)) ///collision body up - worm up
			{
			}
			else
			{
				lastWorm = NULL;
			}
		}
}

void BSTexture::renderMenuLevels(int nrOfLevels, ChapterLevelsStructure* theLevelsToRender[])
{
	BSColorRGBA theColorToRender;
	if(DarkLevels)
	{
		theColorToRender = { 2.f, 1.f, 1.f, 0.9f };
	}
	else
	{
		theColorToRender = { 1.f, 1.f, 1.f, 0.9f };
	}
	renderTransparentSquare(theColorToRender, SCREEN_WIDTH / cross_platform_scale / 2.0f + CameraPosition->xCord - CameraExtremeLeft,
							SCREEN_HEIGHT / cross_platform_scale / 2.0f + CameraPosition->yCord - CameraExtremeDown);

	for(int i = 0 ; i < nrOfLevels ; i++)
	{
		///First, render the connection strings
		///Daca poate pune la stanga, atunci pune
		if( (i + 1) % 5 != 0)
		{
			if(DarkLevels)
			{
				theLevelsToRender[i + 1]->color.r = 2.f;
			}
			else
			{
				theLevelsToRender[i + 1]->color.r = 1.f;
			}
			BS_Renderer::renderSomeBody(theLevelsToRender[i]->xCord + CameraPosition->xCord - CameraExtremeLeft + theLevelsToRender[i]->width / 2.f + theLevelsToRender[i]->width / 2.f,
										theLevelsToRender[i]->yCord + CameraPosition->yCord - CameraExtremeDown,
										scaleSizeInitial, 0, 0, 0, theLevelsToRender[0]->textureOfConnectionLeftRight,
										theLevelsToRender[i + 1]->color, theLevelsToRender[0]->vaoBufferOfConnectionLeftRight,
										theLevelsToRender[0]->widthConnectionLeftRightLevel, theLevelsToRender[0]->heightConnectionLeftRightLevel, theLevelsToRender[0]->depth, false);
		}

		if(i < 20)
		{
			if(DarkLevels)
			{
				theLevelsToRender[i + 5]->color.r = 2.f;
			}
			else
			{
				theLevelsToRender[i + 5]->color.r = 1.f;
			}
			BS_Renderer::renderSomeBody(theLevelsToRender[i]->xCord + CameraPosition->xCord - CameraExtremeLeft,
										theLevelsToRender[i]->yCord - theLevelsToRender[i]->height / 2.f - theLevelsToRender[i]->height / 2.f + CameraPosition->yCord - CameraExtremeDown,
										scaleSizeInitial, 0, 0, 0, theLevelsToRender[0]->textureOfConnectionUpDown,
										theLevelsToRender[i + 5]->color, theLevelsToRender[0]->vaoBufferOfConnectionUpDown,
										theLevelsToRender[0]->widthConnectionUpDownLevel,
										theLevelsToRender[0]->heightConnectionUpDownLevel, theLevelsToRender[0]->depth, false);
		}
	}

	///Randez nivelele
	for(int i = 0 ; i < nrOfLevels ; i++)
	{
		///Second, render the level images
		if(DarkLevels)
		{
			theLevelsToRender[i]->color.r = 2.f;
		}
		else
		{
			theLevelsToRender[i]->color.r = 1.f;
		}
		BS_Renderer::renderSomeBody(theLevelsToRender[i]->xCord + CameraPosition->xCord - CameraExtremeLeft,
									theLevelsToRender[i]->yCord + CameraPosition->yCord - CameraExtremeDown,
									scaleSizeInitial, 0, 0, 0,
									theLevelsToRender[0]->textureOfLevel,
									theLevelsToRender[i]->color, theLevelsToRender[i]->vaoBufferOfLevel,
									theLevelsToRender[0]->width, theLevelsToRender[0]->height, theLevelsToRender[0]->depth, false);

		if(i < 9)
		{
			renderMyFont(theLevelsToRender[i]->textOnLevel, theLevelsToRender[i]->xCord + CameraPosition->xCord - CameraExtremeLeft,
						 theLevelsToRender[i]->yCord + CameraPosition->yCord - CameraExtremeDown, 1.2f, 1.25f, theLevelsToRender[i]->color, false);
		}
		else
		{
			renderMyFont(theLevelsToRender[i]->textOnLevel, theLevelsToRender[i]->xCord + CameraPosition->xCord - CameraExtremeLeft - 0.4f,
						 theLevelsToRender[i]->yCord + CameraPosition->yCord - CameraExtremeDown, 1.2f, 1.25f, theLevelsToRender[i]->color, false);
			//theLevelsToRender[i]->xCord + CameraPosition->xCord - CameraExtremeLeft - letters[char((i + 1) / 10 % 10)]->widthOfLetterOnScreen / 2 * 1.2f,
		}

		if(theLevelsToRender[i]->isFinished)
		{
			if(DarkLevels)
			{
				theLevelsToRender[0]->color.r = 2.f;
			}
			else
			{
				theLevelsToRender[0]->color.r = 1.f;
			}
			BS_Renderer::renderSomeBody(theLevelsToRender[i]->xCord + CameraPosition->xCord - CameraExtremeLeft,
										theLevelsToRender[i]->yCord + CameraPosition->yCord - CameraExtremeDown,
										scaleSizeInitial, 0, 0, 0,
										theLevelsToRender[0]->textureOfFinishedLevel,
										theLevelsToRender[0]->color, theLevelsToRender[0]->vaoBufferOfFinishedLevel,
										theLevelsToRender[0]->width, theLevelsToRender[0]->height, theLevelsToRender[0]->depth, false);
		}
	}


	///Randez animatia cu care ma voi misca prin nivele
	BS_Renderer::renderSomeBody(levelChoserPlayer->xCord + CameraPosition->xCord - CameraExtremeLeft,
								levelChoserPlayer->yCord + CameraPosition->yCord - CameraExtremeDown,
								scaleSizeInitial, 0, 0, 0, levelChoserPlayer->texture, levelChoserPlayer->color,
								levelChoserPlayer->vao, levelChoserPlayer->width, levelChoserPlayer->height,
								levelChoserPlayer->depth, false);

	theLevelMenuPlayer->walkThroughLevels(0, 0, theLevelsToRender);
}


void BSTexture::renderMenuChapters()
{
	BSColorRGBA theColorToRender = { 1.f, 1.f, 1.f, 0.9f };
	renderTransparentSquare(theColorToRender, SCREEN_WIDTH / cross_platform_scale / 2.0f + CameraPosition->xCord - CameraExtremeLeft,
							SCREEN_HEIGHT / cross_platform_scale / 2.0f + CameraPosition->yCord - CameraExtremeDown);


	for(int i = 0 ; i < nrOfChapters - 1 ; i++)
	{
		int indexulUrmatoruluiNivel = 0;
		///First, render the connection strings
		///Daca poate pune la stanga, atunci pune
		float sizeOfConnectionStringX = 0, sizeOfConnectionStringY = 0;

		if(i + 1 < nrOfChapters)
		{
			indexulUrmatoruluiNivel = i + 1;
			sizeOfConnectionStringX = abs(chaptere[i]->xCord - chaptere[i + 1]->xCord) - chaptere[i]->width;
			sizeOfConnectionStringY = abs(chaptere[i]->yCord - chaptere[i + 1]->yCord) - chaptere[i]->height;
		}
		else
		{
			indexulUrmatoruluiNivel = 0;
			sizeOfConnectionStringX = abs(chaptere[i]->xCord - chaptere[0]->xCord) - chaptere[i]->width;
			sizeOfConnectionStringY = abs(chaptere[i]->yCord - chaptere[0]->yCord) - chaptere[i]->height;
		}

		///Daca este mai la dreapta
		if(chaptere[indexulUrmatoruluiNivel]->xCord > chaptere[i]->xCord)
		{
			BS_Renderer::renderSomeBody(chaptere[i]->xCord + CameraPosition->xCord - CameraExtremeLeft + chaptere[i]->width / 2.f + sizeOfConnectionStringX / 2.f + 0.1,
										chaptere[i]->yCord + CameraPosition->yCord - CameraExtremeDown,
										scaleSizeInitial, 0, 0, 0, chaptere[0]->levelFromChapter[0]->textureOfConnectionLeftRightChapter,
										chaptere[indexulUrmatoruluiNivel]->color, chaptere[0]->levelFromChapter[0]->vaoBufferOfConnectionLeftRightChapter,
										chaptere[0]->levelFromChapter[0]->widthConnectionLeftRightChapter,
										chaptere[0]->levelFromChapter[0]->heightConnectionLeftRightChapter, 0, false);
		}
		else
			///Daca este mai la stanga
			if(chaptere[indexulUrmatoruluiNivel]->xCord < chaptere[i]->xCord)
			{
				BS_Renderer::renderSomeBody(chaptere[i]->xCord + CameraPosition->xCord - CameraExtremeLeft - chaptere[i]->width / 2.f - sizeOfConnectionStringX / 2.f - 0.12,
											chaptere[i]->yCord + CameraPosition->yCord - CameraExtremeDown,
											scaleSizeInitial, 0, 0, 0,
											chaptere[0]->levelFromChapter[0]->textureOfConnectionLeftRightChapter,
											chaptere[indexulUrmatoruluiNivel]->color,
											chaptere[0]->levelFromChapter[0]->vaoBufferOfConnectionLeftRightChapter,
											chaptere[0]->levelFromChapter[0]->widthConnectionLeftRightChapter,
											chaptere[0]->levelFromChapter[0]->heightConnectionLeftRightChapter, 0, false);
			}
			else
				///Daca este mai jos
				if(chaptere[indexulUrmatoruluiNivel]->yCord < chaptere[i]->yCord)
				{
					BS_Renderer::renderSomeBody(chaptere[i]->xCord + CameraPosition->xCord - CameraExtremeLeft,
												chaptere[i]->yCord + chaptere[i]->height / 2.f - sizeOfConnectionStringY / 2.f + 0.2 + CameraPosition->yCord - CameraExtremeDown,
												scaleSizeInitial, 0, 0, 0,
												chaptere[0]->levelFromChapter[0]->textureOfConnectionUpDownChapter,
												chaptere[indexulUrmatoruluiNivel]->color,
												chaptere[0]->levelFromChapter[0]->vaoBufferOfConnectionUpDownChapter,
												chaptere[0]->levelFromChapter[0]->widthConnectionUpDownChapter,
												chaptere[0]->levelFromChapter[0]->heightConnectionUpDownChapter, 0, false);
				}
				else
					///Daca este mai sus
					if(chaptere[indexulUrmatoruluiNivel]->yCord > chaptere[i]->yCord)
					{
						BS_Renderer::renderSomeBody(chaptere[i]->xCord + CameraPosition->xCord - CameraExtremeLeft,
													chaptere[i]->yCord + chaptere[i]->height / 2.f + sizeOfConnectionStringY / 2.f + CameraPosition->yCord - CameraExtremeDown,
													scaleSizeInitial, 0, 0, 0,
													chaptere[0]->levelFromChapter[0]->textureOfConnectionUpDownChapter,
													chaptere[indexulUrmatoruluiNivel]->color,
													chaptere[0]->levelFromChapter[0]->vaoBufferOfConnectionUpDownChapter,
													chaptere[0]->levelFromChapter[0]->widthConnectionUpDownChapter,
													chaptere[0]->levelFromChapter[0]->heightConnectionUpDownChapter, 0, false);
					}

	}

	///Randez chapterele
	for(int i = 0 ; i < nrOfChapters ; i++)
	{
		///Second, render the level images
		BS_Renderer::renderSomeBody(chaptere[i]->xCord + CameraPosition->xCord - CameraExtremeLeft,
									chaptere[i]->yCord + CameraPosition->yCord - CameraExtremeDown,
									scaleSizeInitial, 0, 0, 0,
									chaptere[i]->texture,
									chaptere[i]->color,
									chaptere[i]->vao,
									chaptere[i]->width, chaptere[i]->height, chaptere[i]->depth, false);

		if(chaptere[i]->isFinishedChapter)
		{
			BS_Renderer::renderSomeBody(chaptere[i]->xCord + CameraPosition->xCord - CameraExtremeLeft,
										chaptere[i]->yCord + CameraPosition->yCord - CameraExtremeDown,
										scaleSizeInitial, 0, 0, 0,
										chaptere[0]->levelFromChapter[0]->textureOfFinishedLevel,
										chaptere[0]->levelFromChapter[0]->color,
										chaptere[0]->levelFromChapter[0]->vaoBufferOfFinishedLevel, 0, 0, 0, false);
		}
	}

	///Randez animatia cu care ma voi misca prin nivele
	BS_Renderer::renderSomeBody(levelChoserPlayer->xCord + CameraPosition->xCord - CameraExtremeLeft,
								levelChoserPlayer->yCord + CameraPosition->yCord - CameraExtremeDown,
								scaleSizeInitial, 0, 0, 0,
								levelChoserPlayer->texture, levelChoserPlayer->color, levelChoserPlayer->vao,
								levelChoserPlayer->width, levelChoserPlayer->height, levelChoserPlayer->depth, false);

	theLevelMenuPlayer->walkThroughChapters(0, 0);
}





void BSTexture::renderMessages()
{
	ifCollideWithMessage();

	if(nrOfMessageSign)
	{
		for(int i = 0; i < listOfMessages.size(); i++)
		{
			///Randez semnul
			BS_Renderer::renderSomeBody(listOfMessages[i]->xCord, listOfMessages[i]->yCord, scaleSizeInitial, 0, 0, 0,
										listOfMessages[i]->textureOfSign, listOfMessages[i]->colorOfSign,
										listOfMessages[i]->vaoBufferOfSign, listOfMessages[i]->widthOfSign, listOfMessages[i]->heightOfSign, 0, true);

			///Randez mesajul daca este afisat
			if(listOfMessages[i]->isShownTheMessage)
			{
				///Il fac opac
				if(listOfMessages[i]->colorOfMessage.a < 0.9f)
				{
					listOfMessages[i]->colorOfMessage.a += 0.02;
				}
				else
				{
					listOfMessages[i]->colorOfMessage.a = 0.9f;
				}

				BS_Renderer::renderSomeBody(listOfMessages[i]->xCord + listOfMessages[i]->widthOfMessage / 2.f,
											listOfMessages[i]->yCord + listOfMessages[i]->heightOfMessage / 2.f + listOfMessages[i]->heightOfSign / 2.f + 0.2f,
											scaleSizeInitial, 0, 0, 0, listOfMessages[i]->textureOfMessage, listOfMessages[i]->colorOfMessage,
											listOfMessages[i]->vaoBufferOfMessage, listOfMessages[i]->widthOfMessage, listOfMessages[i]->heightOfMessage, 0, true);
			}
			else
			{
				///Il fac transparent
				if(listOfMessages[i]->colorOfMessage.a > 0.f)
				{
					listOfMessages[i]->colorOfMessage.a -= 0.02;

					BS_Renderer::renderSomeBody(listOfMessages[i]->xCord + listOfMessages[i]->widthOfMessage / 2.f,
												listOfMessages[i]->yCord + listOfMessages[i]->heightOfMessage / 2.f + listOfMessages[i]->heightOfSign / 2.f + 0.2f,
												scaleSizeInitial, 0, 0, 0, listOfMessages[i]->textureOfMessage, listOfMessages[i]->colorOfMessage,
												listOfMessages[i]->vaoBufferOfMessage, listOfMessages[i]->widthOfMessage, listOfMessages[i]->heightOfMessage, 0, true);
				}
				else
				{
					listOfMessages[i]->colorOfMessage.a = 0.f;
				}
			}
		}
	}
}

void BSTexture::ifCollideWithMessage()
{
	if(nrOfMessageSign)
	{
		for(int i = 0; i < listOfMessages.size(); i++)
		{
			///if it is colliding
			if(( (hero->theBody->GetPosition().x + hero->width / 2.f >= listOfMessages[i]->xCord - listOfMessages[i]->widthOfSign / 2.f &&
					hero->theBody->GetPosition().x + hero->width / 2.f <= listOfMessages[i]->xCord + listOfMessages[i]->widthOfSign / 2.f) || //coliziune la stanga
					(hero->theBody->GetPosition().x - hero->width / 2.f <= listOfMessages[i]->xCord + listOfMessages[i]->widthOfSign / 2.f &&
					 hero->theBody->GetPosition().x - hero->width / 2.f >= listOfMessages[i]->xCord - listOfMessages[i]->widthOfSign / 2.f) ) //coliziune la dreapta
					&&
					((hero->theBody->GetPosition().y + hero->height / 2.f >= listOfMessages[i]->yCord - listOfMessages[i]->heightOfSign / 2.f &&
					  hero->theBody->GetPosition().y + hero->height / 2.f <= listOfMessages[i]->yCord + listOfMessages[i]->heightOfSign / 2.f) || //coliziune sus/jos
					 (hero->theBody->GetPosition().y - hero->height / 2.f <= listOfMessages[i]->yCord + listOfMessages[i]->heightOfSign / 2.f &&
					  hero->theBody->GetPosition().y - hero->height / 2.f >= listOfMessages[i]->yCord - listOfMessages[i]->heightOfSign / 2.f))) //coliziune sus/jos
			{
				listOfMessages[i]->isShownTheMessage = true;
			}
			else
			{
				listOfMessages[i]->isShownTheMessage = false;
			}
		}
	}
}

void BSTexture::renderTransparentSquare(BSColorRGBA theColorToRenders, float coordinateX, float coordinateY)
{
	theColorToRenders.r = 0;
	theColorToRenders.g = 0;
	theColorToRenders.b = 0;

	BS_Renderer::renderSomeBody(coordinateX, coordinateY, scaleSizeInitial, 0, 0, 0, whiteTexture,
								theColorToRenders, originalTransparentMenu->vao,
								originalTransparentMenu->width, originalTransparentMenu->height, originalTransparentMenu->depth, false);
}

void BSTexture::renderAnyButtons(BSMenuStructure *theMenu)
{
	//Rnder The Background with an alpha black
	theColorToRender = { 1.f, 1.f, 1.f, 0.9f };
	renderTransparentSquare(theColorToRender, SCREEN_WIDTH / cross_platform_scale / 2.0f + CameraPosition->xCord - CameraExtremeLeft,
							SCREEN_HEIGHT / cross_platform_scale / 2.0f + CameraPosition->yCord - CameraExtremeDown);

	theColorToRender = { 1.f, 1.f, 1.f, 1.f };

	for(int i = 0; i < theMenu->listOfButtons.size(); i++)
	{
		float whRatio = (float)(currentButton->height / currentButton->width);
		float newW = (float) currentButton->width + 1 + sin(toRadians(currentButton->value1));
		float newH = (float) currentButton->height + (1 + sin(toRadians(currentButton->value2))) * whRatio;
		if(theMenu->listOfButtons[i] == currentButton && (currentMenu->indexOfMenu != 2 ||
				(currentMenu->indexOfMenu == 2 && currentButton->indexOfButton != 0 && currentButton->indexOfButton != 1)))
		{
			currentButton->value1 += 5;
			currentButton->value2 += 5;

			if(currentButton->value1 > 360 && currentButton->value2 > 360)
			{
				currentButton->value1 = currentButton->value1 - 360;
				currentButton->value2 = currentButton->value2 - 360;
			}
		}

		if(theMenu->listOfButtons[i] == currentButton)
		{
			BS_Renderer::renderSomeBody(currentButton->xCord + CameraPosition->xCord - CameraExtremeLeft,
										currentButton->yCord + CameraPosition->yCord - CameraExtremeDown,
										scaleSizeInitial, 0, 0, 0, currentButton->textureOfButtons[1], currentButton->color, currentButton->vaoOfButtons[1],
										newW, newH, currentButton->depth, false);
		}
		else
		{
			BS_Renderer::renderSomeBody(theMenu->listOfButtons[i]->xCord + CameraPosition->xCord - CameraExtremeLeft,
										theMenu->listOfButtons[i]->yCord + CameraPosition->yCord - CameraExtremeDown,
										scaleSizeInitial, 0, 0, 0, theMenu->listOfButtons[i]->textureOfButtons[0], theMenu->listOfButtons[i]->color,
										theMenu->listOfButtons[i]->vaoOfButtons[0], theMenu->listOfButtons[i]->width,
										theMenu->listOfButtons[i]->height, theMenu->listOfButtons[i]->depth, false);
		}

		if(currentMenu->indexOfMenu == 2)
		{
			for(int j = 0; j < theMenu->listOfButtons.size(); j++)
			{
				if(theMenu->listOfButtons[j]->indexOfButton == 0)
				{
					renderSoundAnimation(theMenu->listOfButtons[j]->yCord, newVolumeAudio);
				}
				else
					if(theMenu->listOfButtons[j]->indexOfButton == 1)
					{
						renderSoundAnimation(theMenu->listOfButtons[j]->yCord, newVolumeSFX);
					}
					else
					{
						break;
					}
			}
		}
	}
}

void BSTexture::renderSoundAnimation(float yCord, float theVolume)
{
	for(int i = 0 ; i < theVolume * 10 ; i++)
	{
		BS_Renderer::renderSomeBody(SoundButtonTexture->xCord + CameraPosition->xCord - CameraExtremeLeft + Hero_size * i + 0.25 * i - 3.2,
									yCord + CameraPosition->yCord - CameraExtremeDown,
									scaleSizeInitial, 0, 0, 0, SoundButtonTexture->texture, SoundButtonTexture->color,
									SoundButtonTexture->vao, SoundButtonTexture->width, SoundButtonTexture->height, SoundButtonTexture->depth, false);
	}
}

void BSTexture::renderBackgroundNo1()
{
	float val1 = 25, val2 = 25, val3 = 3, val4 = 47;
	backgroundSky->xCord = val1;
	backgroundMountain->xCord = val2;
	backgroundBush->xCord = val3;
	backgroundHighField->xCord = val4;

	float val11 = 20, val21 = 18, val31 = 12, val41 = 14;
	backgroundSky->yCord = val11;
	backgroundMountain->yCord = val21;
	backgroundBush->yCord = val31;
	backgroundHighField->yCord = val41;

	BS_Renderer::renderSomeBody(backgroundSky->xCord - 1, backgroundSky->yCord - 2, -40, 0, 0, 0,
								backgroundSky->texture, backgroundSky->color, backgroundSky->vao,
								2.4f * backgroundSky->width, 3 * backgroundSky->height + 10, 0, true);

	BS_Renderer::renderSomeBody(backgroundMountain->xCord, backgroundMountain->yCord, -35, 0, 0, 0,
								backgroundMountain->texture, backgroundMountain->color, backgroundMountain->vao,
								2 * backgroundMountain->width, 3 * backgroundMountain->height, 0, true);

	BS_Renderer::renderSomeBody(backgroundBush->xCord, backgroundBush->yCord, -30, 0, 0, 0,
								backgroundBush->texture, backgroundBush->color, backgroundBush->vao,
								2 * backgroundBush->width, 2 * backgroundBush->height, 0, true);

	BS_Renderer::renderSomeBody(backgroundHighField->xCord - 10, backgroundHighField->yCord, -23, 0, 0, 0,
								backgroundHighField->texture, backgroundHighField->color, backgroundHighField->vao,
								2 * backgroundHighField->width, 2 * backgroundHighField->height, 0, true);
}

void BSTexture::renderChuncksOfObjects(float coordX, float coordY, float coordZ, float ScaleOnX, float ScaleOnY)
{
	BSModelViewMatrix = glm::translate( coordX, coordY, coordZ ) * LookAtMatrix * glm::scale( ScaleOnX, ScaleOnY, 0.0f );
	BSModelViewMatrix = glm::rotate( worldAngle, 0.0f, 0.0f, 1.0f ) * BSModelViewMatrix;

	glUniformMatrix4fv( _my_shader_program->get_location_from_shader(BS_Available_Shaders::model_view_matrix()), 1, GL_FALSE, glm::value_ptr( BSModelViewMatrix ) );

	glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );
}



void BSTexture::loadTextures()
{
	BS_Renderer::loadATexture("texture.png", texture);
	BS_Renderer::loadATexture("whiteImage.png", whiteTexture);
	BS_Renderer::loadATexture("Animations/roundAnimation/windmillPropeller.png", textureMill);
	BS_Renderer::loadATexture("playerTexture.png", playerTexture);
	BS_Renderer::loadATexture("playerTexture round.png", playerTextureRound);
	BS_Renderer::loadATexture("playerTexture Dead.png", playerTextureDead);
	BS_Renderer::loadATexture("background.png", backgroundTexture);
}

void BSTexture::loadMenuButtonsTexture(int numberOfButtons, std::string pathAndName, GLuint theButtonsTexture[])
{
	for(int  i = 0 ; i < numberOfButtons ; i ++)
	{
		std::string TextureName = pathAndName + BStoString(i + 1) + ".png";

		theButtonsTexture[i] = SOIL_load_OGL_texture
							   (
								   TextureName.c_str(),
								   SOIL_LOAD_RGBA,
								   SOIL_CREATE_NEW_ID,
								   SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
							   );
	}
}

float BSTexture::toRadians(float x)
{
	float pi = 3.1415;
	return (float) (pi / 2 * x) / 90;
}

void BSTexture::initTheFont()
{
	BSXmlFiles *doc = new BSXmlFiles();
//	char theNodesNew[6][100];
//	ListaDeCaractere *levelDetailsNew[1000];
	std::vector<std::string> theNodesNew;
	std::vector<std::string> theAttrNew;
	std::vector<std::string> emptyVector;
	std::vector<ListaDeCaractere*> levelDetailsNew;

	theNodesNew.push_back("n");
	theNodesNew.push_back("x");
	theNodesNew.push_back("y");
	theNodesNew.push_back("w");
	theNodesNew.push_back("h");
	theNodesNew.push_back("oX");
	theNodesNew.push_back("oY");
	theNodesNew.push_back("oW");
	theNodesNew.push_back("oH");

	emptyVector.push_back("imagePath");
	emptyVector.push_back("width");
	emptyVector.push_back("height");

	doc->readAnyXML("Animations/Font/dataOfFont.xml", "TextureAtlas", "sprite", theNodesNew, emptyVector, levelDetailsNew, theAttrNew);

	int i = 0;

	letters[0] = new BSTheFontLetters;

	while(i < levelDetailsNew.size() && levelDetailsNew[i])
	{
//		char let = levelDetailsNew[i]->theCharArray2D[0][0];
		char let = levelDetailsNew[i]->theCharArray2D[0][levelDetailsNew[i]->theCharArray2D[0].size() - 5];
		std::cout << let << " '" << levelDetailsNew[i]->theCharArray2D[0] << "' " << levelDetailsNew[i]->theCharArray2D[1]
				  << ' ' << levelDetailsNew[i]->theCharArray2D[2] << ' ' << levelDetailsNew[i]->theCharArray2D[3] << ' ' << levelDetailsNew[i]->theCharArray2D[4] << '\n';

		letters[let] = new BSTheFontLetters;

		letters[let]->nameOfLetterCapsLockOn = levelDetailsNew[i]->theCharArray2D[0][0];
		letters[let]->xCordInImage = atof(levelDetailsNew[i]->theCharArray2D[1].c_str());
		letters[let]->yCordInImage = atof(levelDetailsNew[i]->theCharArray2D[2].c_str());
		letters[let]->widthOfLetterInImage = atof(levelDetailsNew[i]->theCharArray2D[3].c_str());
		letters[let]->heightOfLetterInImage = atof(levelDetailsNew[i]->theCharArray2D[4].c_str());

		letters[let]->widthOfLetterOnScreen = (float) letters[let]->widthOfLetterInImage / letters[let]->heightOfLetterInImage;
		letters[let]->heightOfLetterOnScreen = 1;

		letters[let]->xRatio = (double) letters[let]->widthOfLetterInImage / 1000;
		letters[let]->yRatio = (double) letters[let]->heightOfLetterInImage / 1000;

		if(!i)
		{
			BS_Renderer::loadATexture("Animations/Font/dataOfFont.png", letters[0]->texture);
		}

		letters[let]->color.setColor(0, 0, 0, 1);

		addObstacle->set_vao_data_font(letters[let]->data, letters[let]->widthOfLetterInImage, letters[let]->heightOfLetterInImage, letters[let]->xCordInImage, letters[let]->yCordInImage, letters[let]->vao, letters[let]->vbo, letters[let]->ibo);

		delete levelDetailsNew[i];

		i++;
	}
	delete doc;

	NrOfTotalLetters = i;
	std::cout << "initialized font" << '\n';
}

void BSTexture::renderMyFont(std::string theStringToRenderString, float xCord, float yCord, float FontSizeX, float FontSizeY,
							 BSColorRGBA color, bool can)
{
	float lastWidth = 0, lastX = 0;
	BSColorRGBA newColor = color;

	///Parcurg toate literele sirului randat
	for(unsigned int i = 0 ; i < theStringToRenderString.size() ; i++)
	{
		if(letters[theStringToRenderString[i]] || theStringToRenderString[i] == ' ')
		{
			float xSize = FontSizeX, ySize = FontSizeY, newWidth = 1, newY = yCord, pozNou = 0;
			char let = theStringToRenderString[i];
			newColor = color;

			//if it is a letter or a number
			if( (theStringToRenderString[i] >= 'a' && theStringToRenderString[i] <= 'z') || (theStringToRenderString[i] >= 'A' && theStringToRenderString[i] <= 'Z') || (theStringToRenderString[i] >= '0' && theStringToRenderString[i] <= '9'))
			{
				newColor = color;
				newWidth = letters[theStringToRenderString[i]]->widthOfLetterOnScreen;
			}
			else
				if(theStringToRenderString[i] == ' ')
				{
					let = 'a';
					newWidth = letters['a']->widthOfLetterOnScreen;
					newColor = {1, 1, 1, 0};
				}
				else
					if(theStringToRenderString[i] == '.')
					{
						xSize = FontSizeX / 5.0f;
						ySize = FontSizeY / 5.0f;
						newWidth = letters[theStringToRenderString[i]]->widthOfLetterOnScreen / 5.0f;
						newY = yCord - FontSizeY / 2.0f + 0.1f;
					}
					else
						if(theStringToRenderString[i] == ',')
						{
							xSize = FontSizeX / 3.0f;
							ySize = FontSizeY / 5.0f;
							newWidth = letters[theStringToRenderString[i]]->widthOfLetterOnScreen / 5.0f;
							newY = yCord - FontSizeY / 2.0f + 0.1f;
						}
						else
							if(theStringToRenderString[i] == ';' || theStringToRenderString[i] == ':')
							{
								xSize = FontSizeX / 2.0f;
								ySize = FontSizeY / 1.4f;
								newWidth = letters[theStringToRenderString[i]]->widthOfLetterOnScreen / 2.0f;
								newY = yCord - FontSizeY / 2.0f + 0.1f;
							}
			if(!i)
			{
				pozNou = xCord;
			}
			else
			{
				pozNou = (float) lastX + (newWidth + lastWidth ) * FontSizeX / 2.f + 0.1f;
			}
			lastX = pozNou;
			lastWidth = newWidth;

			BS_Renderer::renderSomeBody(pozNou, newY, scaleSizeInitial, 0, 0, 0, letters[0]->texture, newColor, letters[let]->vao, xSize, ySize, 0, can);
		}
	}
}

bool BSTexture::coinIsInBounds(BSCoins *theCoin)
{
	if(( (hero->theBody->GetPosition().x + hero->width / 2.f >= theCoin->xCord - theCoin->width / 2.f &&
			hero->theBody->GetPosition().x + hero->width / 2.f <= theCoin->xCord + theCoin->width / 2.f) ||
			(hero->theBody->GetPosition().x - hero->width / 2.f <= theCoin->xCord + theCoin->width / 2.f &&
			 hero->theBody->GetPosition().x - hero->width / 2.f >= theCoin->xCord - theCoin->width / 2.f) )
			&&
			((hero->theBody->GetPosition().y + hero->height / 2.f >= theCoin->yCord - theCoin->height / 2.f &&
			  hero->theBody->GetPosition().y + hero->height / 2.f <= theCoin->yCord + theCoin->height / 2.f) ||
			 (hero->theBody->GetPosition().y - hero->height / 2.f <= theCoin->yCord + theCoin->height / 2.f &&
			  hero->theBody->GetPosition().y - hero->height / 2.f >= theCoin->yCord - theCoin->height / 2.f)))
	{
		return true;
	}
	return false;
}



//void BSTexture::lightTheWay()
//{
//    float scaleRatio = (float)(SCREEN_HEIGHT / 18.0f);
//    int nrOfBodiesInWidth = (float) SCREEN_WIDTH / scaleRatio;
//
//	for(std::list<BSTheSquareBodies*>::iterator i = ListOfSquareBodies.begin(); i != ListOfSquareBodies.end(); i++)
//	{
//	    ///if the current body is in the bounds of the screen
//	    if(hero->theBody->GetPosition().x + nrOfBodiesInWidth < (*i)->theBody->getPosition().x - (*i)->width &&
//           hero->theBody->GetPosition().x - nrOfBodiesInWidth > (*i)->theBody->getPosition().x + (*i)->width &&)
//	}
//}





void BSTexture::renderSnowFlakes()
{
	float maximX = gameBoundRight;
	float minimX = gameBoundLeft;

	float maximY = gameBoundUp;
	float minimY = gameBoundDown;

	_my_shader_program->set_uniform(BS_Available_Shaders::is_flake(), 1);
	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 1);
	_my_shader_program->set_uniform(BS_Available_Shaders::circle_radius(), 1);
//	glUniform1f(ShaderFlake, 1);
//	glUniform1f(ShaderIsCircle, 1);
//	glUniform1f(ShaderCircleRadius, 1);

//	BSColorRGBA color = {0, 0, 0, 1};

	_my_shader_program->set_uniform(BS_Available_Shaders::texture_color(), 0, 0, 0, 1 );

	glBindVertexArray(originalCoin->vao);
	glBindTexture(GL_TEXTURE_2D, originalParticle->texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR);

	for(int i = 0; i < listOfSnowFlakes.size(); i++)
	{
		listOfSnowFlakes[i]->xCord += listOfSnowFlakes[i]->xPower;
		listOfSnowFlakes[i]->yCord -= listOfSnowFlakes[i]->yPower;

		renderChuncksOfObjects(listOfSnowFlakes[i]->xCord, listOfSnowFlakes[i]->yCord, scaleSizeInitial, listOfSnowFlakes[i]->width, listOfSnowFlakes[i]->width);

		///check if snow flake got out of boundary
		if(listOfSnowFlakes[i]->xCord > maximX || listOfSnowFlakes[i]->xCord < minimX || listOfSnowFlakes[i]->yCord < minimY || listOfSnowFlakes[i]->yCord > maximY)
		{
			listOfSnowFlakes[i]->sinValue = 0;
			listOfSnowFlakes[i]->xCord = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximX - minimX + 1) + minimX;
			listOfSnowFlakes[i]->yCord = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximY - minimY + 1) + minimY;
		}
	}
	_my_shader_program->set_uniform(BS_Available_Shaders::is_flake(), 0);
	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 0);
//	glUniform1f(ShaderFlake, 0);
//	glUniform1f(ShaderIsCircle, 0);
}

void BSTexture::renderMouseTexture()
{
	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 1);
//	glUniform1f(ShaderIsCircle, 1);
	BSColorRGBA color = {0, 0, 0, 1};
	for(int i = 0; i < listOfMouseCircles.size(); i++)
	{
		_my_shader_program->set_uniform(BS_Available_Shaders::circle_radius(), listOfMouseCircles[i]->width);
//		glUniform1f(ShaderCircleRadius, listOfMouseCircles[i]->width);
		listOfMouseCircles[i]->color = color;
		listOfMouseCircles[i]->zCord = scaleSizeInitial;
		listOfMouseCircles[i]->width = 1;
		listOfMouseCircles[i]->height = 1;
		listOfMouseCircles[i]->draw(blankTexture);
	}
	_my_shader_program->set_uniform(BS_Available_Shaders::is_circle(), 0);
//	glUniform1f(ShaderIsCircle, 0);
}

void BSTexture::renderTheCloudyParticles()
{
	int s = 0;
	for(int i = 0; i < listOfCloudyParticles.size(); i++)
	{
		s++;
		listOfCloudyParticles[i]->xCord = hero->theBody->GetPosition().x + s;
		listOfCloudyParticles[i]->yCord = hero->theBody->GetPosition().y;
		listOfCloudyParticles[i]->zCord = scaleSizeInitial;

		listOfCloudyParticles[i]->draw();
	}
}

void BSTexture::consoleSalveazaHarta()
{
	salveazaFinishedLevel();

	int nrOfLinesSave = 0;/// pauze = 0, indexNumeHarta = 0;
//		char nodesName[100][100], attributesName[100][100], attributesValues[100][100];
//		ListaDeCaractere *nodesForXML[1000];
	std::vector<std::string> theNodes;
	std::vector<std::string> attributesName;
	std::vector<std::string> attributesValues;
	std::vector<ListaDeCaractere*> nodesForXML;

	std::string theNameOfMap = "Menu Of Levels/Chapter" + BStoString(levelChoserPlayer->indexOfChapterThatItIsOn + 1)
							   + "/The Levels objects/level" + BStoString(levelChoserPlayer->indexOfLevelThatItIsOn + 1)
							   + ".xml";
	///Salvez jucatorul
	ListaDeCaractere *heroLine = new ListaDeCaractere;

	heroLine->theCharArray2D[0] = "0";
	heroLine->theCharArray2D[1] = "playerTexture.png";
	heroLine->theCharArray2D[2] = "0";
	heroLine->theCharArray2D[3] = BStoString(HeroInitialX);
	heroLine->theCharArray2D[4] = BStoString(HeroInitialY);
	heroLine->theCharArray2D[5] = BStoString(Hero_size);
	heroLine->theCharArray2D[6] = BStoString(Hero_size);
	heroLine->theCharArray2D[7] = "hero";
	heroLine->theCharArray2D[8] = "playerAnimation";
	heroLine->theCharArray2D[9] = "0";

	nodesForXML.push_back(heroLine);

	std::cout << '\n' << "Salveaza harta: " << theNameOfMap << '\n';

	//Parcurg toate corpurile si le salvez
	for(int i = 0; i < ListOfRoundBodies.size(); i++)
	{
		ListaDeCaractere *line = new ListaDeCaractere;
		//extrag userData-ul
		USERDATA *salveazaUserData = (USERDATA*)ListOfRoundBodies[i]->theBody->GetUserData();

		if(ListOfRoundBodies[i]->script)
		{
			line->theCharArray2D[0] = "9";
			line->theCharArray2D[3] = BStoString(ListOfRoundBodies[i]->script->xInitial);
			line->theCharArray2D[4] = BStoString(ListOfRoundBodies[i]->script->yInitial);;
		}
		else
		{
			line->theCharArray2D[0] = "1";
			line->theCharArray2D[3] = BStoString(ListOfRoundBodies[i]->theBody->GetPosition().x);
			line->theCharArray2D[4] = BStoString(ListOfRoundBodies[i]->theBody->GetPosition().y);
		}
		line->theCharArray2D[1] = ListOfRoundBodies[i]->nameOfTexture;

		line->theCharArray2D[5] = BStoString(ListOfRoundBodies[i]->width);
		line->theCharArray2D[6] = BStoString(ListOfRoundBodies[i]->width);
		line->theCharArray2D[7] = salveazaUserData->name;
		line->theCharArray2D[8] = "roundAnimation";
		line->theCharArray2D[9] = BStoString(ListOfRoundBodies[i]->index);

		if(ListOfRoundBodies[i]->ObstacleFinishesLevel == true)
		{
			line->theCharArray2D[2] = "2";
		}
		else
		{
			line->theCharArray2D[2] = BStoString(ListOfRoundBodies[i]->ObstacleCanKillHero);
		}
		nodesForXML.push_back(line);
		delete salveazaUserData;
	}

	for(int i = 0; i < ListOfSquareBodies.size(); i++)
	{
		ListaDeCaractere *line = new ListaDeCaractere;
		USERDATA *salveazaUserData = (USERDATA*)ListOfSquareBodies[i]->theBody->GetUserData();

		if(ListOfSquareBodies[i]->script)
		{
			line->theCharArray2D[0] = "8";
			line->theCharArray2D[3] = BStoString(ListOfSquareBodies[i]->script->xInitial);
			line->theCharArray2D[4] = BStoString(ListOfSquareBodies[i]->script->yInitial);
		}
		else
		{
			line->theCharArray2D[0] = "0";
			line->theCharArray2D[3] = BStoString(ListOfSquareBodies[i]->theBody->GetPosition().x);
			line->theCharArray2D[4] = BStoString(ListOfSquareBodies[i]->theBody->GetPosition().y);
		}

		line->theCharArray2D[1] = ListOfSquareBodies[i]->nameOfTexture;
		line->theCharArray2D[5] = BStoString(ListOfSquareBodies[i]->width);
		line->theCharArray2D[6] = BStoString(ListOfSquareBodies[i]->height);
		line->theCharArray2D[7] = salveazaUserData->name;
		line->theCharArray2D[8] = "textureAnimation";
		line->theCharArray2D[9] = BStoString(ListOfSquareBodies[i]->index);

		///Vad daca obstacolul va fi cel care il va trece pe jucator la nivelul urmator
		if(ListOfSquareBodies[i]->ObstacleFinishesLevel == true)
		{
			line->theCharArray2D[2] = "2";
		}
		else
		{
			line->theCharArray2D[2] = BStoString(ListOfSquareBodies[i]->ObstacleCanKillHero);
		}
		nodesForXML.push_back(line);
	}

	for(int i = 0; i < listOfMessages.size(); i++)
	{
		ListaDeCaractere *line = new ListaDeCaractere;

		line->theCharArray2D[0] = "2";
		line->theCharArray2D[1] = listOfMessages[i]->nameOfTextureMessage;
		line->theCharArray2D[2] = "0";
		line->theCharArray2D[3] = BStoString(listOfMessages[i]->xCord);
		line->theCharArray2D[4] = BStoString(listOfMessages[i]->yCord);
		line->theCharArray2D[5] = "0";
		line->theCharArray2D[6] = "0";
		line->theCharArray2D[7] = "message";
		line->theCharArray2D[8] = "messageAnimation";
		line->theCharArray2D[9] = BStoString(listOfMessages[i]->index);

		nodesForXML.push_back(line);
	}

	for(int i = 0; i < listOfCoins.size(); i++)
	{
		ListaDeCaractere *line = new ListaDeCaractere;

		line->theCharArray2D[0] = "5";
		line->theCharArray2D[1] = "Animations/coin.png";
		line->theCharArray2D[2] = "0";
		line->theCharArray2D[3] = BStoString(listOfCoins[i]->xInit);
		line->theCharArray2D[4] = BStoString(listOfCoins[i]->yInit);
		line->theCharArray2D[5] = "1";
		line->theCharArray2D[6] = "1";
		line->theCharArray2D[7] = "coin";
		line->theCharArray2D[8] = BStoString(listOfCoins[i]->available);
		line->theCharArray2D[9] = BStoString(listOfCoins[i]->index);

		nodesForXML.push_back(line);
	}

	for(int i = 0; i < listOfFans.size(); i++)
	{
		ListaDeCaractere *line = new ListaDeCaractere;

		line->theCharArray2D[0] = "6";
		line->theCharArray2D[1] = "Animations/fan.png";
		line->theCharArray2D[2] = "0";
		line->theCharArray2D[3] = BStoString(listOfFans[i]->xCord);
		line->theCharArray2D[4] = BStoString(listOfFans[i]->yCord);
		line->theCharArray2D[5] = BStoString(listOfFans[i]->width);
		line->theCharArray2D[6] = BStoString(listOfFans[i]->height);
		line->theCharArray2D[7] = BStoString(listOfFans[i]->lenthOfPower) + " " + BStoString(listOfFans[i]->powerX) + " " + BStoString(listOfFans[i]->powerY);
		line->theCharArray2D[8] = "none";
		line->theCharArray2D[9] = BStoString(listOfFans[i]->index);

		nodesForXML.push_back(line);
	}

	for(int i = 0; i < listOfWormHoles.size(); i++)
	{
		ListaDeCaractere *line = new ListaDeCaractere;

		line->theCharArray2D[0] = "7";
		line->theCharArray2D[1] = "Animations/wormHole.png";
		line->theCharArray2D[2] = "0";
		line->theCharArray2D[3] = BStoString(listOfWormHoles[i]->xCord);
		line->theCharArray2D[4] = BStoString(listOfWormHoles[i]->yCord);
		line->theCharArray2D[5] = "0";
		line->theCharArray2D[6] = "0";
		line->theCharArray2D[7] = BStoString(listOfWormHoles[i]->indexOfWorm);
		line->theCharArray2D[8] = BStoString(listOfWormHoles[i]->connectionIndex);
		line->theCharArray2D[9] = BStoString(listOfWormHoles[i]->index);

		nodesForXML.push_back(line);
	}

	for(int i = 0; i < ListOfSquareDustBodies.size(); i++)
	{
		ListaDeCaractere *line = new ListaDeCaractere;

		line->theCharArray2D[0] = "10";
		line->theCharArray2D[3] = BStoString(ListOfSquareDustBodies[i]->xInit);
		line->theCharArray2D[4] = BStoString(ListOfSquareDustBodies[i]->yInit);


		line->theCharArray2D[1] = ListOfSquareDustBodies[i]->nameOfTexture;
		line->theCharArray2D[5] = BStoString(ListOfSquareDustBodies[i]->width);
		line->theCharArray2D[6] = BStoString(ListOfSquareDustBodies[i]->height);
		line->theCharArray2D[7] = "object";
		line->theCharArray2D[8] = "dust";
		line->theCharArray2D[9] = BStoString(ListOfSquareDustBodies[i]->index);

		line->theCharArray2D[2] = "0";

		nodesForXML.push_back(line);
	}

	BSXmlFiles *saveTheMap = new BSXmlFiles();
	theNodes.push_back("Body_Type");
	theNodes.push_back("Texture_path");
	theNodes.push_back("Kill_or_Next_Level");
	theNodes.push_back("xCoordinate");
	theNodes.push_back("yCoordinate");
	theNodes.push_back("width");
	theNodes.push_back("height");
	theNodes.push_back("userName");
	theNodes.push_back("Animation_name");
	theNodes.push_back("Index");

	attributesName.push_back("name");
	attributesName.push_back("boundary_Left");
	attributesName.push_back("boundary_Down");
	attributesName.push_back("boundary_Up");
	attributesName.push_back("boundary_Right");
	attributesName.push_back("script");

	attributesValues.push_back(theNameOfMap);
	attributesValues.push_back(BStoString(gameBoundLeft));
	attributesValues.push_back(BStoString(gameBoundDown));
	attributesValues.push_back(BStoString(gameBoundUp));
	attributesValues.push_back(BStoString(gameBoundRight));
	attributesValues.push_back(BStoString(levelHasScript));

//		saveTheMap->writeAnyXML(theNameOfMap, "Level", "obstacle", nodesName, nodesForXML, attributesName, attributesValues);
	delete saveTheMap;
	aSalvatHarta = true;
}


void BSTexture::salveazaFinishedLevel()
{
	std::string file = "Saves/playerData" + BStoString(levelChoserPlayer->indexOfChapterThatItIsOn + 1) + ".txt";
	std::ifstream fin(file.c_str());

	int chpNr, lvlNr, cuv[25][3];
	fin >> chpNr >> lvlNr;

	int sw = levelChoserPlayer->indexOfLevelThatItIsOn;
	for(int i = 0 ; i < levelChoserPlayer->indexOfLevelThatItIsOn; i++)
	{
		fin >> cuv[i][0] >> cuv[i][1] >> cuv[i][2];
	}
	fin >> cuv[sw][0] >> cuv[sw][1] >> cuv[sw][2];
	cuv[sw][1] = 1;

	for(int i = sw + 1; i < 25; i++)
	{
		fin >> cuv[i][0] >> cuv[i][1] >> cuv[i][2];
	}

	std::ofstream fout(file.c_str());
	fout << chpNr << ' ' << lvlNr << '\n';
	for(int i = 0 ; i < 25; i ++)
	{
		fout << cuv[i][0] << ' ' << cuv[i][1] << ' ' << cuv[i][2] << '\n';
	}
}
