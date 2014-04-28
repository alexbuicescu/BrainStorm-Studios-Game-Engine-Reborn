#include "BS_Object.h"
#include "BSRenderer.h"



BS_Object::BS_Object(float _coordinate_x, float _coordinate_y, float _coordinate_z, float _width, float _height, float _depth, float _rotation_angle_x, float _rotation_angle_y, float _rotation_angle_z)
{
	set_coordinates(_coordinate_x, _coordinate_y, _coordinate_z);
	set_size(_width, _height, _depth);
	set_rotation_angle(_rotation_angle_x, _rotation_angle_y, _rotation_angle_z);

	set_color(1, 1, 1, 1);

    BS_Renderer::set_vao_data(vao, vbo, ibo, data, width, height);
}

BS_Object::BS_Object(float _coordinate_x, float _coordinate_y, float _width, float _height, std::string _texture_name)
{
	set_coordinates(_coordinate_x, _coordinate_y);
	set_size(_width, _height);

	set_color(1, 1, 1, 1);

	BS_Engine::load_new_texture(_texture_name);

    BS_Renderer::set_vao_data(vao, vbo, ibo, data, width, height);
}

void BS_Object::set_coordinates(float _coordinate_x, float _coordinate_y, float _coordinate_z)
{
	coordinate_x = _coordinate_x;
	coordinate_y = _coordinate_y;
	coordinate_z = _coordinate_z;
}
void BS_Object::set_size(float _width, float _height, float _depth)
{
	width = _width;
	height = _height;
	depth = _depth;
}
void BS_Object::set_rotation_angle(float _rotation_angle_x, float _rotation_angle_y, float _rotation_angle_z)
{
	rotation_angle_x = _rotation_angle_x;
	rotation_angle_y = _rotation_angle_y;
	rotation_angle_z = _rotation_angle_z;
}
void BS_Object::set_color(float red, float green, float blue, float alpha)
{
    color.r = red;
    color.g = green;
    color.b = blue;
    color.a = alpha;
}


float BS_Object::get_coordinates(char _type)
{
	if(_type == 'x')
	{
		return coordinate_x;
	}

	if(_type == 'y')
	{
		return coordinate_y;
	}

	if(_type == 'z')
	{
		return coordinate_z;
	}

	return 0;
}

float BS_Object::get_size(char _type)
{
	if(_type == 'w')
	{
		return width;
	}

	if(_type == 'h')
	{
		return height;
	}

	if(_type == 'd')
	{
		return depth;
	}

	return 0;
}

float BS_Object::get_rotation_angle(char _type)
{
	if(_type == 'x')
	{
		return rotation_angle_x;
	}

	if(_type == 'y')
	{
		return rotation_angle_y;
	}

	if(_type == 'z')
	{
		return rotation_angle_z;
	}

	return 0;
}

float BS_Object::get_color(char _type)
{
	if(_type == 'r')
	{
		return color.r;
	}

	if(_type == 'g')
	{
		return color.g;
	}

	if(_type == 'b')
	{
		return color.b;
	}

	if(_type == 'a')
	{
		return color.a;
	}

	return 0;
}

void BS_Object::draw()
{
	BS_Renderer::renderSomeBody(coordinate_x, coordinate_y, coordinate_z, rotation_angle_x, rotation_angle_y, rotation_angle_z, BS_Engine::get_texture(texture_name), color, vao, width, height, depth, false);
}

void BS_Object::draw(GLuint differentTexture)
{
	BS_Renderer::renderSomeBody(coordinate_x, coordinate_y, coordinate_z, rotation_angle_x, rotation_angle_y, rotation_angle_z, differentTexture, color, vao, width, height, depth, false);
}

void BS_Object::draw(GLuint differentTexture, GLuint differentVao)
{
	BS_Renderer::renderSomeBody(coordinate_x, coordinate_y, coordinate_z, rotation_angle_x, rotation_angle_y, rotation_angle_z, differentTexture, color, differentVao, width, height, depth, false);
}
