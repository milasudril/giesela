//@	{
//@	 "targets":[{"name":"readysignal.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"readysignal.o","rel":"implementation"}]
//@	}

#ifndef UIXX_READYSIGNAL_HPP
#define UIXX_READYSIGNAL_HPP

#include <utility>

namespace UIxx
	{
	class ReadySignal
		{
		public:
			ReadySignal();
			~ReadySignal();

			ReadySignal& operator=(ReadySignal&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			ReadySignal(ReadySignal&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			void set() noexcept;
			void wait() noexcept;
			void reset() noexcept;

		private:
			class Impl;
			Impl* m_impl;
		};
	}

#endif
