//@	{"targets":[{"name":"uitest","type":"application"}]}

#include "uixx/window.hpp"
#include "uixx/tpaned.hpp"
#include "uixx/uicontext.hpp"
#include "uixx/label.hpp"

#include <cstdio>

struct UiCallback
	{
	UIxx::UiContext::RunStatus idle(UIxx::UiContext& ctx)
		{return ctx.RunStatus::WAIT;}
	
	typedef int MessageId;
	typedef int MessageParam;
	void process(UIxx::UiContext& ctx,MessageId,MessageParam){}
	};
	
int main()
	{
	try
		{
		UIxx::UiContext ctx;
		UIxx::Window mainwin("Giesela");
		UIxx::TPaned<UIxx::TPanedLayout::SmallSmallLarge,UIxx::Label,UIxx::Label,UIxx::Label>
			paned(mainwin,true,[](UIxx::Container& cnt)
					{return UIxx::Label(cnt,"A");}
				,[](UIxx::Container& cnt)
					{return UIxx::Label(cnt,"B");}
				,[](UIxx::Container& cnt)
					{return UIxx::Label(cnt,"C");});
			
		mainwin.show();
		
		UiCallback ui_callback;
			
		ctx.run(ui_callback);
		}
	catch(const char* message)
		{fprintf(stderr,"Error: %s\n", message);}
	return 0;
	}
