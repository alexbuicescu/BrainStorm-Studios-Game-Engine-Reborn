#pragma once
#ifndef BSRENDERER_H
#define BSRENDERER_H


#include <iostream>
#include "BSStructs.h"


class BS_Renderer
{
	static void bind_vbo(GLuint &theVboToBind, GLuint &theIbotoBind, BSTexturedVertex2D dataToBind[4]);
public:

    ///remove set_vao from set_vao_for_font and you can make it private
	static void set_vao(GLuint &theVaoBuffer, GLuint &theVboToBind, GLuint &theIbotoBind, BSTexturedVertex2D dataToBind[4]);
	static void set_vao_data(GLuint &vaoBuffer, GLuint &vboBuffer, GLuint &iboBuffer, BSTexturedVertex2D dataToSet[4], GLfloat widthToSet, GLfloat heightToSet);

	static void render_object(float coordX, float coordY, float coordZ, float angleX, float angleY, float angleZ,
							   GLuint theTextureIdToRender, BSColorRGBA theColorOfObstacle,
							   GLuint theVaoBuffer, float ScaleOnX, float ScaleOnY, float ScaleOnZ, bool canRotate);

	static void load_texture(std::string name, GLuint &theTextureId);
	static GLuint load_texture(std::string name);
	static void getTextureWidthAndHeight(std::string name, int &width, int &height);
};
#endif // BSRENDERER_H
