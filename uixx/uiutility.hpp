//@	{
//@	 "targets":[{"name":"uiutility.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"uiutility.o","rel":"implementation"}]
//@	}

#ifndef UIXX_UIUTILITY_HPP
#define UIXX_UIUTILITY_HPP

#include "color.hpp"

namespace UIxx
	{
	ColorRGBA ambientColor(void* widget_handle);
	}

#endif