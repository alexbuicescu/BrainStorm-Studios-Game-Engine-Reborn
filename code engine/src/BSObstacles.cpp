#include "BSObstacles.h"
#include "My_Values.h"
#include "BS_Engine.h"

BSAnimationClass *animationsObstacleClass = new BSAnimationClass();

float scaleRatio = (float)(SCREEN_HEIGHT / 18.0f);

b2Body* BSObstacles::createBodies(b2BodyType Type, float LocationX, float LocationY, float Width, float Height,
								  std::string userData, int &NumberOfBodies, int timeUntilIsDistroyed, bool corpDistrus)
{
	b2BodyDef myBodyDef;
	myBodyDef.type = Type;
	USERDATA* userdata = new USERDATA;

//	if(NumberOfBodies == 0)
//	{
//		myBodyDef.bullet = true;
//	}

	myBodyDef.position.Set((float) LocationX, (float) LocationY);

	userdata->name = userData;

	if(userData == "particle")
	{
		userdata->timeUntilDestroy = timeUntilIsDistroyed;
		userdata->BumTime = SDL_GetTicks() + timeUntilIsDistroyed;
	}

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox((float) Width / 2, (float) Height / 2);

	b2Body* box = world->CreateBody(&myBodyDef);

	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &polygonShape;
	myFixtureDef.density = 1;

	box->ResetMassData();
	box->SetFixedRotation(true);

	box->SetUserData(userdata);

	if(userData == "particle")
	{
		box->SetFixedRotation(false);
		myFixtureDef.filter.groupIndex = BinaryParticles;
	}
	else
		if(userData == "hero")
		{
			myFixtureDef.filter.groupIndex = BinaryPlayer;
		}
		else
		{
			myFixtureDef.filter.groupIndex = BinaryScenery;
		}


	box->CreateFixture(&myFixtureDef);

	///This lets you change the precalculated mass of a body to a fixed one, added by you
//            b2MassData massData;
//            box->GetMassData(&massData);
//
//            float scaleFactor = desiredMass / massData.mass;
//            massData.mass *= scaleFactor;
//            massData.I *= scaleFactor;
//
//            box->SetMassData(&massData);
	return box;
}



b2Body* BSObstacles::createCircleBodies(b2BodyType Type, float LocationX, float LocationY, float Radius,
										std::string userData, int &NumberOfBodies, int timeUntilIsDistroyed, bool corpDistrus)
{
	b2BodyDef myBodyDef;
	myBodyDef.type = Type;
	USERDATA* userdata = new USERDATA;

	if(NumberOfBodies == 0)
	{
		myBodyDef.bullet = true;
	}

	myBodyDef.position.Set((float) LocationX, (float) LocationY);

	userdata->name = userData;

	if(userData == "particle")
	{
		userdata->timeUntilDestroy = timeUntilIsDistroyed;
		userdata->BumTime = SDL_GetTicks() + timeUntilIsDistroyed;
	}

	b2CircleShape circleShape;
	circleShape.m_radius = Radius;

	b2Body* box = world->CreateBody(&myBodyDef);

	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &circleShape;
	myFixtureDef.density = 1;

	box->ResetMassData();
	box->SetFixedRotation(true);


	box->SetUserData(userdata);


	if(userData == "particle")
	{
		box->SetFixedRotation(false);
		myFixtureDef.filter.groupIndex = BinaryParticles;
	}
	else
		if(userData == "hero")
		{
			myFixtureDef.filter.groupIndex = BinaryPlayer;
		}
		else
		{
			myFixtureDef.filter.groupIndex = BinaryScenery;
		}


	if(userData == "Hero2nd" || userData == "hero")
	{
		box->SetFixedRotation(false);
		myFixtureDef.filter.groupIndex = BinaryPlayer;
	}

	box->CreateFixture(&myFixtureDef);

	if(corpDistrus == false)
	{
		NumberOfBodies++;
	}

	return box;
}

void BSObstacles::setVaoForChunckObjects(GLuint &vaoBuffer, GLuint &vboBuffer, GLuint &iboBuffer, BSTexturedVertex2D dataToSet[4], GLfloat widthToSet, GLfloat heightToSet)
{
	GLfloat texTop = 1;
	GLfloat texBottom = 0;
	GLfloat texLeft = 0;
	GLfloat texRight = 1;

	//Vertex coordinates
	GLfloat quadWidth = widthToSet / 2.0f;
	GLfloat quadHeight = heightToSet / 2.0f;

	//Texture coordinates
	dataToSet[0].texCoord.s = texLeft;
	dataToSet[0].texCoord.t = texTop;
	dataToSet[1].texCoord.s = texLeft;
	dataToSet[1].texCoord.t = texBottom;
	dataToSet[2].texCoord.s = texRight;
	dataToSet[2].texCoord.t = texBottom;
	dataToSet[3].texCoord.s = texRight;
	dataToSet[3].texCoord.t = texTop;

	//Vertex positions
	dataToSet[0].position.x = -quadWidth;
	dataToSet[0].position.y = -quadHeight;
	dataToSet[0].position.z = scaleSizeInitial;
	dataToSet[1].position.x = -quadWidth;
	dataToSet[1].position.y = quadHeight;
	dataToSet[1].position.z = scaleSizeInitial;
	dataToSet[2].position.x = quadWidth;
	dataToSet[2].position.y = quadHeight;
	dataToSet[2].position.z = scaleSizeInitial;
	dataToSet[3].position.x = quadWidth;
	dataToSet[3].position.y = -quadHeight;
	dataToSet[3].position.z = scaleSizeInitial;

	BS_Renderer::set_vao(vaoBuffer, vboBuffer, iboBuffer, dataToSet);
}

