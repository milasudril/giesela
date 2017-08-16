//@	{
//@	 "targets":[{"name":"colorstring.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"colorstring.o","rel":"implementation"}]
//@	}

#ifndef UIXX_COLORSTRING_HPP
#define UIXX_COLORSTRING_HPP

#include <array>
#include "color.hpp"

namespace UIxx
	{
	class ColorString:public std::array<char,64>
		{
		public:
			explicit ColorString(const ColorRGBA& color);
		};

	ColorRGBA colorFromString(const char* colorstring);

	bool colorFromString(const char* colorstring,ColorRGBA& color);
	}

#endif
