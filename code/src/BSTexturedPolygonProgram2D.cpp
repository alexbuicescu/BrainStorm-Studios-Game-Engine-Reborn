#include "BSTexturedPolygonProgram2D.h"
#include "BS_Available_Shaders.h"

BSTexturedPolygonProgram2D::BSTexturedPolygonProgram2D()
{
}

GLint BSTexturedPolygonProgram2D::get_location_from_shader(std::string _name)
{
    return locations_shader_values[_name];
}

void BSTexturedPolygonProgram2D::set_uniform(std::string _value_shader_name, float _value)
{
	glUniform1f(locations_shader_values[_value_shader_name], _value);
}

void BSTexturedPolygonProgram2D::set_uniform(std::string _value_shader_name, float _value1, float _value2)
{
	glUniform2f(locations_shader_values[_value_shader_name], _value1, _value2);
}

void BSTexturedPolygonProgram2D::set_uniform(std::string _value_shader_name, float _value1, float _value2, float _value3)
{
	glUniform3f(locations_shader_values[_value_shader_name], _value1, _value2, _value3);
}

void BSTexturedPolygonProgram2D::set_uniform(std::string _value_shader_name, float _value1, float _value2, float _value3, float _value4)
{
	glUniform4f(locations_shader_values[_value_shader_name], _value1, _value2, _value3, _value4);
}

bool BSTexturedPolygonProgram2D::loadProgram(std::string _path_vertex_shader, std::string _path_fragment_shader)
{
	//Generate program
	BSProgramID = glCreateProgram();

	//Load vertex shader
	GLuint vertexShader = loadShaderFromFile( _path_vertex_shader, GL_VERTEX_SHADER );

    //Check for errors
    if( vertexShader == 0 )
    {
        glDeleteProgram( BSProgramID );
        BSProgramID = 0;
        return false;
    }

	//Attach vertex shader to program
	glAttachShader( BSProgramID, vertexShader );

	//Create fragment shader
	GLuint fragmentShader = loadShaderFromFile( _path_fragment_shader, GL_FRAGMENT_SHADER );

    //Check for errors
    if( fragmentShader == 0 )
    {
        glDeleteProgram( BSProgramID );
        BSProgramID = 0;
        return false;
    }

	//Attach fragment shader to program
	glAttachShader( BSProgramID, fragmentShader );

	//Link program
    glLinkProgram( BSProgramID );

	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv( BSProgramID, GL_LINK_STATUS, &programSuccess );
	if( programSuccess != GL_TRUE )
    {
		printf( "Error linking program %d!\n", BSProgramID );
		printProgramLog( BSProgramID );
        glDeleteProgram( BSProgramID );
        BSProgramID = 0;
        return false;
    }

	load_Shader_Attributes_Location(BS_Available_Shaders::vertex_position());
	load_Shader_Attributes_Location(BS_Available_Shaders::texture_coordinates());

    load_Shader_Uniforms_Location(BS_Available_Shaders::scale_size());

    load_Shader_Uniforms_Location(BS_Available_Shaders::is_flake());
    load_Shader_Uniforms_Location(BS_Available_Shaders::is_circle());
    load_Shader_Uniforms_Location(BS_Available_Shaders::is_dark());

    load_Shader_Uniforms_Location(BS_Available_Shaders::circle_radius());

    load_Shader_Uniforms_Location(BS_Available_Shaders::texture_color());
    load_Shader_Uniforms_Location(BS_Available_Shaders::texture_unit());

    load_Shader_Uniforms_Location(BS_Available_Shaders::projection_matrix());
    load_Shader_Uniforms_Location(BS_Available_Shaders::model_view_matrix());

	return true;
}



void BSTexturedPolygonProgram2D::load_Shader_Attributes_Location(std::string theShaderAttributeText)
{
	locations_shader_values[theShaderAttributeText] = glGetAttribLocation( BSProgramID, theShaderAttributeText.c_str() );
	if( locations_shader_values[theShaderAttributeText] == -1 )
	{
		printf( "%s is not a valid glsl program variable (attribute, maybe not used)!\n", theShaderAttributeText.c_str() );
	}
}

void BSTexturedPolygonProgram2D::load_Shader_Uniforms_Location(std::string theShaderUniformText)
{
	locations_shader_values[theShaderUniformText] = glGetUniformLocation( BSProgramID, theShaderUniformText.c_str() );
	if( locations_shader_values[theShaderUniformText] == -1 )
	{
		printf( "%s is not a valid glsl program variable (uniform, maybe not used)!\n", theShaderUniformText.c_str() );
	}
}
