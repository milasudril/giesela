//@	{
//@	"targets":[{"name":"error.o","type":"object"}]
//@	}

#include "error.hpp"
#include <cstdio>

template<>
char* append<const char*>(const char* src,char* ptr,char* ptr_max)
	{
	while(ptr!=ptr_max && *src!='\0')
		{
		*ptr=*src;
		++src;
		++ptr;
		}
	return ptr;
	}


template<>
char* append<int>(int src,char* ptr,char* ptr_max)
	{
	char buffer[16];
	sprintf(buffer,"%d",src);
	return append<const char*>(buffer,ptr,ptr_max);
	}
