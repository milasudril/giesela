			//@	{"targets":[{"name":"portselector.hpp","type":"include"}]}

#ifndef UIXX_PORTSELECTOR_HPP
#define UIXX_PORTSELECTOR_HPP

#include "buttonlist.hpp"
#include "filler.hpp"

namespace UIxx
	{

	class PortSelector
		{
		public:
			PortSelector(Container& cnt):m_box(cnt,true)
				,m_label(m_box.insertMode({4,0}),"Select or unselect ports by click-\ning on the corresponding button")
					,m_ports_panel(m_box.insertMode({4,Box::EXPAND|Box::FILL}),false)
						,m_filler_left(m_ports_panel.insertMode({0,Box::EXPAND|Box::FILL}))
						,m_ports(m_ports_panel.insertMode({0,0}),true)
						,m_filler_right(m_ports_panel.insertMode({0,Box::EXPAND|Box::FILL}))
				,m_sorted(1)
				{
				}

			~PortSelector()
				{}

			PortSelector& portAppend(const char* port_name)
				{
				m_ports.append(port_name);
				m_sorted=0;
				return *this;
				}

			template<class Callback>
			const PortSelector& state(Callback&& cb) const noexcept
				{
				std::for_each(m_ports.begin(),m_ports.end()
					,[&cb](const Button& btn)
						{cb(btn.label(),btn.state());});
				return *this;
				}

			PortSelector& select(const char* port_name)
				{
				auto cmp=[](const Button& a,const char* label)
					{return strcmp(a.label(),label)<0;};

				auto cmp_2=[cmp](const Button& a,const Button& b)
					{return cmp(a,b.label());};

				if(!m_sorted)
					{
					std::sort(m_ports.begin(),m_ports.end(),cmp_2);
					m_sorted=1;
					}

				auto i=std::lower_bound(m_ports.begin(),m_ports.end(),port_name,cmp);
				if(i!=m_ports.end() && !cmp(*i,port_name))
					{i->state(1);}

				return *this;
				}



		private:
			Box m_box;
				Label m_label;
				Box m_ports_panel;
					Filler m_filler_left;
					ButtonList m_ports;
					Filler m_filler_right;

			mutable bool m_sorted;
		};

	}

#endif // UIXX_PORTSELECTOR_HPP
