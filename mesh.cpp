//@	{
//@	 "targets":[{"name":"mesh.o","type":"object"}]
//@	}

#include "mesh.hpp"
#include <string>
#include <algorithm>
#include <map>

using namespace Giesela;

namespace
	{
	struct WavefrontObj_Vertex
		{
		int vertex;
		int uv;
		int normal;
		
		bool operator<(const WavefrontObj_Vertex& v) const noexcept
			{
			if(vertex<v.vertex)
				{return 1;}
			
			if(vertex==v.vertex && uv<v.uv)
				{return 1;}
			
			if(uv==v.uv && normal<v.normal)
				{return 1;}
			
			return 0;
			}
		};
		
	struct WavefrontObj_Face
		{
		WavefrontObj_Vertex verts[3];
		};
	}

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
	
static std::pair<WavefrontObj_Face,bool> face_read(int ch_in,FILE* src,const char* stream_src)
	{
	auto vert_count=0;
	auto field_count=0;
	WavefrontObj_Vertex v{0,0,0};
	std::string buffer;
	WavefrontObj_Face ret;
	while(true)
		{
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
		ch_in=getc(src);
		}
	}
	
static std::pair<glm::vec3,bool> vector_read(int ch_in,FILE* src,const char* stream_src)
	{
	glm::vec3 ret{0.0f,0.0f,0.0f};
	std::string buffer;
	auto field_count=0;
	while(true)
		{
		if(ch_in==EOF)
			{
			if(buffer.length()!=0)
				{
				if(field_count>=3)
					{throw "Too many vector components";}
				ret[field_count]=atof(buffer.c_str());
				++field_count;
				buffer.clear();
				}
			return {ret,false};
			}

		if(ch_in>=0 && ch_in<=' ')
			{
			if(buffer.length()!=0)
				{
				if(field_count>=3)
					{throw "Too many vector components";}
				ret[field_count]=atof(buffer.c_str());
				++field_count;
				buffer.clear();
				}
				
			if(ch_in=='\n')
				{return {ret,true};}
			}
		else
			{buffer+=ch_in;}
		ch_in=getc(src);
		}
	return {ret,true};
	}

static std::pair<glm::vec2,bool> vector2_read(int ch_in,FILE* src,const char* stream_src)
	{
	glm::vec2 ret{0.0f,0.0f};
	std::string buffer;
	auto field_count=0;
	while(true)
		{
		if(ch_in==EOF)
			{
			if(buffer.length()!=0)
				{
				if(field_count>=2)
					{throw "Too many vector components";}
				ret[field_count]=atof(buffer.c_str());
				++field_count;
				buffer.clear();
				}
			return {ret,false};
			}

		if(ch_in>=0 && ch_in<=' ')
			{
			if(buffer.length()!=0)
				{
				if(field_count>=2)
					{throw "Too many vector components";}
				ret[field_count]=atof(buffer.c_str());
				++field_count;
				buffer.clear();
				}
				
			if(ch_in=='\n')
				{return {ret,true};}
			}
		else
			{buffer+=ch_in;}
		ch_in=getc(src);
		}
	return {ret,true};
	}
	
static void validate(const WavefrontObj_Face& face
	,int n_verts,int n_uvs,int n_normals
	,const char* stream_src,size_t face_index)
	{
	for(int k=0;k<3;++k)
		{
		if(face.verts[k].vertex<=0 || face.verts[k].vertex > n_verts)
			{throw "Invalid vertex index";}
		if(face.verts[k].uv<=0 || face.verts[k].uv > n_uvs)
			{throw "Invalid uv index";}
		if(face.verts[k].normal<=0 || face.verts[k].normal > n_normals)
			{throw "Invalid normal index";}
		}
	}
	
static uint16_t to_uint16_checked(size_t x,const char* context)
	{
	if(x>0xffff)
		{throw "Value out of range";}
	return static_cast<uint16_t>(x);
	}
	