void BSObstacles::set_vao_data_font(BSTexturedVertex2D dataTSet[4], GLfloat widthToSet, GLfloat heightToSet, float xCord, float yCord, GLuint &vaoBuffer, GLuint &vboBuffer, GLuint &iboBuffer)
{
	glGenVertexArrays(1, &vaoBuffer);
	glBindVertexArray(vaoBuffer);

	BSTexturedVertex2D data[4];
	float texBottom = yCord / 1024.f;
	float texTop = (yCord + heightToSet) / 1024.f;
	float texLeft = xCord / 1024.f;
	float texRight = (xCord + widthToSet) / 1024.f;

	//Vertex coordinates
	float quadWidth = 1 / 2.f;
	float quadHeight = 1 / 2.f;
	quadWidth = (float) widthToSet / heightToSet / 2.f;


	//Texture coordinates
	data[0].texCoord.s = texLeft;
	data[0].texCoord.t = texTop;
	data[1].texCoord.s = texLeft;
	data[1].texCoord.t = texBottom;
	data[2].texCoord.s = texRight;
	data[2].texCoord.t = texBottom;
	data[3].texCoord.s = texRight;
	data[3].texCoord.t = texTop;

	//Vertex positions
	data[0].position.x = -quadWidth;
	data[0].position.y = -quadHeight;
	data[0].position.z = scaleSizeInitial;
	data[1].position.x = -quadWidth;
	data[1].position.y = quadHeight;
	data[1].position.z = scaleSizeInitial;
	data[2].position.x = quadWidth;
	data[2].position.y = quadHeight;
	data[2].position.z = scaleSizeInitial;
	data[3].position.x = quadWidth;
	data[3].position.y = -quadHeight;
	data[3].position.z = scaleSizeInitial;




	glGenBuffers(1, &vboBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vboBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &iboBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(BS_Engine::indexes_order), BS_Engine::indexes_order, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(_my_shader_program->get_location_from_shader(BS_Available_Shaders::vertex_position()));//locationBSVertexPosition3D);
	glVertexAttribPointer(_my_shader_program->get_location_from_shader(BS_Available_Shaders::vertex_position()), 3, GL_FLOAT, GL_FALSE, sizeof(BSTexturedVertex2D), 0);

	glEnableVertexAttribArray(_my_shader_program->get_location_from_shader(BS_Available_Shaders::texture_coordinates()));//locationBSTextureCoordinate);
	glVertexAttribPointer(_my_shader_program->get_location_from_shader(BS_Available_Shaders::texture_coordinates()), 2, GL_FLOAT, GL_FALSE, sizeof(BSTexturedVertex2D), (GLvoid*) (sizeof(float) * 3));

	//Unbind VAO
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}



void BSObstacles::setUpBodies()
{
	setUpOriginals();

	incarcaHarta("Menu Of Levels/Chapter1/The Levels objects/level1.xml");
}


void BSObstacles::setUpOriginals()
{
	for(int i = 0 ; i < 5; i++)
	{
		BsActiveAnimations *p = new BsActiveAnimations;
//        listOfActiveAnimations[i]->indexOfAnimation = 0;
//        listOfActiveAnimations[i]->isAtStepOfAnimation1 = false;
//        listOfActiveAnimations[i]->isAtStepOfAnimation2 = false;
//        listOfActiveAnimations[i]->isAtStepOfAnimation3 = false;
//        listOfActiveAnimations[i]->canPlayAnimation1 = false;
//        listOfActiveAnimations[i]->canPlayAnimation2 = false;
//        listOfActiveAnimations[i]->canPlayAnimation3 = false;
		p->canPlayIt = false;

		p->width = 2;
		p->height = 2;
		listOfActiveAnimations.push_back(p);
	}//0-hero, 1-berserk, 2-electricity, 3-wormHole, 4-last power up

	///Set the particle data !!!!!!!! Need initialization only once, vaoBuffer too
	originalParticle->width = 1 / 8.f;
	originalParticle->height = 1 / 8.f;
	BS_Renderer::set_vao_data(originalParticle->vao, originalParticle->vbo, originalParticle->ibo, originalParticle->data, 1  / 16.0f, 1  / 16.0f);

	///Hero2nd
	Hero2nd->theBody = createCircleBodies(b2_dynamicBody, 6, 4, Hero_size, "Hero2nd", nrBodies, 0, false);
	Hero2nd->width = Hero_size + 0.15;
	Hero2nd->height = Hero_size + 0.15;
	Hero2nd->texture = playerTexture;
	Hero2nd->color.setColor(1, 1, 1, 1);
	BS_Renderer::set_vao_data(Hero2nd->vao, Hero2nd->vbo, Hero2nd->ibo, Hero2nd->data, Hero2nd->width, Hero2nd->height);


	///SoundButton texture
	SoundButtonTexture->width = Hero_size;
	SoundButtonTexture->height = Hero_size;
	SoundButtonTexture->texture = playerTexture;
	SoundButtonTexture->color.setColor(1, 1, 1, 1);
	SoundButtonTexture->xCord = (float) SCREEN_WIDTH / scaleRatio / 2;
	SoundButtonTexture->yCord = (float) SCREEN_HEIGHT / scaleRatio / 2;
	BS_Renderer::set_vao_data(SoundButtonTexture->vao, SoundButtonTexture->vbo, SoundButtonTexture->ibo,
				  SoundButtonTexture->data, SoundButtonTexture->width, SoundButtonTexture->height);



	///Set the coin
	BS_Renderer::load_texture("Animations/coin.png", originalCoin->texture);
	originalCoin->width = Hero_size;
	originalCoin->height = Hero_size;
	originalCoin->xCord = 2;
	originalCoin->yCord = (float) SCREEN_HEIGHT / scaleRatio - 1;
	BS_Renderer::set_vao_data(originalCoin->vao, originalCoin->vbo, originalCoin->ibo, originalCoin->data, 1, 1);


	///Set the dust bodies
	BS_Renderer::load_texture("Animations/dust.png", originalDustBody->texture);
	originalDustBody->width = 1;
	originalDustBody->height = 1;
	originalDustBody->xCord = 0;
	originalDustBody->yCord = 0;
	BS_Renderer::set_vao_data(originalDustBody->vao, originalDustBody->vbo, originalDustBody->ibo, originalDustBody->data, 1, 1);


	///Set the fans
	BS_Renderer::load_texture("Animations/fan.png", originalFan->texture);
	originalFan->width = Hero_size * 10;
	originalFan->height = Hero_size;
	BS_Renderer::set_vao_data(originalFan->vao, originalFan->vbo, originalFan->ibo, originalFan->data, 1, 1);
	setVaoForChunckObjects(originalFanParticleX->vao, originalFanParticleX->vbo, originalFanParticleX->ibo, originalFanParticleX->data, 1, 0.05f);
	setVaoForChunckObjects(originalFanParticleY->vao, originalFanParticleY->vbo, originalFanParticleY->ibo, originalFanParticleY->data, 0.05f, 1);


	///Set the dark level light
	BS_Renderer::load_texture("Animations/dark.png", mainDarkTexture);
	originalDarkMenu->width = (float) (SCREEN_WIDTH / scaleRatio + 2) * 2;
	originalDarkMenu->height = (float) (SCREEN_HEIGHT / scaleRatio + 2) * 2;
	BS_Renderer::set_vao_data(originalDarkMenu->vao, originalDarkMenu->vbo, originalDarkMenu->ibo, originalDarkMenu->data, 1, 1);


	originalDarkLevel->width = (float) SCREEN_WIDTH / scaleRatio - Hero_size * 6.f;
	originalDarkLevel->height = (float) SCREEN_HEIGHT / scaleRatio - Hero_size * 6.f;
	BS_Renderer::set_vao_data(originalDarkLevel->vao, originalDarkLevel->vbo, originalDarkLevel->ibo, originalDarkLevel->data, 1, 1);


	originalWormHole->width = 2;
	originalWormHole->height = 2;
	BS_Renderer::set_vao_data(originalWormHole->vao, originalWormHole->vbo, originalWormHole->ibo, originalWormHole->data, 1, 1);
	BS_Renderer::load_texture("Animations/wormHole.png", originalWormHole->texture);
	BSColorRGBA color = {1.0f, 1.0f, 1.0f, 1.0f};
	originalWormHole->color = color;

	hero->heroIsBerserk1 = false;
	hero->heroIsBerserk2 = false;
	hero->heroIsBerserk3 = false;

	///The rendered Square
	theRenderedObstacleSquare->width = hero->width;
	theRenderedObstacleSquare->height = hero->height;
	theRenderedObstacleSquare->color.setColor(1, 0.8f, 0, 0.5f); //transparent yellowish
	theRenderedObstacleSquare->texture = blankTexture;
	BS_Renderer::set_vao_data(theRenderedObstacleSquare->vao, theRenderedObstacleSquare->vbo, theRenderedObstacleSquare->ibo,
				  theRenderedObstacleSquare->data, 1, 1);

	originalTransparentMenu->width = (float) SCREEN_WIDTH / scaleRatio;
	originalTransparentMenu->height = (float) SCREEN_HEIGHT / scaleRatio;
	originalTransparentMenu->color.setColor(1, 0.8f, 0, 0.5f); //transparent yellowish
	originalTransparentMenu->texture = blankTexture;
	BS_Renderer::set_vao_data(originalTransparentMenu->vao, originalTransparentMenu->vbo, originalTransparentMenu->ibo,
				  originalTransparentMenu->data, 1, 1);

	///The rendered Circle
	theRenderedObstacleCircle->width = hero->width;
	theRenderedObstacleCircle->height = hero->height;
	theRenderedObstacleCircle->color.setColor(1, 0.8f, 0, 0.5f); //transparent yellowish
	theRenderedObstacleCircle->texture = blankTexture;
	BS_Renderer::set_vao_data(theRenderedObstacleCircle->vao, theRenderedObstacleCircle->vbo, theRenderedObstacleCircle->ibo,
				  theRenderedObstacleCircle->data, 1, 1);


	///Set the Dead Scene data
	deadScene->width = SCREEN_WIDTH * 1.5f / scaleRatio * 2;
	deadScene->height = SCREEN_HEIGHT * 1.5f / scaleRatio * 2;
	deadScene->color.setColor(0.f, 0.f, 0.f, 0.f);
	deadScene->texture = blankTexture;
	BS_Renderer::set_vao_data(deadScene->vao, deadScene->vbo, deadScene->ibo, deadScene->data, 1, 1);
}





///Black magic, if you try to understand the next method you are going to turn into a frog!!
void BSObstacles::incarcaHarta(std::string cale)
{
	BSXmlFiles *doc = new BSXmlFiles();
//	char theNodes[10][100], theAttributes[6][100];
//	ListaDeCaractere *levelDetails[1000], *levelDetailsAttributes[1];
	std::vector<std::string>theNodes, theAttributes;
	std::vector<ListaDeCaractere*> levelDetails;
	std::vector<std::string> levelDetailsAttributes;

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

	theAttributes.push_back("name");
	theAttributes.push_back("boundary_Left");
	theAttributes.push_back("boundary_Down");
	theAttributes.push_back("boundary_Up");
	theAttributes.push_back("boundary_Right");
	theAttributes.push_back("script");

	doc->readAnyXML(cale, "Level", "obstacle", theNodes, theAttributes, levelDetails, levelDetailsAttributes);


	scaleRatio = cross_platform_scale;

	CameraExtremeLeft = atof(levelDetailsAttributes[1].c_str()) + SCREEN_WIDTH / 2.f / scaleRatio;
	CameraExtremeDown = atof(levelDetailsAttributes[2].c_str()) + SCREEN_HEIGHT / 2.f / scaleRatio;
	CameraExtremeUp = atof(levelDetailsAttributes[3].c_str()) - SCREEN_HEIGHT / 2.f / scaleRatio;
	CameraExtremeRight = atof(levelDetailsAttributes[4].c_str()) - SCREEN_WIDTH / 2.f / scaleRatio;

	gameBoundLeft = atof(levelDetailsAttributes[1].c_str());
	gameBoundDown = atof(levelDetailsAttributes[2].c_str());
	gameBoundUp = atof(levelDetailsAttributes[3].c_str());
	gameBoundRight = atof(levelDetailsAttributes[4].c_str());


	///in case that the level is not big (e.g. the camera doesn't have to move, because all the objects are in the frame)
	if(CameraExtremeUp < CameraExtremeDown)
	{
		CameraExtremeUp = CameraExtremeDown;
	}
	if(CameraExtremeRight < CameraExtremeLeft)
	{
		CameraExtremeRight = CameraExtremeLeft;
	}

	levelHasScript = atof(levelDetailsAttributes[5].c_str());

	std::cout << '\n';

	HeroInitialX = atof(levelDetails[0]->theCharArray2D[3].c_str());
	HeroInitialY = atof(levelDetails[0]->theCharArray2D[4].c_str());


	///Hero
	hero->animationIndexPlayerMoveRight = 0;
	hero->animationIndexPlayerMoveLeft = 1;
	hero->animationIndexPlayerMoveUpRight = 2;
	hero->animationIndexPlayerMoveUpLeft = 3;
	hero->animationIndexPlayerMoveDownRight = 4;
	hero->animationIndexPlayerMoveDownLeft = 5;
	hero->animationIndexPlayerStand = 6;
	hero->animationIndexPlayerDieLeft = 7;
	hero->animationIndexPlayerDieRight = 8;
	hero->animationIndexPlayerTeleportedIn = 9;
	hero->animationIndexPlayerTeleportedOut = 10;
	hero->animationIndexPlayerAura1 = 11;
	hero->animationIndexPlayerAura2 = 12;
	hero->animationIndexPlayerAura3 = 13;
	hero->animationIndexPlayerAuraElectricity = 14;
	hero->animationIndexPlayerWormHole = 15;
	hero->animationIndexPlayerWallLeft = 16;
	hero->animationIndexPlayerWallRight = 17;

	hero->animationIndexCurrentAnimation = 0;

	hero->heroIsDead = false;

	std::ifstream playerSize("playerSize.txt");
	float width, height;
	playerSize >> width >> height;

	if(hero->width != Hero_size && hero->height != Hero_size)
	{
		hero->theBody = createBodies(b2_dynamicBody, HeroInitialX, HeroInitialY,
									 Hero_size - 0.01f, Hero_size - 0.04f,
									 levelDetails[0]->theCharArray2D[7], nrBodies, 0, false);

		playerAnimWidth = Hero_size;
		playerAnimHeight = Hero_size;

		BS_Renderer::set_vao_data(hero->vao, hero->vbo, hero->ibo, hero->data, playerAnimWidth, playerAnimHeight);

		hero->width = Hero_size;
		hero->height = Hero_size;
	}

	///Add the animations
	if(nrOfListOfAnimations == 0)
	{
		addTheAnimationsForMap(levelDetails[0]->theCharArray2D[8]);
	}

	///Sterg mesajele daca exista
	deleteAllMessageTexture();

	int currentInWhile = 1;
	while(currentInWhile < levelDetails.size() && levelDetails[currentInWhile])
	{
		///Daca e patrat
		if(atof(levelDetails[currentInWhile]->theCharArray2D[0].c_str()) == 0)
		{
			addDinamicSquare(atof(levelDetails[currentInWhile]->theCharArray2D[3].c_str()), atof(levelDetails[currentInWhile]->theCharArray2D[4].c_str()),
							 atof(levelDetails[currentInWhile]->theCharArray2D[5].c_str()), atof(levelDetails[currentInWhile]->theCharArray2D[6].c_str()),
							 levelDetails[currentInWhile]->theCharArray2D[1], 0, levelDetails[currentInWhile]->theCharArray2D[7],
							 b2_staticBody, atof(levelDetails[currentInWhile]->theCharArray2D[2].c_str()), 0);

			ListOfSquareBodies.back()->index = atof(levelDetails[currentInWhile]->theCharArray2D[9].c_str());
		}
		else
			///Daca e cerc
			if(atof(levelDetails[currentInWhile]->theCharArray2D[0].c_str()) == 1)
			{
				addDinamicRound(atof(levelDetails[currentInWhile]->theCharArray2D[3].c_str()), atof(levelDetails[currentInWhile]->theCharArray2D[4].c_str()),
								atof(levelDetails[currentInWhile]->theCharArray2D[5].c_str()), levelDetails[currentInWhile]->theCharArray2D[1],
								0, levelDetails[currentInWhile]->theCharArray2D[7],
								b2_staticBody, atof(levelDetails[currentInWhile]->theCharArray2D[5].c_str()));

				ListOfRoundBodies.back()->index = atof(levelDetails[currentInWhile]->theCharArray2D[9].c_str());
			}
			else
				///Daca e mesaj
				if(atof(levelDetails[currentInWhile]->theCharArray2D[0].c_str()) == 2)
				{
					addMessageTexture(atof(levelDetails[currentInWhile]->theCharArray2D[3].c_str()), atof(levelDetails[currentInWhile]->theCharArray2D[4].c_str()),
									  atof(levelDetails[currentInWhile]->theCharArray2D[5].c_str()), atof(levelDetails[currentInWhile]->theCharArray2D[6].c_str()),
									  levelDetails[currentInWhile]->theCharArray2D[1]);

					listOfMessages.back()->index = atof(levelDetails[currentInWhile]->theCharArray2D[9].c_str());
				}
				else
					///Daca e texture square
					if(atof(levelDetails[currentInWhile]->theCharArray2D[0].c_str()) == 3)
					{
						addOnlyTexture(1, atof(levelDetails[currentInWhile]->theCharArray2D[3].c_str()),
									   atof(levelDetails[currentInWhile]->theCharArray2D[4].c_str()),
									   atof(levelDetails[currentInWhile]->theCharArray2D[5].c_str()),
									   atof(levelDetails[currentInWhile]->theCharArray2D[6].c_str()),
									   levelDetails[currentInWhile]->theCharArray2D[1]);

						ListOfSquareTextures.back()->index = atof(levelDetails[currentInWhile]->theCharArray2D[9].c_str());
					}
					else
						///Daca e texture circle
						if(atof(levelDetails[currentInWhile]->theCharArray2D[0].c_str()) == 4)
						{
							addOnlyTexture(-1, atof(levelDetails[currentInWhile]->theCharArray2D[3].c_str()),
										   atof(levelDetails[currentInWhile]->theCharArray2D[4].c_str()),
										   atof(levelDetails[currentInWhile]->theCharArray2D[5].c_str()),
										   atof(levelDetails[currentInWhile]->theCharArray2D[6].c_str()),
										   levelDetails[currentInWhile]->theCharArray2D[1].c_str());

							ListOfRoundTextures.back()->index = atof(levelDetails[currentInWhile]->theCharArray2D[9].c_str());
						}
						else
							///Daca e coin
							if(atof(levelDetails[currentInWhile]->theCharArray2D[0].c_str()) == 5)
							{
								addCoin(atof(levelDetails[currentInWhile]->theCharArray2D[3].c_str()),
										atof(levelDetails[currentInWhile]->theCharArray2D[4].c_str()),
										atof(levelDetails[currentInWhile]->theCharArray2D[8].c_str()));

								listOfCoins.back()->index = atof(levelDetails[currentInWhile]->theCharArray2D[9].c_str());
							}
							else
								///Daca e fan
								if(atof(levelDetails[currentInWhile]->theCharArray2D[0].c_str()) == 6)
								{
									std::string details = levelDetails[currentInWhile]->theCharArray2D[7];
									char length[100] = "", powerX[100] = "", powerY[100] = "";
									int nrPauze = 0, kLength = 0, kPowerX = 0, kPowerY = 0;

									for(unsigned int i = 0 ; i < details.size(); i++)
									{
										///Length
										if(!nrPauze)
										{
											length[kLength] = details[i];
											kLength++;
										}
										else

											///PowerX
											if(nrPauze == 1)
											{
												powerX[kPowerX] = details[i];
												kPowerX++;
											}
											else

												///PowerY
												if(nrPauze == 2)
												{
													powerY[kPowerY] = details[i];
													kPowerY++;
												}

										if(details[i] == ' ')
										{
											nrPauze++;
										}
									}

									addFan(atof(levelDetails[currentInWhile]->theCharArray2D[3].c_str()),
										   atof(levelDetails[currentInWhile]->theCharArray2D[4].c_str()),
										   atof(levelDetails[currentInWhile]->theCharArray2D[5].c_str()),
										   atof(levelDetails[currentInWhile]->theCharArray2D[6].c_str()),
										   atof(length), atof(powerX), atof(powerY));

									listOfFans.back()->index = atof(levelDetails[currentInWhile]->theCharArray2D[9].c_str());
								}
								else
									///Daca e worm hole
									if(atof(levelDetails[currentInWhile]->theCharArray2D[0].c_str()) == 7)
									{
										addWormHole(atof(levelDetails[currentInWhile]->theCharArray2D[7].c_str()),
													atof(levelDetails[currentInWhile]->theCharArray2D[8].c_str()),
													atof(levelDetails[currentInWhile]->theCharArray2D[3].c_str()),
													atof(levelDetails[currentInWhile]->theCharArray2D[4].c_str()));

										listOfWormHoles.back()->index = atof(levelDetails[currentInWhile]->theCharArray2D[9].c_str());
									}
									else
										///Daca e kinematic square
										if(atof(levelDetails[currentInWhile]->theCharArray2D[0].c_str()) == 8)
										{
											addDinamicSquare(atof(levelDetails[currentInWhile]->theCharArray2D[3].c_str()),
															 atof(levelDetails[currentInWhile]->theCharArray2D[4].c_str()),
															 atof(levelDetails[currentInWhile]->theCharArray2D[5].c_str()),
															 atof(levelDetails[currentInWhile]->theCharArray2D[6].c_str()),
															 levelDetails[currentInWhile]->theCharArray2D[1], 0,
															 levelDetails[currentInWhile]->theCharArray2D[7],
															 b2_kinematicBody, atof(levelDetails[currentInWhile]->theCharArray2D[2].c_str()), 0);

											ListOfSquareBodies.back()->index = atof(levelDetails[currentInWhile]->theCharArray2D[9].c_str());
										}
										else
											///Daca e kinematic circle
											if(atof(levelDetails[currentInWhile]->theCharArray2D[0].c_str()) == 9)
											{
												addDinamicRound(atof(levelDetails[currentInWhile]->theCharArray2D[3].c_str()),
																atof(levelDetails[currentInWhile]->theCharArray2D[4].c_str()),
																atof(levelDetails[currentInWhile]->theCharArray2D[5].c_str()),
																levelDetails[currentInWhile]->theCharArray2D[1],
																0, levelDetails[currentInWhile]->theCharArray2D[7],
																b2_kinematicBody, atof(levelDetails[currentInWhile]->theCharArray2D[2].c_str()));

												ListOfRoundBodies.back()->index = atof(levelDetails[currentInWhile]->theCharArray2D[9].c_str());
											}
											else
												///Daca e dust body
												if(atof(levelDetails[currentInWhile]->theCharArray2D[0].c_str()) == 10)
												{
													addDinamicSquare(atof(levelDetails[currentInWhile]->theCharArray2D[3].c_str()),
																	 atof(levelDetails[currentInWhile]->theCharArray2D[4].c_str()),
																	 atof(levelDetails[currentInWhile]->theCharArray2D[5].c_str()),
																	 atof(levelDetails[currentInWhile]->theCharArray2D[6].c_str()),
																	 levelDetails[currentInWhile]->theCharArray2D[1], 0,
																	 levelDetails[currentInWhile]->theCharArray2D[7],
																	 b2_staticBody, atof(levelDetails[currentInWhile]->theCharArray2D[2].c_str()), 1);

													ListOfSquareDustBodies.back()->index = atof(levelDetails[currentInWhile]->theCharArray2D[9].c_str());
												}
		delete levelDetails[currentInWhile];
		currentInWhile++;
	}

	hero->theBody->SetTransform(b2Vec2(HeroInitialX, HeroInitialY), hero->theBody->GetAngle());
	hero->theBody->ApplyLinearImpulse(b2Vec2(0.1, 0.1), hero->theBody->GetWorldCenter());

	delete doc;
	levelDetailsAttributes.clear();
	theNodes.clear();
	theAttributes.clear();
	levelDetails.clear();

	if(levelHasScript)
	{
		addScript(cale);///change nume- add "-script" in its name!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!(do it in the function)
	}

	backgroundSky->xInit = (CameraExtremeRight + CameraExtremeLeft) / 2.0f ;
	backgroundSky->yInit = (CameraExtremeUp + CameraExtremeDown) / 2.0f ;

	backgroundMountain->xInit = (CameraExtremeRight + CameraExtremeLeft) / 2.0f ;
	backgroundMountain->yInit = CameraExtremeDown - SCREEN_HEIGHT / 64 / 2.0f ;

	backgroundBush->xInit = (CameraExtremeLeft - SCREEN_WIDTH / 2.0 / 64) + backgroundBush->width / 2.0f;
	backgroundBush->yInit = CameraExtremeDown - SCREEN_HEIGHT / 64 / 2.0f ;

	backgroundHighField->xInit = (CameraExtremeRight + SCREEN_WIDTH / 2.0 / 64) - backgroundHighField->width / 2.0f;
	backgroundHighField->yInit = CameraExtremeDown - SCREEN_HEIGHT / 64 / 2.0f ;

	std::cout << '\n' << "added objects" << '\n';
}


void BSObstacles::addScript(std::string Path)
{
	///set the actual path of the script.
	///if the path of the level is: Level/level1.xml then the path of the script is: Level/level1-script.xml
	std::string newPath = "";
	for(unsigned int i = 0; i < Path.size() - 4; i++)
	{
		newPath += Path[i];
	}
	Path = newPath + "-script.xml";

	BSXmlFiles *doc = new BSXmlFiles();
//	char theNodes[7][100];
//	ListaDeCaractere *levelDetails[1000];
	std::vector<std::string> theNodes;
	std::vector<std::string> empty;
	std::vector<ListaDeCaractere*> levelDetails;
	theNodes.push_back("Index");
	theNodes.push_back("xPower");
	theNodes.push_back("yPower");
	theNodes.push_back("xLength");
	theNodes.push_back("yLength");
	theNodes.push_back("xCoordinate");
	theNodes.push_back("yCoordinate");

	doc->readAnyXML(Path, "Level", "obstacle", theNodes, empty, levelDetails, empty);

	int i = 0;

	while(i < levelDetails.size() && levelDetails[i])
	{
		int index = atof(levelDetails[i]->theCharArray2D[0].c_str());
		bool founded = false;
		///search the scripted body in the squareBody list
		for(int j = 0; j < ListOfSquareBodies.size(); j++)
		{
			if(ListOfSquareBodies[j]->index == index)
			{
				founded = true;

				BSBodyScript *newScript = new BSBodyScript;
				newScript->xCord = atof(levelDetails[i]->theCharArray2D[5].c_str());
				newScript->xInitial = newScript->xCord;
				newScript->yCord = atof(levelDetails[i]->theCharArray2D[6].c_str());
				newScript->yInitial = newScript->yCord;
				newScript->xPower = atof(levelDetails[i]->theCharArray2D[1].c_str());
				newScript->yPower = atof(levelDetails[i]->theCharArray2D[2].c_str());
				newScript->xLength = atof(levelDetails[i]->theCharArray2D[3].c_str());
				newScript->yLength = atof(levelDetails[i]->theCharArray2D[4].c_str());
				newScript->value1 = 1;
				newScript->value2 = 90;

				ListOfSquareBodies[j]->script = newScript;

				break;
			}
		}
		///daca nu l-a gasit in lista de squareBody atunci e in cea de roundBodies
		if(!founded)
		{
			for(int j = 0; j < ListOfRoundBodies.size(); j++)
			{
				if(ListOfRoundBodies[j]->index == index)
				{
					founded = true;

					BSBodyScript *newScript = new BSBodyScript;
					newScript->xCord = atof(levelDetails[i]->theCharArray2D[5].c_str());
					newScript->xInitial = newScript->xCord;
					newScript->yCord = atof(levelDetails[i]->theCharArray2D[6].c_str());
					newScript->yInitial = newScript->yCord;
					newScript->xPower = atof(levelDetails[i]->theCharArray2D[1].c_str());
					newScript->yPower = atof(levelDetails[i]->theCharArray2D[2].c_str());
					newScript->xLength = atof(levelDetails[i]->theCharArray2D[3].c_str());
					newScript->yLength = atof(levelDetails[i]->theCharArray2D[4].c_str());
					newScript->value1 = 1;
					newScript->value2 = 90;

					ListOfRoundBodies[j]->script = newScript;

					break;
				}
			}
		}

		std::cout << "new script: " << index << ' ' << levelDetails[i]->theCharArray2D[5] << ' ' <<
				  levelDetails[i]->theCharArray2D[6] << '\n';

		delete levelDetails[i];
		i++;
	}
	delete doc;
}







void BSObstacles::addCoin(float x, float y, int taken)
{
	BSCoins *newCoin = new BSCoins;
	newCoin->color.setColor(1, 1, 1, 1);
	newCoin->width = Hero_size;// / 2.f;
	newCoin->height = Hero_size;// / 2.f;
	newCoin->sinValue = 90;
	newCoin->deleteIt = false;
	newCoin->xCord = x;
	newCoin->yCord = y;
	newCoin->xInit = x;
	newCoin->yInit = y;
	newCoin->xAngle = 0;
	newCoin->yAngle = 0;
	newCoin->zAngle = 0;

	newCoin->available = taken;

	///daca a fost taken (not available) inseamna ca deja o are
	if(taken == false)
	{
		newCoin->alreadyHadIt = true;
		newCoin->sinValue = 0;
		newCoin->deleteIt = true;
		newCoin->color.setColor(1, 1, 1, 0);
	}
	else
	{
		newCoin->alreadyHadIt = false;
	}

	listOfCoins.push_back(newCoin);
}

void BSObstacles::addFan(float x, float y, float width, float height, float length, float powerX, float powerY)
{
	BSFans *newFan = new BSFans;
	newFan->color.setColor(1, 1, 1, 1);
	newFan->width = width;
	newFan->height = height;
	newFan->xCord = x;
	newFan->yCord = y;
	newFan->powerX = powerX;
	newFan->powerY = powerY;
	newFan->lenthOfPower = length;
	newFan->nrOfParticles = 100;
	listOfFans.push_back(newFan);

	for(int i = 0; i < newFan->nrOfParticles; i++)
	{
		BSBody *newPart = new BSBody;
		BSColorRGBA black = {0, 0, 0, 1};
		newPart->color = black;
		float maximX, minimX, maximY, minimY;

		///daca e pe orizontala
		if(powerX)
		{
			newPart->width = 1;
			newPart->height = 0.05f;
			///daca bate vantul spre dreapta
			if(powerX > 0)
			{
				maximX = x + length;
				minimX = x;
				///salvez viteza particulelor pozitiva pe orizontala
				newPart->value1 = (rand() % 3 + 1 ) / 10.0f;
			}
			else
			{
				maximX = x;
				minimX = x - length;
				///salvez viteza particulelor negativa pe orizontala
				newPart->value1 = (rand() % 3 + 1) * (-1) / 10.0f;
			}

			maximY = y + (height / 2.0f);
			minimY = y - (height / 2.0f);
			///viteza pe verticala este 0
			newPart->value2 = 0;
		}
		else
		{
			newPart->width = 0.1f;
			newPart->height = 1;
			maximX = x + (width / 2.0f);
			minimX = x - (width / 2.0f);

			if(powerY > 0)
			{
				maximY = y + length;
				minimY = y;
				newPart->value2 = (rand() % 3 + 1 ) / 10.0f;
			}
			else
			{
				maximY = y;
				minimY = y - length;
				newPart->value2 = (rand() % 3 + 1) * (-1) / 10.0f;
			}
			newPart->value1 = 0;
		}

		float posX = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximX - minimX + 1) + minimX;
		float posY = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximY - minimY + 1) + minimY;

		newFan->xMax = maximX;
		newFan->yMax = maximY;
		newFan->xMin = minimX;
		newFan->yMin = minimY;

		newPart->xCord = posX;
		newPart->yCord = posY;
		newFan->particles.push_back(newPart);
	}
}






