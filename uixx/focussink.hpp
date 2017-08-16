//@	{"targets":[{"name":"focussink.hpp","type":"include"}]}

#ifndef UIXX_FOCUSSINK_HPP
#define UIXX_FOCUSSINK_HPP

namespace UIxx
	{
	struct FocusSink
		{
		void* object;
		void (*action)(void* obj);
		};
	}

#endif
