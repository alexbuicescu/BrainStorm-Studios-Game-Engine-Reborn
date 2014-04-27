#pragma once
#ifndef OBSTACLES_H_INCLUDED
#define OBSTACLES_H_INCLUDED

#include <iostream>
#include "BSValues.h"
#include "BSAnimationClass.h"
#include "BSXmlFiles.h"
#include <fstream>

class BSObstacles
{
public:

	b2Body* createBodies(b2BodyType, float, float, float, float, std::string, int &NumberOfBodies
						 , int timeUntilIsDestroyed, bool corpDistrus);


	b2Body* createCircleBodies(b2BodyType Type, float LocationX, float LocationY, float Radius,
							   std::string userData, int &NumberOfBodies, int timeUntilIsDistroyed, bool corpDistrus);

	void setUpBodies();
	void setUpOriginals();

	void incarcaHarta(std::string cale);
	void addScript(std::string Path);
	void destroyLevel();


	void addCoin(float x, float y, int taken);
	void addFan(float x, float y, float width, float height, float length, float powerX, float powerY);
	void addWormHole(int index, int connectionIndex, float x, float y);
	void loadFans(std::string nameOfFile);
	void destroyAllCoins();
	void destroyAllFans();
	void deleteAllWormHoles();


//	void setTheVaoData(BSTexturedVertex2D dataToSet[4], GLfloat widthToSet, GLfloat heightToSet);
	void setTheVaoData(GLuint &vaoBuffer, GLuint &vboBuffer, GLuint &iboBuffer, BSTexturedVertex2D dataToSet[4], GLfloat widthToSet, GLfloat heightToSet);
	void setVaoForChunckObjects(GLuint &vaoBuffer, GLuint &vboBuffer, GLuint &iboBuffer, BSTexturedVertex2D dataToSet[4], GLfloat widthToSet, GLfloat heightToSet);

	void setTheVao( GLuint &theVaoBuffer, BSTexturedVertex2D dataToBind[4], GLuint &theVboToBind, GLuint &theIbotoBind);
	void bindVBO(BSTexturedVertex2D dataToBind[4], GLuint &theVboToBind, GLuint &theIbotoBind);

	void setTheVaoDataForFont(BSTexturedVertex2D dataTSet[4], GLfloat widthToSet, GLfloat heightToSet, float xCord, float yCord, GLuint &vaoBuffer, GLuint &vboBuffer, GLuint &iboBuffer);

	void setTheMenuButtons(BSButtonStructure* theButtonToSet, BSTexturedVertex2D dataToSet[4],
						   float coordinateX, float coordinateY,
						   float ToWidth, float ToHeight,
						   GLuint vaoData, GLuint vboData, GLuint iboData);


	void addTheAnimationsForMap(std::string theLocationOfAnimation);

	void destroyAllBodiesDustSquare();
	void destroyAllAnimations();
	void destroyAllBodiesSquare();
	void destroyAllBodiesRound();

	void loadAnimationList(std::string path, int firstPart, int secondPart, int thirdPart);



	void addDinamicSquare(float BodyX, float BodyY, float BodyW, float BodyH,
						  std::string textureName, GLuint BodyVaoBuffer,
						  std::string userData, b2BodyType tipulCorpului, int canKillHero, int isDust);

	void addDinamicRound(float BodyX, float BodyY, float BodyR,
						 std::string textureName, GLuint BodyVaoBuffer,
						 std::string userData, b2BodyType tipulCorpului, int canKillHero);



	void deleteBodySquare(BSTheSquareBodies* &corpulCareVreauSters);


	void deleteBodyRound(BSTheRoundBodies* &corpulCareVreauSters);

	void addAnimation(std::string name, int nrOfAnimations);

	void addOnlyTexture(int squareOrCirlce, float xCoord, float yCoord, float width, float height, std::string texturePath);
	void deleteOnlyTexture(float xCoord, float yCoord, float width, float height);




	void addMessageTexture(float xCoord, float yCoord, float width, float height, std::string textureName);
	void deleteAllMessageTexture();

	void addSnowFlakeS();
	void createMouseCircles();
	void addBackgrounds();


	void addCloudyParticles();
	void addTheTexturesOfCloudyParticles();

	void loadTheLoadingImage();


};
#endif // OBSTACLES_H_INCLUDED