void BSObstacles::setTheMenuButtons(BSButtonStructure* theButtonToSet, BSTexturedVertex2D dataToSet[4],
									float coordinateX, float coordinateY, float ToWidth, float ToHeight,
									GLuint vaoData, GLuint vboData, GLuint iboData)
{
	BS_Renderer::set_vao_data(vaoData, vboData, iboData, dataToSet, ToWidth, ToHeight);
}





void BSObstacles::addTheAnimationsForMap(std::string theLocationOfAnimation)
{
	std::string newAnimationDetailPath = "Animations/" + theLocationOfAnimation + "/animations.txt";
	std::string detailName = "";

	std::ifstream animationsDetails(newAnimationDetailPath.c_str());
	int totalNrAnimations, total1, total2, total3, totalTime1, totalTime2, totalTime3, totalLoop1, totalLoop2, totalLoop3;

	animationsDetails >> totalNrAnimations;

	for(int i = 0 ; i < totalNrAnimations ; i++)
	{
		animationsDetails >> total1 >> total2 >> total3 >> totalTime1 >> totalTime2 >> totalTime3 >> totalLoop1 >> totalLoop2 >> totalLoop3 >> detailName;

		std::string newAnimationCale = "Animations/" + theLocationOfAnimation + "/" + detailName + "/playerTexture";

		int animationIndex = 0;

		animationsObstacleClass->addAnimation(total1, total2, total3, totalTime1, totalTime2, totalTime3, totalLoop1, totalLoop2, totalLoop3, i, newAnimationCale);
	}
	nrOfListOfAnimations = 1;
}

