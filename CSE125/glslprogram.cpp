#include "glslprogram.h"

#include "glutils.h"

#include <fstream>
using std::ifstream;
using std::ios;

#include <sstream>
using std::ostringstream;

#include <sys/stat.h>

const bool PRINT_ERROR = false;

GLSLProgram::GLSLProgram() : handle(0), linked(false) { }

bool GLSLProgram::compileShaderFromFile( const char * fileName,
                                         GLSLShader::GLSLShaderType type )
{
    if( ! fileExists(fileName) )
    {
        logString = "File not found.";
        return false;
    }

    if( handle <= 0 ) {
        handle = glCreateProgram();
        if( handle == 0) {
            logString = "Unable to create shader program.";
            return false;
        }
    }

    ifstream inFile( fileName, ios::in );
    if( !inFile ) {
        return false;
    }

    ostringstream code;
    while( inFile.good() ) {
        int c = inFile.get();
        if( ! inFile.eof() ) code << (char) c;
    }
    inFile.close();

    return compileShaderFromString(code.str(), type);
}

bool GLSLProgram::compileShaderFromString( const string & source, GLSLShader::GLSLShaderType type )
{
    if( handle <= 0 ) {
        handle = glCreateProgram();
        if( handle == 0) {
            logString = "Unable to create shader program.";
            return false;
        }
    }

    GLuint shaderHandle = 0;

    switch( type ) {
    case GLSLShader::VERTEX:
        shaderHandle = glCreateShader(GL_VERTEX_SHADER);
        break;
    case GLSLShader::FRAGMENT:
        shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    case GLSLShader::GEOMETRY:
        shaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
        break;
    case GLSLShader::TESS_CONTROL:
        shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
        break;
    case GLSLShader::TESS_EVALUATION:
        shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
        break;
    default:
        return false;
    }

    const char * c_code = source.c_str();
    glShaderSource( shaderHandle, 1, &c_code, NULL );

    // Compile the shader
    glCompileShader(shaderHandle );

    // Check for errors
    int result;
    glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result ) {
        // Compile failed, store log and return false
        int length = 0;
        logString = "";
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length );
        if( length > 0 ) {
            char * c_log = new char[length];
            int written = 0;
            glGetShaderInfoLog(shaderHandle, length, &written, c_log);
            logString = c_log;
			printf("GLSL compile error:\n %s\n",logString);
            delete [] c_log;
        }

        return false;
    } else {
        // Compile succeeded, attach shader and return true
        glAttachShader(handle, shaderHandle);
        return true;
    }
}

bool GLSLProgram::link()
{
    if( linked ) return true;
    if( handle <= 0 ) return false;

    glLinkProgram(handle);

    int status = 0;
    glGetProgramiv( handle, GL_LINK_STATUS, &status);
    if( GL_FALSE == status ) {
        // Store log and return false
        int length = 0;
        logString = "";

        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 ) {
            char * c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog(handle, length, &written, c_log);
            logString = c_log;
			printf("GLSL link error:\n %s\n",logString);
            delete [] c_log;
        }

        return false;
    } else {
        linked = true;
        return linked;
    }
}

void GLSLProgram::use()
{
    if( handle <= 0 || (! linked) ) return;
    glUseProgram( handle );
}

string GLSLProgram::log()
{
    return logString;
}

int GLSLProgram::getHandle()
{
    return handle;
}

bool GLSLProgram::isLinked()
{
    return linked;
}

void GLSLProgram::bindAttribLocation( GLuint location, const char * name)
{
    glBindAttribLocation(handle, location, name);
}

void GLSLProgram::bindFragDataLocation( GLuint location, const char * name )
{
    glBindFragDataLocation(handle, location, name);
}

void GLSLProgram::setUniform( const char *name, float x, float y, float z)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ) {
        glProgramUniform3f(handle,loc,x,y,z);
	}
	else if (PRINT_ERROR)
	{
		fprintf(stderr, "Uniform variable %s not found!\n",name);
	}
}

