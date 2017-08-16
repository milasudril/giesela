//@	{"targets":[{"name":"rectangle.hpp","type":"include"}]}

#ifndef UIXX_RECTANGLE_HPP
#define UIXX_RECTANGLE_HPP

#include "vec2.hpp"

namespace UIxx
	{
	struct Rectangle
		{
		Vec2<double> min;
		Vec2<double> max;
		};
	}

#endif // UIXX_RECTANGLE_HPP
