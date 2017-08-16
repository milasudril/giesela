//@	{
//@	 "targets":[{"name":"paletteview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"paletteview.o","rel":"implementation"}]
//@	}

#ifndef UIXX_PALETTEVIEW_HPP
#define UIXX_PALETTEVIEW_HPP

#include <utility>
#include <cstddef>

namespace UIxx
	{
	class Container;
	class ColorRGBA;

	class PaletteView
		{
		public:
			explicit PaletteView(Container& cnt);
			~PaletteView();

			PaletteView& operator=(PaletteView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			PaletteView(PaletteView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}


			template<class ColorRange>
			PaletteView& palette(const ColorRange& r)
				{return palette(r.begin(),r.end());}

			PaletteView& palette(const ColorRGBA* colors_begin
				,const ColorRGBA* colors_end);

			template<class Callback,class IdType>
			PaletteView& callback(Callback& cb_obj,IdType id) noexcept
				{
				auto cb_wrapper=[](void* rvc,PaletteView& self)
					{
					auto x=reinterpret_cast<Callback*>(rvc);
					auto id=static_cast<IdType>( self.id() );
					x->indexSelected(self,id);
					};
				return callback(cb_wrapper,&cb_obj,static_cast<int>(id));
				}

			int id() const noexcept;

			int selection() const noexcept;

			PaletteView& selection(int index);

			const ColorRGBA& color(int index) const noexcept;

			PaletteView& color(int index,const ColorRGBA& color);

			const ColorRGBA* paletteBegin() const noexcept;

			const ColorRGBA* paletteEnd() const noexcept;

		private:
			class Impl;
			Impl* m_impl;

			PaletteView(Impl& impl):m_impl(&impl)
				{}

			typedef void (*CallbackImpl)(void* cb_obj,PaletteView& self);
			PaletteView& callback(CallbackImpl,void* cb_obj,int id);
		};
	}

#endif // UIXX_PALETTEVIEW_HPP
