//@	{
//@	 "targets":[{"name":"floatconv.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"floatconv.o","rel":"implementation"}]
//@	}

#ifndef UIXX_FLOATCONV_HPP
#define UIXX_FLOATCONV_HPP

#include "error.hpp"

namespace UIxx
	{
	bool convert(const char* string,double& result);

	inline bool convert(const char* string,float& result)
		{
		double temp;
		auto ret=convert(string,temp); //This is cheating...
		result=temp;
		return ret;
		}

	inline double convert(const char* string)
		{
		double ret;
		if(!convert(string,ret))
			{throw Error(string," is not a valid number.");}
		return ret;
		}
	}

#endif
