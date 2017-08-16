//@	{
//@	 "targets":[{"name":"imageview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"imageview.o","rel":"implementation"}]
//@	}

#ifndef UIXX_IMAGEVIEW_HPP
#define UIXX_IMAGEVIEW_HPP

#include "imagerepository.hpp"
#include <utility>
#include <cstdint>

namespace UIxx
	{
	class Container;

	class ImageView
		{
		public:
			explicit ImageView(Container& cnt);
			~ImageView();

			ImageView& operator=(ImageView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			ImageView(ImageView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}

			ImageView& showPng(const ImageRepository& repo,ImageRepository::IdType id
				,const std::pair<const uint8_t*,const uint8_t*>& data);

			ImageView& minHeight(int h);

			ImageView& minWidth(int w);

			ImageView& minSize(int w,int h);

			template<class Callback,class IdType>
			ImageView& callback(Callback& cb_obj,IdType id)
				{
				auto cb=[](void* cb_obj,ImageView& source)
					{reinterpret_cast<Callback*>(cb_obj)->clicked(source,static_cast<IdType>(source.id()));};
				return callback(cb,&cb_obj,static_cast<int>(id));
				}

			int id() const noexcept;

			ImageView& backgroundShade(float hue,float strength) noexcept;

			ImageView& padding(int x) noexcept;

			ImageView& title(const char* text);

		private:
			class Impl;
			Impl* m_impl;

			ImageView(Impl& impl):m_impl(&impl)
				{}

			typedef void (*CallbackImpl)(void* cb_obj,ImageView& self);
			ImageView& callback(CallbackImpl,void* cb_obj,int id);
		};
	}

#endif // UIXX_IMAGEVIEW_HPP
