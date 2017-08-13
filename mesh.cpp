//@	{
//@	 "targets":[{"name":"mesh.o","type":"object"}]
//@	}

#include "mesh.hpp"
#include <string>
#include <algorithm>

using namespace Giesela;

static int16_t to_int_checked(const char* x)
	{
	auto val=atoll(x);
	if(val>32767 || val<32767)
		{throw "Value out of range";}
	return val;
	}

Mesh Mesh::fromWavefrontObj(FILE* src,const char* stream_src)
	{
	Mesh ret;
	enum class State:int{INIT,COMMENT,VERTEX,VERTEX_NORMAL,FACE};
	std::string token_in;
	auto state_current=State::INIT;
	auto v=glm::vec3{0.0f,0.0f,0.0f};
	auto val_count=0;
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
					case '#':
						state_current=State::COMMENT;
					case 'v':
						state_current=State::VERTEX;
						break;
					case 'f':
						state_current=State::FACE;
						break;
					default:
						if(!(ch_in>=0 && ch_in<=' '))
							{throw "Invalid character";}
					break;
					}
				break;
			case State::COMMENT:
				{
				switch(ch_in)
					{
					case '\n':
						state_current=State::INIT;
						break;
					default:
						break;
					}
				}
				break;
			case State::VERTEX:
				{
				switch(ch_in)
					{
					case 't':
						throw "Texture mapping is not implemented";
						break;
					case 'p':
						throw "Parameter space vertices are not supported";
						break;
					case 'n':
						if(token_in.size()!=0 || val_count!=0)
							{throw "Invalid state switch";}
						state_current=State::VERTEX_NORMAL;
						break;
						
					case ' ':
						if(token_in.size()!=0)
							{
							if(val_count>=3)
								{throw "Too many coordinates";}
							v[val_count]=atof(token_in.c_str());
							token_in.clear();
							++val_count;
							}
						break;
						
					case '\n':
						if(token_in.size()!=0)
							{
							if(val_count>=3)
								{throw "Too many coordinates";}
							v[val_count]=atof(token_in.c_str());
							token_in.clear();
							++val_count;
							}
						if(ret.m_vertices.size()>=32767)
							{throw "Too many m_vertices";}
						ret.m_vertices.push_back(v);
						state_current=State::INIT;
						val_count=0;
						break;
					default:
						token_in+=ch_in;
					}
				}
				
			case State::VERTEX_NORMAL:
				{
				switch(ch_in)
					{
					case ' ':
						if(token_in.size()!=0)
							{
							if(val_count>=3)
								{throw "Too many coordinates";}
							v[val_count]=atof(token_in.c_str());
							token_in.clear();
							++val_count;
							}
						break;
						
					case '\n':
						if(token_in.size()!=0)
							{
							if(val_count>=3)
								{throw "Too many coordinates";}
							v[val_count]=atof(token_in.c_str());
							token_in.clear();
							++val_count;
							}
							
						if(ret.m_vertices.size()>=32767)
							{throw "Too many vertex normals";}
							
						ret.m_normals.push_back(normalize(v));
						
						state_current=State::INIT;
						val_count=0;
						break;
					default:
						token_in+=ch_in;
					}
				}
				break;
				
			case State::FACE:
				{
				switch(ch_in)
					{
					case '/':
						throw "Texture mapping is not implemented";
						break;

					case ' ':
						if(token_in.size()!=0)
							{
							if(val_count>=3)
								{throw "N-gons are not supported";}
							ret.m_faces.push_back(to_int_checked(token_in.c_str()));
							token_in.clear();
							++val_count;
							}
						break;
						
					case '\n':
						if(token_in.size()!=0)
							{
							if(val_count>=3)
								{throw "N-gons are not supported";}
							ret.m_faces.push_back(to_int_checked(token_in.c_str()));
							token_in.clear();
							++val_count;
							}
						state_current=State::INIT;
						val_count=0;
						break;
					default:
						token_in+=ch_in;
					}
				}
				break;
			}
		}
	
	if(ret.m_normals.size()!=ret.m_vertices.size())
		{throw "Vertex-Normal count mismatch";}
	
	auto n_max=static_cast<int16_t>( ret.m_vertices.size() );
	std::for_each(ret.m_faces.begin(),ret.m_faces.end(),[n_max](int16_t& x)
		{
		if(x<0)
			{throw "Relative vertex indices are not supported";}
		--x;
		if(x<0 || x>=n_max )
			{throw "Vertex index out of bounds";}
		});
	
	return std::move(ret);
	}
