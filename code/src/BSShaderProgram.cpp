/*
 * Some of the code from this class was taken from the Lazy-Foo tutorials, so all copyrights reserved to Lazy-Foo. There was no point in changing or adding
 * the code, because the only thing it does is some error checking and the shader compiling.
 */
#include "BSShaderProgram.h"
#include "BSValues.h"
#include <fstream>

BSShaderProgram::BSShaderProgram()
{
	BSProgramID = 0;
}

BSShaderProgram::~BSShaderProgram()
{
	//Free program if it exists
	freeProgram();
}

void BSShaderProgram::freeProgram()
{
	//Delete program
	glDeleteProgram( BSProgramID );
}

bool BSShaderProgram::bind()
{
	//Use shader
	glUseProgram( BSProgramID );

	//Check for error
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error binding shader! %s\n", gluErrorString( error ) );
		printProgramLog( BSProgramID );
		return false;
	}

	return true;
}

void BSShaderProgram::unbind()
{
	//Use default program
	glUseProgram( 0 );
}

GLuint BSShaderProgram::getProgramID()
{
	return BSProgramID;
}

void BSShaderProgram::printProgramLog( GLuint program )
{
	//Make sure name is shader
	if( glIsProgram( program ) )
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

		//Allocate string
		char* infoLog = new char[ maxLength ];

		//Get info log
		glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf( "Name %d is not a program\n", program );
	}
}

void BSShaderProgram::printShaderLog( GLuint shader )
{
	//Make sure name is shader
	if( glIsShader( shader ) )
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

		//Allocate string
		char* infoLog = new char[ maxLength ];

		//Get info log
		glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf( "Name %d is not a shader\n", shader );
	}
}

GLuint BSShaderProgram::loadShaderFromFile( std::string path, GLenum shaderType )
{
	//Open file
	GLuint shaderID = 0;
	std::string shaderString;
	std::ifstream sourceFile( path.c_str() );

	//Source file loaded
	if( sourceFile )
	{
		//Get shader source
		shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );

		//Create shader ID
		shaderID = glCreateShader( shaderType );

		//Set shader source
		const GLchar* shaderSource = shaderString.c_str();
		glShaderSource( shaderID, 1, (const GLchar**)&shaderSource, NULL );

		//Compile shader source
		glCompileShader( shaderID );

		//Check shader for errors
		GLint shaderCompiled = GL_FALSE;
		glGetShaderiv( shaderID, GL_COMPILE_STATUS, &shaderCompiled );
		if( shaderCompiled != GL_TRUE )
		{
			printf( "Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource );
			printShaderLog( shaderID );
			glDeleteShader( shaderID );
			shaderID = 0;
		}
	}
	else
	{
		printf( "Unable to open file %s\n", path.c_str() );
	}

	return shaderID;
}

