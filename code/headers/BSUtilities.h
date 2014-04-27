#pragma once
#ifndef BSUTILITIES_H
#define BSUTILITIES_H

#include "BSOpenGL.h"
#include <stdio.h>
#include <iostream>
#include "BSValues.h"
#include <fstream>
#include "BSObstacles.h"
#include "BSTexturedPolygonProgram2D.h"
#include "BSTexture.h"
#include "BSLevelMenu.h"
#include "BSXmlFiles.h"


class BSUtilities
{
public:

	bool initGL();

	bool loadGP();

	bool loadMedia();
	void loadCoinsAndDeaths();

	void update();

	void render();

	void keyboardHandling();
};

#endif
