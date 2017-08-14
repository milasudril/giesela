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
	class Mesh;
	class Renderer
		{
		public:
			Renderer();
			~Renderer();
			
			void render() noexcept;
			void viewport(int width,int height) noexcept;
			void mesh(const Mesh& m);
			
		private:
			uint16_t m_theta;
			std::unique_ptr<Program> m_program;

			glm::vec3 m_cam_pos;
			glm::mat4 m_View;
			glm::mat4 m_Projection;
			
			unsigned int m_vao;
			unsigned int m_vertex_buffer;
			unsigned int m_normal_buffer;
			unsigned int m_uv_buffer;
			unsigned int m_index_buffer;
			unsigned int m_n_faces;
		};
	}

#endif
