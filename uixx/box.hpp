//@	{
//@  "targets":[{"name":"box.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"box.o","rel":"implementation"}]
//@	}

#ifndef UIXX_BOX_HPP
#define UIXX_BOX_HPP

#include "container.hpp"
#include "rectangle.hpp"
#include <utility>

namespace UIxx
	{
	class Box:public Container
		{
		public:
			static constexpr unsigned short FILL=1;
			static constexpr unsigned short EXPAND=2;
			struct InsertMode
				{
				unsigned short padding;
				unsigned short flags;
				};

			explicit Box(Container& parent,bool vertical,int global_spacing=2);
			~Box();

			Box& operator=(Box&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			Box(Box&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			Box& add(void* handle);
			Box& show();
			Box& sensitive(bool val);
			void* toplevel() const;

			Box& homogenous(bool status) noexcept;
			Box& insertMode(const InsertMode& mode) noexcept;

			Box& alignment(float x) noexcept;

			Rectangle boundingBox() const noexcept;

		protected:
			class Impl;
			explicit Box(Box::Impl& impl):m_impl(&impl){}
			Impl* m_impl;
		};
	}

#endif // UIXX_Box_HPP
