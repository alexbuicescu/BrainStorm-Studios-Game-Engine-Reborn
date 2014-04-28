#pragma once
#ifndef BS_OBJECT_H_INCLUDED
#define BS_OBJECT_H_INCLUDED

#include "BSStructs.h"

class BS_Object
{
private:
	float coordinate_x = 0, coordinate_y = 0, coordinate_z = 0;
	float width = 0, height = 0, depth = 0;
	float rotation_angle_x = 0, rotation_angle_y = 0, rotation_angle_z = 0;
	BSColorRGBA color;
	int index = 0;
	BSTexturedVertex2D data[4];
	GLuint texture = 0, vao = 0, vbo = 0, ibo = 0;
	std::string texture_name = "";

	void set_vao_data();
	void set_vao();
	void bind_vbo();

public:

    BS_Object();

	BS_Object(float _coordinate_x, float _coordinate_y, float _coordinate_z, float _width, float _height, float _depth, float _rotation_angle_x, float _rotation_angle_y, float _rotation_angle_z);

	BS_Object(float _coordinate_x, float _coordinate_y, float _width, float _height, std::string _texture_name);

	void set_coordinates(float _coordinate_x, float _coordinate_y, float _coordinate_z = 0);
	void set_size(float _width, float _height, float _depth = 0);
	void set_rotation_angle(float _rotation_angle_x, float _rotation_angle_y, float _rotation_angle_z = 0);
	void set_color(float red, float green, float blue, float alpha);

	float get_coordinates(char _type);
	float get_size(char _type);
	float get_rotation_angle(char _type);
	float get_color(char _type);


	void draw();
	void draw(GLuint differentTexture);
	void draw(GLuint differentTexture, GLuint differentVao);
};


#endif // BS_OBJECT_H_INCLUDED
