#pragma once
#ifndef ACTIONSTUFF_H_INCLUDED
#define ACTIONSTUFF_H_INCLUDED

#include <iostream>
#include <fstream>
#include "BSValues.h"
#include "BSObstacles.h"
#include "BSTexture.h"
#include "BSUtilities.h"
#include <Box2D/Box2D.h>
#include <AL/al.h>
#include <AL/alc.h>


class BSActionStuff
{
public:
	//circle that closes on the player when he finishes the level
	void levelFinished();
	void moveThePlayer();

	void moveRightOnGround(b2Body *body);
	void moveLeftOnGround();

	void moveRightInAir();

	void controlAir(float T, b2Body *body);
	void controlWall(b2Body *body);
	void controlCorner(b2Body *body);
	inline bool itWalks(b2Body *body, float T);
	bool itWalksInAir(b2Body *body, float T);
	void controlGround(b2Body *body, float T);


	void generateTheParticles();
	void destroyTheParticles();
	void generateWhenHitsGroundParticles();


	void ClickOnBlock();

	void TeleportHero(float coordX, float coordY);
	void ActionFuncTeleportHero(float xCord, float yCord);

	void PathColoring(int LeftOrRight);

	void heroTouchesObjects();
	bool CanGoAwayFromWall(float timeToMatch);


	static float easeIn(float t, float b , float c, float d);
	static float easeOut(float t, float b , float c, float d);
	static float easeInOut(float t, float b , float c, float d);

	float radical(float x);
};
#endif // ACTIONSTUFF_H_INCLUDED
