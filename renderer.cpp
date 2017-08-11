//@	{"targets":[{"name":"renderer.o","type":"object","pkgconfig_libs":["glew"]}]}

#include "renderer.hpp"
#include <GL/glew.h>
#include <cstdio>

static auto initiated=false;

using namespace Giesela;

static const GLfloat g_vertex_buffer_data[]=
	{
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f,  1.0f, 0.0f
	};

Renderer::Renderer()
	{
	if(!initiated)
		{
		glewExperimental=GL_TRUE;
		auto status=glewInit();
		if(status!=GLEW_OK)
			{throw "GLEW initailzation failed";}
		auto renderer = glGetString(GL_RENDERER);
		auto version = glGetString(GL_VERSION);
		fprintf(stderr,"Renderer: %s\n", renderer);
		fprintf(stderr,"OpenGL version supported %s\n", version);
		initiated=true;
		}
		
	glCreateVertexArrays(1,&m_vao);
	glCreateBuffers(1,&m_vbo);
	
	glNamedBufferData(m_vbo,sizeof(g_vertex_buffer_data),g_vertex_buffer_data,GL_STATIC_DRAW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1);
	glEnableVertexArrayAttrib(m_vao,0);
	glBindVertexArray(m_vao);
	}

Renderer::~Renderer()
	{
	glDisableVertexArrayAttrib(m_vao,0);
	glDeleteBuffers(1,&m_vbo);
	glDeleteVertexArrays(1,&m_vao);
	}
	
void Renderer::render() noexcept
	{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,nullptr);
	glDrawArrays(GL_TRIANGLES,0,3);
	}
