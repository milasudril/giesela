//@	{
//@  "targets":[{"name":"tabview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"tabview.o","rel":"implementation"}]
//@	}

#ifndef UIXX_TABVIEW_HPP
#define UIXX_TABVIEW_HPP

#include "container.hpp"
#include <utility>

namespace UIxx
	{
	class TabView:public Container
		{
		public:
			explicit TabView(Container& parent);
			~TabView();

			TabView& operator=(TabView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			TabView(TabView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			TabView& add(void* handle);
			TabView& show();
			TabView& sensitive(bool val);
			TabView& tabTitle(const char* t);
			void* toplevel() const;

			TabView& activate(int index) noexcept;

		protected:
			class Impl;
			explicit TabView(TabView::Impl& impl):m_impl(&impl){}
			Impl* m_impl;
		};
	}

#endif // UIXX_TabView_HPP
