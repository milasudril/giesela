//@	{
//@  "targets":[{"name":"container.hpp","type":"include"}]
//@	}

#ifndef UIXX_CONTAINER_HPP
#define UIXX_CONTAINER_HPP

namespace UIxx
	{
	class Container
		{
		public:
			virtual ~Container()=default;
			virtual Container& add(void* handle)=0;
			virtual Container& show()=0;
			virtual Container& sensitive(bool val)=0;
			virtual void* toplevel() const=0;
		};
	}

#endif // UIXX_RANGEVIEW_HPP
