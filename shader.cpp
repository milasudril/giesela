//@	{"targets":[{"name":"shader.o","type":"object","pkgconfig_libs":["glew"]}]}

#include "shader.hpp"
#include <algorithm>
#include <cstring>

using namespace Giesela;

Shader::Shader(const char* source,ShaderType type)
	{
	assert(glCreateShader!=nullptr);
	m_handle=glCreateShader(native_type(type));
	assert(m_handle!=0);

	auto length=static_cast<GLint>(strlen(source));
	glShaderSource(m_handle,1,&source,&length);
	glCompileShader(m_handle);
	auto status=static_cast<GLint>(0);
	glGetShaderiv(m_handle,GL_COMPILE_STATUS,&status);
	if(status!=GL_TRUE)
		{
		auto length=static_cast<GLint>(0);
		glGetShaderiv(m_handle,GL_INFO_LOG_LENGTH,&length);
		char message[1024];
		glGetShaderInfoLog(m_handle,std::min(1024,length),NULL,message);
		glDeleteShader(m_handle);
	
		fprintf(stderr,"%s ",message);
		throw "Failed to compile shader";
//		exceptionRaise(Error("It was not possible to compile the ",name(type),". ",message));
		}
	}