void BSObstacles::loadAnimationList(std::string path, int firstPart, int secondPart, int thirdPart)
{
	for(int i = 0 ; i < firstPart ; i++)
	{

	}
}



void BSObstacles::addDinamicSquare(float BodyX, float BodyY, float BodyW, float BodyH,
								   std::string textureName, GLuint vao, std::string userData,
								   b2BodyType tipulCorpului, int canKillHero, int isDust)
{
	std::cout << "new square Body: " << canKillHero << " " << BodyX << " " << BodyY << " " << BodyW << " " << BodyH << " " << userData << '\n';
	BSTheSquareBodies *p = new BSTheSquareBodies;

	p->theBody = createBodies(tipulCorpului, BodyX, BodyY, BodyW, BodyH, userData, nrBodies, 0, false);
	BS_Renderer::set_vao_data(p->vao, p->vbo, p->ibo, p->data, BodyW, BodyH);

	p->xCord = BodyX;
	p->xInit = BodyX;
	p->yCord = BodyY;
	p->yInit = BodyY;

	p->width = BodyW;
	p->height = BodyH;
	p->script = NULL;
	p->nameOfTexture = textureName;

	BS_Renderer::load_texture(textureName, p->texture);

	p->color.setColor(1, 1, 1, 1);

	p->value1 = 0;
	p->value2 = 0;

	USERDATA* dataForUser = new USERDATA;

	dataForUser->isDustAble = isDust;
	dataForUser->isDust = 0;

	if(canKillHero == 2)
	{
		dataForUser->canKillHero = false;
		dataForUser->isToNextLevel = true;
		p->ObstacleCanKillHero = false;
		p->ObstacleFinishesLevel = true;
	}
	else
	{
		dataForUser->canKillHero = canKillHero;
		dataForUser->isToNextLevel = false;
		p->ObstacleCanKillHero = canKillHero;
		p->ObstacleFinishesLevel = false;
	}

	dataForUser->name = userData;
	p->theBody->SetUserData(dataForUser);

	if(!isDust)
	{
		ListOfSquareBodies.push_back(p);
	}
	else
	{
		ListOfSquareDustBodies.push_back(p);
	}
}

