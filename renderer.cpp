//@	{"targets":[{"name":"renderer.o","type":"object","pkgconfig_libs":["glew"]}]}

#include "renderer.hpp"
#include "program.hpp"
#include "mesh.hpp"
#include "error.hpp"
#include <GL/glew.h>
#include <cstdio>
#include <cmath>

static auto initiated=false;

using namespace Giesela;

static constexpr auto pi=std::acos(-1.0f);

static constexpr const char* s_default_shader=R"EOF(#version 450 core

in vec3 fragment_normal;
in vec3 fragment_pos;
out vec3 color;

float sRGB(float x)
	{
	if (x <= 0.00031308)
		{return 12.92 * x;}
	else
		{return 1.055*pow(x,(1.0 / 2.4) ) - 0.055;}
	}

vec3 to_srgb(vec3 color)
	{
	return vec3(sRGB(color.r),sRGB(color.g),sRGB(color.b));
	}

layout(location=2) uniform vec3 color_in;
layout(location=3) uniform vec3 light_position;
layout(location=4) uniform float light_intensity;

void main()
	{
	vec3 lf=light_position - fragment_pos;
	float d2=dot(lf,lf);
	float d=sqrt(d2);
	float proj=clamp(dot( fragment_normal,lf/d ),0,1);
	color=to_srgb(light_intensity * color_in*proj/d2 );
	}
)EOF";

const char* Renderer::defaultShader() noexcept
	{return s_default_shader;}

Renderer::Renderer(void (*log_callback)(void* cb_obj,const char* message),void* cb_obj)
	{
	if(!initiated)
		{
		glewExperimental=GL_TRUE;
		auto status=glewInit();
		if(status!=GLEW_OK)
			{throw Error("GLEW: ",reinterpret_cast<const char*>( glewGetErrorString(status) ) );}
		initiated=true;
		
		std::string buffer("(i) Using GLEW version ");
		buffer+=reinterpret_cast<const char*>(glewGetString(GLEW_VERSION));
		log_callback(cb_obj,buffer.c_str());
	
		buffer.clear();
		buffer+="\nOpenGL information: \n"
			"  Vendor:   ";
		buffer+=reinterpret_cast<const char*>( glGetString(GL_VENDOR) );
		buffer+="\n  Renderer: ";
			buffer+=reinterpret_cast<const char*>( glGetString(GL_RENDERER) );
		buffer+="\n  Version:  ";
			buffer+=reinterpret_cast<const char*>( glGetString(GL_VERSION) );
		buffer+="\n  Shader:   ";
			buffer+=reinterpret_cast<const char*>( glGetString(GL_SHADING_LANGUAGE_VERSION) );
		buffer+="\n";
		
		log_callback(cb_obj,buffer.c_str());
		
		}
		
	glCreateVertexArrays(1,&m_vao);
	m_vertex_buffer=0;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0,0,0,1);
	glBindVertexArray(m_vao);
	shader(s_default_shader);
	m_theta=0;
	m_cam_pos=glm::vec3{0.0f,-3.0f,2.0f};
	m_View=glm::lookAt(m_cam_pos,glm::vec3{0.0f,0.0f,0.0f},glm::vec3{0.0f,0.0f,1.0f});
	m_Projection=glm::perspective(0.5f*pi,1.0f,0.1f,100.0f);
	}

void Renderer::shader(const char* shader_string)
	{
	m_program.reset(new Program(
		  Shader{R"EOF(#version 450 core
layout(location=0) in vec3 vertex_pos;
layout(location=1) in vec3 vertex_normal;

layout(location=0) uniform mat4 Model;
layout(location=1) uniform mat4 MVP;

out vec3 fragment_normal;
out vec3 fragment_pos;

void main()
	{
	gl_Position=MVP*vec4(vertex_pos,1.0);
	fragment_normal=vec3( Model*vec4(vertex_normal,0.0) );
	fragment_pos=vec3( Model*vec4(vertex_pos,1.0) );
	}
)EOF",ShaderType::VERTEX_SHADER}
		 ,Shader{shader_string,ShaderType::FRAGMENT_SHADER}
		));
	m_program->bind();
	glUniform3f(2,0.25f,0.0f,1.0f);
	glUniform3f(3,-2.0f,-2.0f,3.0f);
	glUniform1f(4,10.0f);
	}

Renderer::~Renderer()
	{
	glDisableVertexArrayAttrib(m_vao,0);

	glDeleteVertexArrays(1,&m_vao);
	}
	
void Renderer::viewport(int width,int height) noexcept
	{
	m_Projection=glm::perspective(0.5f*pi
		,static_cast<float>(width)/static_cast<float>(height),0.1f,100.0f);
	}

void Renderer::mesh(const Mesh& m)
	{
	if(m.faceCount()==0 || m.vertexCount()==0)
		{throw Error("Mesh is empty");}
	if(m_vertex_buffer!=0)
		{
		for(int k=0;k<3;++k)
			{glDisableVertexArrayAttrib(m_vao,k);}
		glDeleteBuffers(1,&m_vertex_buffer);
		glDeleteBuffers(1,&m_normal_buffer);
		glDeleteBuffers(1,&m_uv_buffer);
		glDeleteBuffers(1,&m_index_buffer);
		}
		
	glCreateBuffers(1,&m_vertex_buffer);
	glCreateBuffers(1,&m_normal_buffer);
	glCreateBuffers(1,&m_uv_buffer);
	glCreateBuffers(1,&m_index_buffer);
	
	glNamedBufferData(m_vertex_buffer,sizeof(glm::vec3)*m.vertexCount(),m.vertices(),GL_STATIC_DRAW);
	glNamedBufferData(m_normal_buffer,sizeof(glm::vec3)*m.vertexCount(),m.normals(),GL_STATIC_DRAW);
	glNamedBufferData(m_uv_buffer,sizeof(glm::vec2)*m.vertexCount(),m.uvs(),GL_STATIC_DRAW);
	glNamedBufferData(m_index_buffer,sizeof(Mesh::Face)*m.faceCount(),m.faces(),GL_STATIC_DRAW);
	m_n_faces=m.faceCount();

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, m_normal_buffer);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, m_uv_buffer);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_index_buffer);
	for(int k=0;k<3;++k)
		{glEnableVertexArrayAttrib(m_vao,k);}
	}
	
void Renderer::render() noexcept
	{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	if(m_vertex_buffer!=0)
		{
		auto theta=2.0f*pi*(m_theta/65536.0f);
		auto Model=glm::rotate(glm::mat4{},theta,glm::vec3{0.0f,0.0f,1.0f});
		auto MVP=m_Projection*m_View*Model;
		glUniformMatrix4fv(0, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(1, 1, GL_FALSE, &MVP[0][0]);
		glDrawElements(GL_TRIANGLES,3*m_n_faces,GL_UNSIGNED_SHORT,nullptr);
		m_theta+=64;
		}
	}
