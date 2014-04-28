#include "BS_Camera.h"


BS_Camera::BS_Camera()
{
    set_coordinates(0, 0, 0);
}

BS_Camera::BS_Camera(float _coordinate_x, float _coordinate_y, float _coordinate_z)
{
    set_coordinates(_coordinate_x, _coordinate_y, _coordinate_z);
}

void BS_Camera::move_camera(float _coordinate_x, float _coordinate_y, float _coordinate_z)
{
    set_coordinates(_coordinate_x, _coordinate_y, _coordinate_z);
}

void BS_Camera::update_camera()
{
	matrix_position = glm::vec3(get_coordinates('x'), get_coordinates('y'), get_coordinates('z'));
	LookAtMatrix = glm::lookAt(matrix_position, matrix_position + matrix_direction, matrix_up);
}