void BSObstacles::addDinamicRound(float BodyX, float BodyY, float BodyR,
								  std::string textureName, GLuint vao, std::string userData,
								  b2BodyType tipulCorpului, int canKillHero)
{
	std::cout << "new round Body: " << canKillHero << " " << BodyX << " " << BodyY << " " << BodyR << " " << userData << '\n';
	BSTheRoundBodies *p = new BSTheRoundBodies;

	p->theBody = createCircleBodies(tipulCorpului, BodyX, BodyY, BodyR / 2, userData, nrBodies, 0, false);
	BS_Renderer::set_vao_data(p->vao, p->vbo, p->ibo, p->data, BodyR, BodyR);
	p->width = BodyR;
	p->height = BodyR;
	p->nameOfTexture = textureName;
	p->script = NULL;

	BS_Renderer::load_texture(textureName, p->texture);

	p->color.setColor(1, 1, 1, 1);
	p->isRotating = true;

	USERDATA* dataForUser = new USERDATA;

	if(canKillHero == 2)
	{
		dataForUser->canKillHero = false;
		dataForUser->isToNextLevel = true;
		p->ObstacleCanKillHero = false;
		p->ObstacleFinishesLevel = true;
	}
	else
	{
		dataForUser->canKillHero = canKillHero;
		dataForUser->isToNextLevel = false;
		p->ObstacleCanKillHero = canKillHero;
		p->ObstacleFinishesLevel = false;
	}

	dataForUser->name = userData;
	p->theBody->SetUserData(dataForUser);

	ListOfRoundBodies.push_back(p);
}






