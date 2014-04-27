#pragma once
#ifndef BSSTRUCTS_H
#define BSSTRUCTS_H

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <Box2D/Box2D.h>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int SCREEN_FPS;
extern int SCREEN_BPP;
extern float scaleSize;
extern int showBackgr, isFullScreen;
#define Hero_size 1
#define scaleSizeInitial -11.3

extern GLint locationBSVertexPosition3D, locationBSTextureCoordinate, locationBSModelViewMatrix, locationBSProjectionMatrix;
extern GLint locationBSTextureColor, locationBSTextureUnit;
extern GLuint BSProgramID;

extern glm::mat4 BSProjectionMatrix;
extern glm::mat4 BSModelViewMatrix;
extern glm::mat4 LookAtMatrix;

extern float cross_platform_scale;


extern float worldAngle;
extern b2Vec2 worldGravity;
extern int gravityIsChanging, startAngle;

struct BSTexCoord
{
	GLfloat s;
	GLfloat t;
};

struct BSRectangle
{
	GLfloat x;
	GLfloat y;
	GLfloat w;
	GLfloat h;
};

struct BSColorRGBA
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;

	void setColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	void setColorsTo1()
	{
		r = 1;
		g = 1;
		b = 1;
		a = 1;
	}
};

struct BSVertexPos2D
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct BSTexturedVertex2D
{
	BSVertexPos2D position;
	BSTexCoord texCoord;
};


#endif // BSSTRUCTS_H
