//@	{"targets":[{"name":"renderer.o","type":"object","pkgconfig_libs":["glew"]}]}

#include "renderer.hpp"
#include "program.hpp"
#include <GL/glew.h>
#include <cstdio>
#include <cmath>

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
	m_program.reset(new Program(
		  Shader{R"EOF(
#version 450 core
layout(location=0) in vec3 vertex_pos;
layout(location=0) uniform mat4 ModelMatrix;
void main()
	{
	gl_Position=ModelMatrix*vec4(vertex_pos,1.0);
	}
)EOF",ShaderType::VERTEX_SHADER}
		 ,Shader{R"EOF(
#version 450 core
out vec3 color;
void main()
	{
	color=vec3(1,0,0);
	}
)EOF",ShaderType::FRAGMENT_SHADER}
		));
	m_program->bind();
	m_theta=0;
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
	auto theta=2.0f*std::acos(-1.0f)*(m_theta/65536.0f);
	float M[16]=
		{
		 std::cos(theta),-std::sin(theta),0,0
		,std::sin(theta),std::cos(theta),0,0
		,0,0,1,0
		,0,0,0,1
		};
	glUniformMatrix4fv(0, 1, GL_FALSE, &M[0]);
	glDrawArrays(GL_TRIANGLES,0,3);
	m_theta+=16;
	}
