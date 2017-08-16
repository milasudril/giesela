//@	{
//@	 "targets":[{"name":"colorpicker.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"colorpicker.o","rel":"implementation"}]
//@	}

#ifndef UIXX_COLORPICKER_HPP
#define UIXX_COLORPICKER_HPP

#include "box.hpp"
#include "paletteview.hpp"
#include "colorview.hpp"
#include "slider.hpp"
#include "textentry.hpp"
#include "label.hpp"
#include "color.hpp"

namespace UIxx
	{
	class ColorPicker
		{
		public:
			enum class SliderId:int{HUE,SATURATION,LIGHTNESS};

			explicit ColorPicker(Container& cnt);

			ColorPicker& operator=(ColorPicker&&)=delete;
			ColorPicker(ColorPicker&&)=delete;

			void indexSelected(PaletteView& palview,int id);

			void changed(Slider& slider,SliderId id);

			const ColorRGBA& color() noexcept
				{return m_result.color();}

			ColorPicker& color(const ColorRGBA& color)
				{
				slidersUpdate(color);
				return *this;
				}

			const ColorRGBA* presetsBegin() const noexcept
				{return m_pal_view.paletteBegin();}

			const ColorRGBA* presetsEnd() const noexcept
				{return m_pal_view.paletteEnd();}

			void changed(TextEntry& entry,int id) noexcept;

			ColorPicker& presets(const ColorRGBA* pal_begin,const ColorRGBA* pal_end);

		private:
			Box m_box;
				PaletteView m_pal_view;
				Box m_hue;
					Label m_hue_label;
					Slider m_hue_slider;
				Box m_sat;
					Label m_sat_label;
					ColorView m_sat_min;
					Slider m_sat_slider;
					ColorView m_sat_max;
				Box m_lightness;
					Label m_lightness_label;
					ColorView m_lightness_min;
					Slider m_lightness_slider;
					ColorView m_lightness_max;
				ColorView m_result;
				TextEntry m_rgb;

			void viewUpdate(const ColorRGBA& color);

			void slidersUpdate(const ColorRGBA& color);
		};
	}

#endif
