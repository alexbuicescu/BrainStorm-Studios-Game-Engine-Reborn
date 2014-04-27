#pragma once
#ifndef BS_AVAILABLE_SHADERS_H_INCLUDED
#define BS_AVAILABLE_SHADERS_H_INCLUDED

#include <iostream>

class BS_Available_Shaders
{
public:
    virtual ~BS_Available_Shaders() = 0;

    static std::string vertex_position()
    {
        return "BSVertexPosition3D";
    }
    static std::string texture_coordinates()
    {
        return "BSTextureCoordinates";
    }
    static std::string scale_size()
    {
        return "ScaleSize";
    }
    static std::string is_flake()
    {
        return "isFlake";
    }
    static std::string is_circle()
    {
        return "isCircle";
    }
    static std::string is_dark()
    {
        return "isDark";
    }
    static std::string circle_radius()
    {
        return "CircleRadius";
    }
    static std::string texture_color()
    {
        return "BSTextureColor";
    }
    static std::string texture_unit()
    {
        return "BSTextureUnit";
    }
    static std::string projection_matrix()
    {
        return "BSProjectionMatrix";
    }
    static std::string model_view_matrix()
    {
        return "BSModelViewMatrix";
    }
};


#endif // BS_AVAILABLE_SHADERS_H_INCLUDED