void BSObstacles::addOnlyTexture(int squareOrCirlce, float xCoord, float yCoord,
								 float width, float height, std::string texturePath)
{
	if(squareOrCirlce == 1)
	{
		BSTextureSquare *p = new BSTextureSquare;
		BS_Renderer::set_vao_data(p->vao, p->vbo, p->ibo, p->data, width, height);
		p->width = width;
		p->height = height;
		p->xCord = xCoord;
		p->yCord = yCoord;
		p->nameOfTexture = texturePath;
		p->color.setColor(1, 1, 1, 1);

		char textureCharPath[100] = "";
		strcpy(textureCharPath, texturePath.c_str());
		BS_Renderer::load_texture(textureCharPath, p->texture);

		///Salvez primul corp din lista inlantuita
		ListOfSquareTextures.push_back(p);
	}
	else
		if(squareOrCirlce == -1)
		{
			std::cout << "from obstacles: added a round" << '\n';
			BSTextureRound *p = new BSTextureRound;
			BS_Renderer::set_vao_data(p->vao, p->vbo, p->ibo, p->data, width, width);
			p->width = width;
			p->height = width;
			p->xCord = xCoord;
			p->yCord = yCoord;
			p->nameOfTexture = texturePath;
			p->color.setColor(1, 1, 1, 1);

			BS_Renderer::load_texture(texturePath.c_str(), p->texture);

			///Salvez primul corp din lista inlantuita
			ListOfRoundTextures.push_back(p);
		}
}




