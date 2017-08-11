//@	{
//@	"targets":[{"name":"renderer.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"renderer.o","rel":"implementation"}]
//@	}

#ifndef GIESELA_RENDERER_HPP
#define GIESELA_RENDERER_HPP

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
			
		private:
			unsigned int m_vao;
			unsigned int m_vbo;
			uint16_t m_theta;
			std::unique_ptr<Program> m_program;
		};
	}

#endif
