//@	{
//@	"targets":[{"name":"shader.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"shader.o","rel":"implementation"}]
//@	}

#ifndef GIESELA_SHADER_HPP
#define GIESELA_SHADER_HPP

#include <GL/glew.h>
#include <utility>
#include <cassert>

namespace Giesela
	{
	enum class ShaderType:GLenum
		{
		 COMPUTE_SHADER=GL_COMPUTE_SHADER
		,VERTEX_SHADER=GL_VERTEX_SHADER
		,TESS_CONTROL_SHADER=GL_TESS_CONTROL_SHADER
		,TESS_EVALUATION_SHADER=GL_TESS_EVALUATION_SHADER
		,GEOMETRY_SHADER=GL_GEOMETRY_SHADER
		,FRAGMENT_SHADER=GL_FRAGMENT_SHADER
		};

	static constexpr GLenum native_type(ShaderType type) noexcept
		{return static_cast<GLenum>(type);}

	static constexpr const char* name(ShaderType type) noexcept
		{
		switch(type)
			{
			case ShaderType::COMPUTE_SHADER:
				return "compute shader";

			case ShaderType::VERTEX_SHADER:
				return "vertex shader";

			case ShaderType::TESS_CONTROL_SHADER:
				return "tesselation control shader";

			case ShaderType::TESS_EVALUATION_SHADER:
				return "tesselation evaluation shader";

			case ShaderType::GEOMETRY_SHADER:
				return "geometry shader";

			case ShaderType::FRAGMENT_SHADER:
				return "fragment shader";
			default:
				return "";
			}
		}

	class Shader
		{
		public:
			Shader(const char* source,ShaderType type);
			Shader(const Shader&)=delete;
			Shader& operator=(const Shader&)=delete;

			Shader(Shader&& obj) noexcept:m_handle(obj.m_handle)
				{obj.m_handle=0;}

			Shader& operator=(Shader&& obj) noexcept
				{
				std::swap(m_handle,obj.m_handle);
				return *this;
				}

			~Shader() noexcept
				{glDeleteShader(m_handle);}

			GLuint handle() const noexcept
				{return m_handle;}

		private:
			GLuint m_handle;
	};
	}
#endif
