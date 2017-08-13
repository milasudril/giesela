//@	{
//@	 "targets":[{"name":"mesh.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"mesh.o","rel":"implementation"}]
//@	}

#ifndef GIESELA_MESH_HPP
#define GIESELA_MESH_HPP

#include "glm_giesela.hpp"
#include <cstdio>
#include <vector>

namespace Giesela
	{
	class Mesh
		{
		public:
			struct Face
				{
				uint16_t verts[3];	
				};
			
			static Mesh fromWavefrontObj(FILE* src,const char* stream_src);

			size_t faceCount() const noexcept
				{return m_faces.size()/3;}
			
			size_t vertexCount() const noexcept
				{return m_vertices.size();}
				
			const glm::vec3* vertices() const noexcept
				{return m_vertices.data();}
				
			const glm::vec3* normals() const noexcept
				{return m_vertices.data();}
			
		private:
			Mesh()=default;
			
			std::vector<glm::vec3> m_vertices;
			std::vector<glm::vec3> m_normals;
			std::vector<glm::vec2> m_uvs;
			std::vector<uint16_t> m_faces;
		};
	}
#endif
