#pragma once
#ifndef BSRENDERER_H
#define BSRENDERER_H
#include "BSStructs.h"
#include "My_Values.h"
#include "BSValues.h"
#include "BS_Engine.h"
#include <SOIL/SOIL.h>
#include <iostream>
#include "BS_Available_Shaders.h"


class BS_Renderer
{
public:

	static void set_vao(GLuint &theVaoBuffer, BSTexturedVertex2D dataToBind[4], GLuint &theVboToBind, GLuint &theIbotoBind)
	{
		//Generate VAO buffer
		glGenVertexArrays( 1, &theVaoBuffer );

		//Bind vertex array
		glBindVertexArray( theVaoBuffer );

		//bind the VBO
		bind_vbo(dataToBind, theVboToBind, theIbotoBind);

		glEnableVertexAttribArray(_my_shader_program->get_location_from_shader(BS_Available_Shaders::vertex_position()));// locationBSVertexPosition3D);
		glVertexAttribPointer( _my_shader_program->get_location_from_shader(BS_Available_Shaders::vertex_position()), 3, GL_FLOAT, GL_FALSE, sizeof(BSTexturedVertex2D), (GLvoid*)offsetof( BSTexturedVertex2D, position ) );

		glEnableVertexAttribArray(_my_shader_program->get_location_from_shader(BS_Available_Shaders::texture_coordinates()));//locationBSTextureCoordinate);
		glVertexAttribPointer( _my_shader_program->get_location_from_shader(BS_Available_Shaders::texture_coordinates()), 2, GL_FLOAT, GL_FALSE, sizeof(BSTexturedVertex2D), (GLvoid*)offsetof( BSTexturedVertex2D, texCoord ) );

		//Unbind VAO
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}


	static void bind_vbo(BSTexturedVertex2D dataToBind[4], GLuint &theVboToBind, GLuint &theIbotoBind)
	{
	    ///GLuint iData[4] = {0, 1, 2, 3};
		//Create VBO
		glGenBuffers( 1, &theVboToBind );
		glBindBuffer( GL_ARRAY_BUFFER, theVboToBind );
		glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(BSTexturedVertex2D), dataToBind, GL_DYNAMIC_DRAW );

		//Create IBO
		glGenBuffers( 1, &theIbotoBind );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, theIbotoBind );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), BS_Engine::indexes_order, GL_DYNAMIC_DRAW );
	}


	static void set_vao_data(GLuint &vaoBuffer, GLuint &vboBuffer, GLuint &iboBuffer,
									BSTexturedVertex2D dataToSet[4], GLfloat widthToSet, GLfloat heightToSet)
	{
		widthToSet = 1 / 2.0f;
		heightToSet = 1 / 2.0f;


		GLfloat texTop = 1;
		GLfloat texBottom = 0;
		GLfloat texLeft = 0;
		GLfloat texRight = 1;

		//Vertex coordinates
		GLfloat quadWidth = widthToSet;
		GLfloat quadHeight = heightToSet;

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

		set_vao(vaoBuffer, dataToSet, vboBuffer, iboBuffer);
	}



	static void renderSomeBody(float coordX, float coordY, float coordZ, float angleX, float angleY, float angleZ,
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


	static void loadATexture(std::string name, GLuint &theTextureId)
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
			std::cout << "texture NOT loaded successfully: " << name << '\n';
		}
	}

	static GLuint loadATexture(std::string name)
	{
	    GLuint theTextureId;
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
            std::cout << "texture NOT loaded successfully: " << name << '\n';
            return 0;
		}
		else
        {
            return theTextureId;
        }
	}

	static void getTextureWidthAndHeight(std::string name, int &width, int &height)
	{
		int channels = 0;
		SOIL_load_image
		(
			name.c_str(),
			&width, &height, &channels,
			SOIL_LOAD_AUTO
		);
	}
};
#endif // BSRENDERER_H
