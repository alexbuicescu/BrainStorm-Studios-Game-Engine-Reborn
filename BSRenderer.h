#pragma once
#ifndef BSRENDERER_H
#define BSRENDERER_H
#include "BSStructs.h"
#include "My_Values.h"
#include <SOIL/SOIL.h>
#include <iostream>
#include "BS_Available_Shaders.h"

static inline void renderSomeBody(float coordX, float coordY, float coordZ, float angleX, float angleY, float angleZ,
                                GLuint theTextureIdToRender, BSColorRGBA theColorOfObstacle,
                                GLuint theVaoBuffer, float ScaleOnX, float ScaleOnY, float ScaleOnZ, bool canRotate)
{
	glBindTexture(GL_TEXTURE_2D, theTextureIdToRender);

	BSModelViewMatrix = glm::translate( coordX, coordY, coordZ ) * LookAtMatrix;
	if(canRotate)
    {
        BSModelViewMatrix = glm::rotate( worldAngle, 0.0f, 0.0f, 1.0f ) * BSModelViewMatrix;
    }

	//inversez ordinea matricelor daca vreau sa se invarta in jurul originii scenei
	if(angleX || angleY || angleZ)
	{
	    if(angleX)
        {
            BSModelViewMatrix = BSModelViewMatrix * glm::rotate( angleX, 0.0f, 0.0f, 1.0f );
        }
        if(angleY)
        {
            BSModelViewMatrix = BSModelViewMatrix * glm::rotate( angleY, 0.0f, 1.0f, 0.0f );
        }
        if(angleZ)
        {
            BSModelViewMatrix = BSModelViewMatrix * glm::rotate( angleZ, 1.0f, 0.0f, 0.0f );
        }
	}

    if(ScaleOnX && ScaleOnY)
	{
	    BSModelViewMatrix = BSModelViewMatrix * glm::scale( ScaleOnX, ScaleOnY, 0.0f );
	}
	else
        if(ScaleOnX)
        {
            BSModelViewMatrix = BSModelViewMatrix * glm::scale( ScaleOnX, 1.0f, 0.0f );
        }
        else
            if(ScaleOnY)
            {
                BSModelViewMatrix = BSModelViewMatrix * glm::scale( 1.f, ScaleOnY, 0.0f );
            }


	glUniformMatrix4fv(	_my_shader_program->get_location_from_shader(BS_Available_Shaders::model_view_matrix()), 1, GL_FALSE, glm::value_ptr( BSModelViewMatrix ) );
	_my_shader_program->set_uniform(BS_Available_Shaders::texture_color(), theColorOfObstacle.r, theColorOfObstacle.g, theColorOfObstacle.b, theColorOfObstacle.a);
	//glUniform4f( locationBSTextureColor, theColorOfObstacle.r, theColorOfObstacle.g, theColorOfObstacle.b, theColorOfObstacle.a );

	glBindVertexArray( theVaoBuffer );
	glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


static inline void loadATexture(std::string name, GLuint &theTextureId)
{
	const char *nameChar = name.c_str();
	theTextureId = SOIL_load_OGL_texture
				   (
					   nameChar,
					   SOIL_LOAD_AUTO,
					   SOIL_CREATE_NEW_ID,
					   SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
				   );
	glBindTexture(GL_TEXTURE_2D, theTextureId);
//	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
//	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR);


	if(!theTextureId)
	{
		std::cout<<"texture NOT loaded successfully: "<<name<<'\n';
	}
}

static inline void getTextureWidthAndHeight(std::string name, int &width, int &height)
{
	int channels = 0;
	SOIL_load_image
                   (
                       name.c_str(),
                       &width, &height, &channels,
                       SOIL_LOAD_AUTO
                   );
}

#endif // BSRENDERER_H
