//@	{
//@	 "targets":[{"name":"mesh.o","type":"object"}]
//@	}

#include "mesh.hpp"
#include <string>
#include <algorithm>

using namespace Giesela;

static uint16_t to_uint16_checked(const char* x)
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

static int to_int_checked(const char* x)
	{
	auto val=atoll(x);
	if( val>0x7fffffff || val<static_cast<int>(0x80000000) )
		{throw "Value out of range";}
	return val;
	}
	
void value_set(const std::string& buffer,WavefrontObj_Vertex& v,int field_count)
	{
	switch(field_count)
		{
		case 0:
			v.vertex=to_int_checked(buffer.c_str());
			break;
		case 1:
			v.uv=to_int_checked(buffer.c_str());
			break;
		case 2:
			v.normal=to_int_checked(buffer.c_str());
			break;
		default:
			throw "Too many vertex fields";
		}
	}
	
static std::pair<WavefrontObj_Face,bool> face_read(FILE* src,const char* stream_src)
	{
	auto vert_count=0;
	auto field_count=0;
	WavefrontObj_Vertex v{0,0,0};
	std::string buffer;
	WavefrontObj_Face ret;
	while(true)
		{
		auto ch_in=getc(src);
		if(ch_in==EOF)
			{
			if(vert_count>=3)
				{throw "Only tris are supported";}
			ret.verts[vert_count]=v;	
			++vert_count;
			if(vert_count!=3)
				{throw "Too few vertices in face";}
			return {ret,false};
			}
		else
		if(ch_in=='/')
			{
			value_set(buffer,v,field_count);
			++field_count;
			buffer.clear();
			}
		else
		if(ch_in>=0 && ch_in<=' ') 
			{
			value_set(buffer,v,field_count);
			buffer.clear();
			switch(ch_in)
				{
				case '\n':
					if(vert_count>=3)
						{throw "Only tris are supported";}
					ret.verts[vert_count]=v;	
					++vert_count;
					if(vert_count!=3)
						{throw "Too few vertices in face";}
					return {ret,true};
					
				default:
					if(v.vertex!=0)
						{
						if(vert_count>=3)
							{throw "Only tris are supported";}
						ret.verts[vert_count]=v;	
						++vert_count;
						field_count=0;
						}
				}
			}
		else
			{buffer+=ch_in;}
		}
	}

Mesh Mesh::fromWavefrontObj(FILE* src,const char* stream_src)
	{
	Mesh ret;
	enum class State:int{INIT,COMMENT};
	std::string token_in;
	auto state_current=State::INIT;
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<WavefrontObj_Face> faces;
	
	while(true)
		{
		auto ch_in=getc(src);
		if(ch_in==EOF)
			{break;}
		if(ch_in=='\r')
			{continue;}

		switch(state_current)
			{
			case State::INIT:
				switch(ch_in)
					{
				//	case 'v':
					//	state_current=State::VERTEX;
					//	break;
					case 'f':
						{
						auto f=face_read(src,stream_src);
						if(!f.second) //EOF
							{break;}
						faces.push_back(f.first);
						}
						break;
					default:
						if(!(ch_in>=0 && ch_in<=' '))
							{state_current=State::COMMENT;}
					}
				break;
			case State::COMMENT:
				if(ch_in=='\n')
					{state_current=State::INIT;}
				break;
			}
		}
	return std::move(ret);
	}