void GLSLProgram::setUniform( const char *name, const vec2 & v )
{
	int loc = getUniformLocation(name);
	if( loc >= 0 ) {
		glProgramUniform2f(handle,loc,v.x,v.y);
	}
	else if (PRINT_ERROR)
	{
		fprintf(stderr, "Uniform variable %s not found!\n",name);
	}
}

void GLSLProgram::setUniform( const char *name, const vec3 & v)
{
    this->setUniform(name,v.x,v.y,v.z);
}

void GLSLProgram::setUniform( const char *name, const vec4 & v)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ) {
        glProgramUniform4f(handle,loc,v.x,v.y,v.z,v.w);
	}
	else if (PRINT_ERROR)
	{
		fprintf(stderr, "Uniform variable %s not found!\n",name);
	}
}

void GLSLProgram::setUniform( const char *name, const mat4 & m)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glProgramUniformMatrix4fv(handle,loc, 1, GL_FALSE, &m[0][0]);
	}
	else if (PRINT_ERROR)
	{
		fprintf(stderr, "Uniform variable %s not found!\n",name);
	}
}

void GLSLProgram::setUniform( const char *name, const mat3 & m)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glProgramUniformMatrix3fv(handle,loc, 1, GL_FALSE, &m[0][0]);
	}
	else if (PRINT_ERROR)
	{
		fprintf(stderr, "Uniform variable %s not found!\n",name);
	}
}

void GLSLProgram::setUniform( const char *name, float val )
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glProgramUniform1f(handle,loc, val);
	}
	else if (PRINT_ERROR)
	{
		fprintf(stderr, "Uniform variable %s not found!\n",name);
	}
}

void GLSLProgram::setUniform( const char *name, int val )
{
	int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glProgramUniform1i(handle,loc, val);
	}
	else if (PRINT_ERROR)
	{
		fprintf(stderr, "Uniform variable %s not found!\n",name);
	}
}

void GLSLProgram::setUniform( const char *name, bool val )
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glProgramUniform1i(handle,loc, val);
	}
	else if (PRINT_ERROR)
	{
		fprintf(stderr, "Uniform variable %s not found!\n",name);
	}
}

void GLSLProgram::printActiveUniforms() {

    GLint nUniforms, size, location, maxLen;
    GLchar * name;
    GLsizei written;
    GLenum type;

    glGetProgramiv( handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
    glGetProgramiv( handle, GL_ACTIVE_UNIFORMS, &nUniforms);

    name = (GLchar *) malloc( maxLen );

    printf(" Location | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nUniforms; ++i ) {
        glGetActiveUniform( handle, i, maxLen, &written, &size, &type, name );
        location = glGetUniformLocation(handle, name);
        printf(" %-8d | %s\n",location, name);
    }

    free(name);
}

void GLSLProgram::printActiveAttribs() {

    GLint written, size, location, maxLength, nAttribs;
    GLenum type;
    GLchar * name;

    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &nAttribs);

    name = (GLchar *) malloc( maxLength );

    printf(" Index | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nAttribs; i++ ) {
        glGetActiveAttrib( handle, i, maxLength, &written, &size, &type, name );
        location = glGetAttribLocation(handle, name);
        printf(" %-5d | %s\n",location, name);
    }

    free(name);
}

int GLSLProgram::getUniformLocation(const char * name )
{
    return glGetUniformLocation(handle, name);
}

bool GLSLProgram::fileExists( const string & fileName )
{
    struct stat info;
    int ret = -1;

    ret = stat(fileName.c_str(), &info);
    return 0 == ret;
}

void GLSLProgram::setUniformSubroutine( const char *name, GLenum shadertype )
{
	GLuint subIndex = glGetSubroutineIndex(handle,shadertype,name);
	glUniformSubroutinesuiv(shadertype,1,&subIndex);
}
