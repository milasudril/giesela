//@	{
//@  "targets":[{"name":"tpaned.hpp","type":"include"}]
//@	}

#ifndef UIXX_TPANED_HPP
#define UIXX_TPANED_HPP

#include "paned.hpp"
#include <utility>

namespace UIxx
	{
	enum class TPanedLayout:int{LargeSmallSmall,SmallSmallLarge};
		
	template<TPanedLayout l,class MainWidget,class SubwidgetA,class SubwidgetB>
	class TPaned
		{
		public:
			template<class MainWidgetMaker,class SubwidgetAMaker,class SubwidgetBMaker>
			TPaned(Container& cnt,bool vertical
				,MainWidgetMaker&& widget_main,SubwidgetAMaker&& widget_a,SubwidgetBMaker&& widget_b):
				m_layout(cnt,vertical,std::forward<MainWidgetMaker>(widget_main)
					,std::forward<SubwidgetAMaker>(widget_a),std::forward<SubwidgetBMaker>(widget_b))
				{}

		private:
			template<TPanedLayout layout,bool dummy=0>
			struct Layout;
			
			template<bool dummy>
			struct Layout<TPanedLayout::LargeSmallSmall,dummy>
				{
				template<class MainWidgetMaker,class SubwidgetAMaker,class SubwidgetBMaker>
				Layout(Container& cnt,bool vertical
					,MainWidgetMaker&& widget_main,SubwidgetAMaker&& widget_a
					,SubwidgetBMaker&& widget_b):m_paned(cnt,vertical)
						,m_main(widget_main( m_paned.insertMode({Paned::RESIZE|Paned::SHRINK_ALLOWED}) ))
						,m_subpaned(m_paned,!vertical)
							,m_a(widget_a( m_subpaned.insertMode({Paned::RESIZE|Paned::SHRINK_ALLOWED}) ))
							,m_b(widget_b( m_subpaned ))
					{}
			
				Paned m_paned;
					MainWidget m_main;
					Paned m_subpaned;
						SubwidgetA m_a;
						SubwidgetB m_b;
				};
			
			
			template<bool dummy>
			struct Layout<TPanedLayout::SmallSmallLarge,dummy>
				{
				template<class MainWidgetMaker,class SubwidgetAMaker,class SubwidgetBMaker>
				Layout(Container& cnt,bool vertical
					,MainWidgetMaker&& widget_main,SubwidgetAMaker&& widget_a
					,SubwidgetBMaker&& widget_b):m_paned(cnt,vertical)
						,m_subpaned(m_paned.insertMode({Paned::RESIZE|Paned::SHRINK_ALLOWED}),!vertical)
							,m_a(widget_a( m_subpaned.insertMode({Paned::RESIZE|Paned::SHRINK_ALLOWED}) ))
							,m_b(widget_b( m_subpaned ))
						,m_main(widget_main( m_paned ))
					{}
			
				Paned m_paned;
					Paned m_subpaned;
						SubwidgetA m_a;
						SubwidgetB m_b;
					MainWidget m_main;

				};
	
			Layout<l> m_layout;
		};
	}

#endif // UIXX_TPANED_HPP
