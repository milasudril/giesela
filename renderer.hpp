//@	{
//@	"targets":[{"name":"renderer.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"renderer.o","rel":"implementation"}]
//@	}

#ifndef GIESELA_RENDERER_HPP
#define GIESELA_RENDERER_HPP

#include "glm_giesela.hpp"
#include <memory>

namespace Giesela
	{
	class Program;
	class Renderer
		{
		public:
			Renderer();
			~Renderer();
			
			void render() noexcept;
			void viewport(int width,int height) noexcept;
			
		private:
			uint16_t m_theta;
			std::unique_ptr<Program> m_program;

			glm::vec3 m_cam_pos;
			glm::mat4 m_View;
			glm::mat4 m_Projection;
			
			unsigned int m_vao;
			unsigned int m_vbo;
		};
	}

#endif
