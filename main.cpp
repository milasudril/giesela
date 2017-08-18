//@	{"targets":[{"name":"giesela","type":"application"}]}

#include "renderer.hpp"
#include "mesh.hpp"
#include "uixx/uicontext.hpp"
#include "uixx/window.hpp"
#include "uixx/tpaned.hpp"
#include "uixx/sourceview.hpp"
#include "uixx/glarea.hpp"
#include "uixx/box.hpp"

using namespace Giesela;

class Application
	{
	public:
		explicit Application(UIxx::UiContext& ctx):r_ctx(ctx),m_mainwin("Giesela")
			,m_tp(m_mainwin,true
				,[](UIxx::Container& cnt)
					{return UIxx::SourceView(cnt);}
				,[](UIxx::Container& cnt)
					{return UIxx::SourceView(cnt);}
				,[](UIxx::Container& cnt)
					{return UIxx::GLArea(cnt);})
			{
			m_mainwin.callback(*this,0).show();
			m_tp.main().content("Log goes here");
			m_tp.a().content("Source goes here").lineNumbers(true).focus();
			}
		
		UIxx::UiContext::RunStatus idle(UIxx::UiContext& context)
			{return UIxx::UiContext::RunStatus::WAIT;}
		
		typedef int MessageId;
		typedef int MessageParam;
		void process(UIxx::UiContext&,MessageId,MessageParam){}
		
		void closing(UIxx::Window& win,int)
			{r_ctx.exit();}
			
		void keyDown(UIxx::Window&,int,UIxx::keymask_t,int){}
		void keyUp(UIxx::Window&,int,UIxx::keymask_t,int){}
		void focusIn(UIxx::Window&,int){}
		void focusOut(UIxx::Window&,int){}
		
	private:
		UIxx::UiContext& r_ctx;
		UIxx::Window m_mainwin;
			UIxx::TPaned<UIxx::TPanedLayout::SmallSmallLarge
				,UIxx::SourceView
				,UIxx::SourceView
				,UIxx::GLArea> m_tp;
	};
	
int main()
	{
	try
		{
		UIxx::UiContext ctx;
		Application app(ctx);
		ctx.run(app);
		}
	catch(const char* message)
		{fprintf(stderr,"Error: %s\n", message);}
	return 0;
	}