void BSObstacles::addMessageTexture(float xCoord, float yCoord, float width, float height, std::string textureName)
{
	BSTheMessages* newMessage = new BSTheMessages;
	std::cout << "animation added: " << textureName << '\n';
	///Load the message texture

	BS_Renderer::load_texture(textureName, newMessage->textureOfMessage);
	int newWidth, newHeight;
	BS_Renderer::getTextureWidthAndHeight(textureName, newWidth, newHeight);
	newMessage->widthOfMessage = newWidth / scaleRatio;
	newMessage->heightOfMessage = newHeight / scaleRatio;

	BS_Renderer::set_vao_data(newMessage->vaoBufferOfMessage, newMessage->vboBufferOfMessage, newMessage->iboBufferOfMessage,
				  newMessage->dataOfMessage, newMessage->widthOfMessage, newMessage->heightOfMessage);

	newMessage->colorOfMessage.setColor(1, 1, 1, 0);
	newMessage->isShownTheMessage = false;

	newMessage->nameOfTextureMessage = textureName;

	newMessage->xCord = xCoord;
	newMessage->yCord = yCoord;

	///Load the message sign texture
	std::string fullTextureName = "Animations/messageAnimation/theMessageSign.png";

	BS_Renderer::load_texture(fullTextureName.c_str(), newMessage->textureOfSign);
	BS_Renderer::set_vao_data(newMessage->vaoBufferOfSign, newMessage->vboBufferOfSign, newMessage->iboBufferOfSign,
				  newMessage->dataOfSign, Hero_size, Hero_size * 2.0f);

	newMessage->widthOfSign = Hero_size;
	newMessage->heightOfSign = Hero_size * 1.5f;
	newMessage->colorOfSign.setColor(1, 1, 1, 1);

	listOfMessages.push_back(newMessage);
	nrOfMessageSign++;
}



void BSObstacles::addSnowFlakeS()
{
	float maximX = CameraExtremeRight + (SCREEN_WIDTH / scaleRatio / 2.0f);
	float minimX = CameraExtremeLeft - (SCREEN_WIDTH / scaleRatio / 2.0f);

	float maximY = CameraExtremeUp + (SCREEN_HEIGHT / scaleRatio / 2.0f);
	float minimY = CameraExtremeDown - (SCREEN_HEIGHT / scaleRatio / 4.0f);

	float posX, posY;///, xSpeedMin, xSpeedMax, ySpeedMin, ySpeedMax, xSpeed, ySpeed;
	int leftOrRight;

	for(int i = 0 ; i < 150; i++)
	{
		BSSnowFlakes *newFlake = new BSSnowFlakes;

		posX = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximX - minimX + 1) + minimX;
		posY = ((float)rand() / ((float)RAND_MAX + 1.0)) * (maximY - minimY + 1) + minimY;

		newFlake->xCord = posX;
		newFlake->yCord = posY;

		newFlake->sinValue = 90;
		newFlake->color.setColor(0, 0, 0, 1);

		if(rand() % 2)
		{
			leftOrRight = 1;
		}
		else
		{
			leftOrRight = -1;
		}

		newFlake->width = float ((rand() % 4 ) / 10.0f);
		newFlake->height = float ((rand() % 4 ) / 10.0f);
		newFlake->xPower = float (rand() % 4 + 2) / 1000 * leftOrRight;
		newFlake->yPower = float (rand() % 10 + 2) / 1000;

		setVaoForChunckObjects(newFlake->vao, newFlake->vbo, newFlake->ibo, newFlake->data, newFlake->width, newFlake->width);

		listOfSnowFlakes.push_back(newFlake);
	}
}


void BSObstacles::createMouseCircles()
{
	float lastR = 0.2;
	for(int i = 1 ; i < 15 ; i++)
	{
		BSWorldOfGooMouse *circ = new BSWorldOfGooMouse;

		circ->width = lastR;
		circ->height = lastR;
		circ->xCord = 0;
		circ->yCord = 0;
		BS_Renderer::set_vao_data(circ->vao, circ->vbo, circ->ibo, circ->data, circ->width, circ->width);
		listOfMouseCircles.push_back(circ);

		lastR += 0.03f;
	}
}

void BSObstacles::addBackgrounds()
{
	float ratioImage = 1.77f; ///Screen_width / Screen_height
	BSColorRGBA color = {1, 1, 1, 1};

	backgroundSky->height = (float) SCREEN_HEIGHT / scaleRatio;
	backgroundSky->width = (float) backgroundSky->height * ratioImage;
	backgroundSky->color = color;
	BS_Renderer::load_texture("Background images/sky.png", backgroundSky->texture);
	BS_Renderer::set_vao_data(backgroundSky->vao, backgroundSky->vbo, backgroundSky->ibo,
				  backgroundSky->data, backgroundSky->width, backgroundSky->height);

	backgroundMountain->height = (float) SCREEN_HEIGHT / scaleRatio;
	backgroundMountain->width = (float) backgroundMountain->height * ratioImage;
	backgroundMountain->color = color;
	BS_Renderer::load_texture("Background images/mountains.png", backgroundMountain->texture);
	BS_Renderer::set_vao_data(backgroundMountain->vao, backgroundMountain->vbo, backgroundMountain->ibo,
				  backgroundMountain->data, backgroundMountain->width, backgroundMountain->height);

	backgroundBush->height = (float) SCREEN_HEIGHT / scaleRatio;
	backgroundBush->width = (float) backgroundBush->height * ratioImage;
	backgroundBush->color = color;
	BS_Renderer::load_texture("Background images/bush.png", backgroundBush->texture);
	BS_Renderer::set_vao_data(backgroundBush->vao, backgroundBush->vbo, backgroundBush->ibo,
				  backgroundBush->data, backgroundBush->width, backgroundBush->height);

	backgroundField->height = (float) SCREEN_HEIGHT / scaleRatio;
	backgroundField->width = (float) backgroundField->height * ratioImage;
	backgroundField->color = color;
	BS_Renderer::load_texture("Background images/field.png", backgroundField->texture);
	BS_Renderer::set_vao_data(backgroundField->vao, backgroundField->vbo, backgroundField->ibo,
				  backgroundField->data, backgroundField->width, backgroundField->height);

	backgroundHighField->height = (float) SCREEN_HEIGHT / scaleRatio;
	backgroundHighField->width = (float) backgroundHighField->height * ratioImage;
	backgroundHighField->color = color;
	BS_Renderer::load_texture("Background images/high field.png", backgroundHighField->texture);
	BS_Renderer::set_vao_data(backgroundHighField->vao, backgroundHighField->vbo, backgroundHighField->ibo,
				  backgroundHighField->data, backgroundHighField->width, backgroundHighField->height);
}


void BSObstacles::addCloudyParticles()
{
	addTheTexturesOfCloudyParticles();


	float minimSize = 3;
	float maximSize = 4;

	int leftOrRight = 0;
	for(int i = 0; i < 10; i++)
	{
		BSCloudyParticles *part = new BSCloudyParticles;
		part->color.setColor(1, 1, 1, 1);
		int randomul = rand() % 5 + 1;

		if(randomul % 2)
		{
			leftOrRight = 1;
		}
		else
		{
			leftOrRight = -1;
		}

		part->texture = clouds[randomul];

		part->width = (float) ((rand() / ((float)RAND_MAX + 1.0)) * (maximSize - minimSize + 1) + minimSize) / 10.0f;///float ((rand() % 10 ) / 10.0f);
		part->height = part->width;///float ((rand() % 10 ) / 10.0f);///((float)rand() / ((float)RAND_MAX + 1.0)) * (maximSize - minimSize + 1) + minimSize;
		part->xPower = float (rand() % 4 + 2) / 1000 * leftOrRight;
		part->yPower = float (rand() % 10 + 2) / 1000 * (-1);

		BS_Renderer::set_vao_data(part->vao, part->vbo, part->ibo, part->data, part->width, part->height);

		listOfCloudyParticles.push_back(part);
	}
}




