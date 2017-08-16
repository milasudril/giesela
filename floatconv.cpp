//@	 {"targets":[{"name":"floatconv.o","type":"object"}]}

#include "floatconv.hpp"
#include "localeguard.hpp"
#include <cstdlib>
#include <string>

using namespace UIxx;

bool UIxx::convert(const char* string,double& result)
	{
	auto ptr=string;
	bool got_separator=0;
	std::string buffer;
	while(*ptr!='\0')
		{
		if(*ptr>=1 && *ptr<=' ')
			{++ptr;}
		else
			{
			switch(*ptr)
				{
				case '.':
				case ',':
					if(got_separator)
						{return 0;}
					buffer+='.';
					got_separator=1;
					++ptr;
					break;
				default:
					buffer+=(*ptr);
					++ptr;
				}
			}
		}

	char* ptr_end;
	auto text=buffer.c_str();
	LocaleGuard locale("C");
	result=strtod(text,&ptr_end);
	if(ptr_end!=text)
		{return 1;}
	return 0;
	}
