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
			template<class Logger>
			Renderer(Logger& log):
				Renderer([](void* cb_obj,const char* message)
					{reinterpret_cast<Logger*>(cb_obj)->log(message);},&log)
				{}
			
			Renderer(void (*log_callback)(void* cb_obj,const char* message),void* cb_obj);
			~Renderer();
			
			void render() noexcept;
			void viewport(int width,int height) noexcept;
			void mesh(const Mesh& m);
			
			static const char* defaultShader() noexcept;
			
			void shader(const char* shader_string);
			
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