void BSObstacles::addWormHole(int index, int connectionIndex, float x, float y)
{
	std::cout << "new worm hole: " << x << " " << y << " " << index << " " << connectionIndex << '\n';
	BSWormHole *newWorm = new BSWormHole;
	newWorm->value1 = 0;
	newWorm->xCord = x;
	newWorm->yCord = y;
	newWorm->indexOfWorm = index;
	newWorm->connectionIndex = connectionIndex;

	listOfWormHoles.push_back(newWorm);
}

void BSObstacles::deleteAllWormHoles()
{
	while(!listOfWormHoles.empty())
	{
		delete listOfWormHoles.back();
		listOfWormHoles.pop_back();
	}
}





void BSObstacles::addTheTexturesOfCloudyParticles()
{
	for(int i = 1; i <= 5; i++)
	{
		BS_Renderer::load_texture("Background images/cloud" + BStoString(i) + ".png", clouds[i]);
	}
}







void BSObstacles::destroyLevel()
{
	destroyAllBodiesSquare();
	destroyAllBodiesRound();
	destroyAllBodiesDustSquare();
	destroyAllCoins();
	destroyAllFans();
	deleteAllWormHoles();
}

void BSObstacles::destroyAllBodiesDustSquare()
{
	//system("PAUSE");
	while(!ListOfSquareDustBodies.empty())
	{
		BSTheSquareBodies *deletedBody = (BSTheSquareBodies*)ListOfSquareDustBodies.back();

		glDeleteBuffers( 1, &deletedBody->vbo);
		glDeleteBuffers( 1, &deletedBody->ibo);
		glDeleteTextures(1, &deletedBody->texture);
		glDeleteVertexArrays(1, &deletedBody->vao);

		if(deletedBody->theBody)
		{
			world->DestroyBody(deletedBody->theBody);
		}

		delete ListOfSquareDustBodies.back();
		ListOfSquareDustBodies.pop_back();
	}
}

void BSObstacles::destroyAllBodiesSquare()
{
	while(!ListOfSquareBodies.empty())
	{
		BSTheSquareBodies *deletedBody = (BSTheSquareBodies*)ListOfSquareBodies.back();

		glDeleteBuffers( 1, &deletedBody->vbo);
		glDeleteBuffers( 1, &deletedBody->ibo);
		glDeleteTextures(1, &deletedBody->texture);
		glDeleteVertexArrays(1, &deletedBody->vao);

		delete deletedBody->theBody->GetUserData();

		world->DestroyBody(deletedBody->theBody);

		if(deletedBody->script)
		{
			delete deletedBody->script;
		}

		delete deletedBody;
		ListOfSquareBodies.pop_back();
	}
}

void BSObstacles::destroyAllBodiesRound()
{
	while(!ListOfRoundBodies.empty())
	{
		BSTheRoundBodies *deletedBody = (BSTheRoundBodies*)ListOfRoundBodies.back();

		glDeleteBuffers( 1, &deletedBody->vbo);
		glDeleteBuffers( 1, &deletedBody->ibo);
		glDeleteTextures(1, &deletedBody->texture);
		glDeleteVertexArrays(1, &deletedBody->vao);

		delete deletedBody->theBody->GetUserData();

		world->DestroyBody(deletedBody->theBody);

		if(deletedBody->script)
		{
			delete deletedBody->script;
		}

		delete ListOfRoundBodies.back();
		ListOfRoundBodies.pop_back();
	}
}

void BSObstacles::deleteAllMessageTexture()
{
	while(!listOfMessages.empty())
	{
		BSTheMessages *deletedBody = (BSTheMessages*)listOfMessages.back();

		glDeleteBuffers( 1, &deletedBody->vboBufferOfSign);
		glDeleteBuffers( 1, &deletedBody->vboBufferOfMessage);
		glDeleteBuffers( 1, &deletedBody->iboBufferOfMessage);
		glDeleteBuffers( 1, &deletedBody->iboBufferOfSign);
		glDeleteTextures(1, &deletedBody->textureOfMessage);
		glDeleteVertexArrays(1, &deletedBody->vaoBufferOfMessage);
		glDeleteTextures(1, &deletedBody->textureOfSign);
		glDeleteVertexArrays(1, &deletedBody->vaoBufferOfSign);

		delete listOfMessages.back();
		listOfMessages.pop_back();
	}

	nrOfMessageSign = 0;
}

//static bool deleteAll( BSTheSquareBodies* theElement )
//{
//    delete theElement;
//    return true;
//}

void BSObstacles::destroyAllCoins()
{
	while(!listOfCoins.empty())
	{
		delete listOfCoins.back();
		listOfCoins.pop_back();
	}
}

void BSObstacles::destroyAllFans()
{
	while(!listOfFans.empty())
	{
		std::vector<BSBody*> theList = ((BSFans*)(listOfFans.back()))->particles;
		while(!theList.empty())
		{
			delete theList.back();
			theList.pop_back();
		}
		delete listOfFans.back();
		listOfFans.pop_back();
	}
}

void BSObstacles::destroyAllAnimations()
{
	///First, go through every list and delete the animations
	for(int i = 0; i < nrOfListOfAnimations; i++)
	{
		///Deleting the textures and VAOs of every animations
		///First list of animations
		for(int j = 0; j < listOfAnimations[i]->listForAnimations1.size(); j++)
		{
			glDeleteBuffers(1, &listOfAnimations[i]->listForAnimations1[j]->vbo);
			glDeleteBuffers(1, &listOfAnimations[i]->listForAnimations1[j]->ibo);
			glDeleteTextures(1, &listOfAnimations[i]->listForAnimations1[j]->texture);
			glDeleteVertexArrays(1, &listOfAnimations[i]->listForAnimations1[j]->vao);
		}

		///Emptying the list of animations for each animation List
		while(!listOfAnimations[i]->listForAnimations1.empty())
		{
			delete listOfAnimations[i]->listForAnimations1.back();
			listOfAnimations[i]->listForAnimations1.pop_back();
		}

		///Second list of animations
		for(int j = 0; j < listOfAnimations[i]->listForAnimations2.size(); j++)
		{
			glDeleteBuffers(1, &listOfAnimations[i]->listForAnimations2[j]->vbo);
			glDeleteBuffers(1, &listOfAnimations[i]->listForAnimations2[j]->ibo);
			glDeleteTextures(1, &listOfAnimations[i]->listForAnimations2[j]->texture);
			glDeleteVertexArrays(1, &listOfAnimations[i]->listForAnimations2[j]->vao);
		}

		///Emptying the list of animations for each animation List
		while(!listOfAnimations[i]->listForAnimations2.empty())
		{
			delete listOfAnimations[i]->listForAnimations2.back();
			listOfAnimations[i]->listForAnimations2.pop_back();
		}

		///Third list of animations
		for(int j = 0; j < listOfAnimations[i]->listForAnimations3.size(); j++)
		{
			glDeleteBuffers(1, &listOfAnimations[i]->listForAnimations3[j]->vbo);
			glDeleteBuffers(1, &listOfAnimations[i]->listForAnimations3[j]->ibo);
			glDeleteTextures(1, &listOfAnimations[i]->listForAnimations3[j]->texture);
			glDeleteVertexArrays(1, &listOfAnimations[i]->listForAnimations3[j]->vao);
		}

		///Emptying the list of animations for each animation List
		while(!listOfAnimations[i]->listForAnimations3.empty())
		{
			delete listOfAnimations[i]->listForAnimations3.back();
			listOfAnimations[i]->listForAnimations3.pop_back();
		}
	}

	///Emptying the list of animations
	while(nrOfListOfAnimations > 0)
	{
		delete listOfAnimations[nrOfListOfAnimations - 1];
		nrOfListOfAnimations--;
	}
}
