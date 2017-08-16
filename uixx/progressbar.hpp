//@	{
//@  "targets":[{"name":"progressbar.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"progressbar.o","rel":"implementation"}]
//@	}

#ifndef UIXX_PROGRESSBAR_HPP
#define UIXX_PROGRESSBAR_HPP

#include <utility>

namespace UIxx
	{
	class Container;

	class ProgressBar
		{
		public:
			explicit ProgressBar(Container& container);
			~ProgressBar();

			ProgressBar& operator=(ProgressBar&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			ProgressBar(ProgressBar&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			ProgressBar& value(double x);

		protected:
			class Impl;
			Impl* m_impl;
			explicit ProgressBar(Impl& impl):m_impl(&impl){}
		};
	}

#endif
