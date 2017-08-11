//@	{
//@	"targets":[{"name":"renderer.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"renderer.o","rel":"implementation"}]
//@	}

#ifndef GIESELA_RENDERER_HPP
#define GIESELA_RENDERER_HPP

namespace Giesela
	{
	class Renderer
		{
		public:
			Renderer();
			~Renderer();
			
			void render() noexcept;
			
		private:
			unsigned int m_vao;
			unsigned int m_vbo;
		};
	}

#endif
