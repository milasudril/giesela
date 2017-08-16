//@	 {"targets":[{"name":"colorstring.o","type":"object"}]}

#include "colorstring.hpp"
#include "../floatconv.hpp"
#include <cstdio>
#include <string>

using namespace UIxx;

ColorString::ColorString(const ColorRGBA& color)
	{
	sprintf(begin(),"%.7g; %.7g; %.7g; %.7g"
		,color.red,color.green,color.blue,color.alpha);
	}



ColorRGBA UIxx::colorFromString(const char* colorstring)
	{
	std::string buffer;
	float values[4]={0.0f,0.0f,0.0f,1.0f};
	uint8_t count=0;
	while(*colorstring!='\0' && count!=4)
		{
		switch(*colorstring)
			{
			case ';':
				values[count]=convert(buffer.c_str());
				++count;
				buffer.clear();
				break;
			default:
				buffer+=(*colorstring);
				break;
			}
		++colorstring;
		}
	if(count!=4)
		{values[count]=convert(buffer.c_str());}

	return ColorRGBA{values[0],values[1],values[2],values[3]};
	}

bool UIxx::colorFromString(const char* colorstring,ColorRGBA& color)
	{
	std::string buffer;
	float values[4]={0.0f,0.0f,0.0f,1.0f};
	uint8_t count=0;
	while(*colorstring!='\0' && count!=4)
		{
		switch(*colorstring)
			{
			case ';':
				if(!convert(buffer.c_str(),values[count]))
					{return 0;}
				++count;
				buffer.clear();
				break;
			default:
				buffer+=(*colorstring);
				break;
			}
		++colorstring;
		}
	if(count!=4)
		{
		if(!convert(buffer.c_str(),values[count]))
			{return 0;}
		}
	color=ColorRGBA{values[0],values[1],values[2],values[3]};
	return 1;
	}