Mesh Mesh::fromWavefrontObj(FILE* src,const char* stream_src)
	{
	Mesh ret;
	enum class State:int{INIT,COMMENT,VERTEX};
	auto state_current=State::INIT;
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<WavefrontObj_Face> faces;
	
	auto parser=[&state_current,&vertices,&normals,&uvs,&faces,src,stream_src](int ch_in)
		{
		if(ch_in==EOF)
			{return false;}
		if(ch_in=='\r')
			{return true;}

		switch(state_current)
			{
			case State::INIT:
				switch(ch_in)
					{
					case 'v':
						state_current=State::VERTEX;
						return true;
					case 'f':
						{
						ch_in=getc(src); //consume next byte
						auto f=face_read(ch_in,src,stream_src);
						faces.push_back(f.first);
						if(!f.second) //EOF
							{return false;}
						}
						return true;
					default:
						if(!(ch_in>=0 && ch_in<=' '))
							{state_current=State::COMMENT;}
						return true;
					}
				break;
			case State::VERTEX:
				switch(ch_in)
					{
					case 'n':
						{
					//	We must read one more byte here. Otherwise, vector_read
					//	will try to process 'n'
						ch_in=getc(src); 
						auto v=vector_read(ch_in,src,stream_src);
						normals.push_back(normalize(v.first));
						if(!v.second)
							{return false;}
						state_current=State::INIT;
						return true;
						}
					case 't':
						{
						ch_in=getc(src); //	For the sake of consistency with the other cases
						auto uv=vector2_read(ch_in,src,stream_src);
						uvs.push_back(uv.first);
						if(!uv.second)
							{return false;}
						state_current=State::INIT;
						return true;
						}
					default:
						{
						auto v=vector_read(ch_in,src,stream_src);
						vertices.push_back(v.first);
						if(!v.second)
							{return false;}
						state_current=State::INIT;
						return true;
						}
					}
				break;
			case State::COMMENT:
				if(ch_in=='\n')
					{state_current=State::INIT;}
				return true;
				
			default:
				return false;
			}
		};
	while(parser(getc(src)));
	
		{
	//	Validate input data
		auto n_verts=vertices.size();
		auto n_uvs=uvs.size();
		auto n_normals=normals.size();
		size_t count=0;
		std::for_each(faces.begin(),faces.end()
			,[n_verts,n_uvs,n_normals,&count,stream_src](const WavefrontObj_Face& face)
			{
			validate(face,n_verts,n_uvs,n_normals,stream_src,count);
			++count;
			});
		}
		
		{
	//	Normalize indices
		std::for_each(faces.begin(),faces.end(),[](WavefrontObj_Face& face)
			{
			for(int k=0;k<3;++k)
				{
				face.verts[k].vertex-=1;
				face.verts[k].uv-=1;
				face.verts[k].normal-=1;
				}
			});
		}

		{
	//	Make index
		std::map<WavefrontObj_Vertex,size_t> verts_uniq;
		std::for_each(faces.begin(),faces.end()
			,[&verts_uniq,&ret,stream_src](const WavefrontObj_Face& face)
			{
			Face f;
			for(int k=0;k<3;++k)
				{
				auto i=verts_uniq.find(face.verts[k]);
				auto vertex_index=verts_uniq.size();
				if(i==verts_uniq.end())
					{verts_uniq.insert({face.verts[k],vertex_index});}
				else
					{vertex_index=i->second;}
					
				f.verts[k]=to_uint16_checked( vertex_index,stream_src);
				}
			ret.m_faces.push_back(f);
			});
		
		ret.m_vertices.resize(verts_uniq.size());
		ret.m_uvs.resize(verts_uniq.size());
		ret.m_normals.resize(verts_uniq.size());
		std::for_each(verts_uniq.begin(),verts_uniq.end()
			,[&ret,&vertices,&uvs,&normals](const std::pair<WavefrontObj_Vertex,size_t>& v)
			{
			ret.m_vertices[ v.second ]=vertices[ v.first.vertex ];
			ret.m_uvs[ v.second ]=uvs[ v.first.uv ];
			ret.m_normals[ v.second ]=normals[ v.first.normal];
			});
		}
	
	return std::move(ret);
	}
