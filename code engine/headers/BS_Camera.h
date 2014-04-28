#pragma once
#ifndef BS_CAMERA_H_INCLUDED
#define BS_CAMERA_H_INCLUDED

#include "BS_Object.h"

class BS_Camera : public BS_Object
{
private:
    glm::vec3 matrix_position, matrix_direction = glm::vec3(0, 0, -1), matrix_up = glm::vec3(0, 1, 0);
public:
glm::mat4 LookAtMatrix;

    BS_Camera();
    BS_Camera(float _coordinate_x, float _coordinate_y, float _coordinate_z);

    void move_camera(float _coordinate_x, float _coordinate_y, float _coordinate_z);
    void update_camera();
};

#endif // BS_CAMERA_H_INCLUDED
