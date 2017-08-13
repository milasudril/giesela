//@	{
//@	 "targets":[{"name":"mesh.o","type":"object"}]
//@	}

#include "mesh.hpp"
#include <string>
#include <algorithm>

using namespace Giesela;

static uint16_t to_uint_checked(const char* x)
	{
	auto val=atoll(x);
	if(val>0xffff || val<0)
		{throw "Value out of range";}
	return val;
	}
	
struct WavefrontObj_Vertex
	{
	int vertex;
	int uv;
	int normal;
	};
	
struct WavefrontObj_Face
	{
	WavefrontObj_Vertex verts[3];
	};

Mesh Mesh::fromWavefrontObj(FILE* src,const char* stream_src)
	{
	Mesh ret;
	enum class State:int{INIT,COMMENT,VERTEX,VERTEX_NORMAL,FACE};
	std::string token_in;
	auto state_current=State::INIT;
	auto v=glm::vec3{0.0f,0.0f,0.0f};
	auto val_count=0;
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<WavefrontObj_Vertex> faces;
	
	while(true)
		{
		auto ch_in=getc(src);
		if(ch_in==EOF)
			{break;}
		if(ch_in=='\r')
			{continue;}

		switch(state_current)
			{
			}
		}
	return std::move(ret);
	}
