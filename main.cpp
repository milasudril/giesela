//@	{"targets":[{"name":"giesela","type":"application"}]}

#include "renderer.hpp"
#include "mesh.hpp"
#include "uixx/uicontext.hpp"
#include "uixx/window.hpp"
#include "uixx/tpaned.hpp"
#include "uixx/sourceview.hpp"
#include "uixx/glarea.hpp"
#include "uixx/box.hpp"
#include "uixx/label.hpp"
#include "uixx/button.hpp"

using namespace Giesela;

struct SourcePanel
	{
	SourcePanel(UIxx::Container& cnt):m_box(cnt,true)
		,m_label(m_box,"Fragment shader:")
		,m_src(m_box.insertMode({0,UIxx::Box::FILL|UIxx::Box::EXPAND}))
		,m_compile(m_box.insertMode({0,0}),"Compile")
		{}
		
	UIxx::Box m_box;
		UIxx::Label m_label;
		UIxx::SourceView m_src;
		UIxx::Button m_compile;
	};

class Application
	{
	public:
		explicit Application(UIxx::UiContext& ctx):r_ctx(ctx),m_mainwin("Giesela")
			,m_tp(m_mainwin,true
				,[](UIxx::Container& cnt)
					{return UIxx::SourceView(cnt);}
				,[](UIxx::Container& cnt)
					{return SourcePanel(cnt);}
				,[](UIxx::Container& cnt)
					{return UIxx::GLArea(cnt);})
			{
			m_tp.b().minSize(320,220).versionRequest(4,5).callback(*this,0);
			m_tp.a().m_src.content("Source goes here").lineNumbers(true)
				.highlight(".glslf").minSize(500,400);
			m_tp.a().m_label.alignment(0.0f);
			m_tp.main().content("Log goes here");
			
			m_mainwin.callback(*this,0).show();
			m_tp.a().m_src.focus();
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
		
		void render(UIxx::GLArea& area,int id)
			{m_renderer->render();}
			
		void resize(UIxx::GLArea& area,int id,int width,int height)
			{m_renderer->viewport(width,height);}
			
		void realize(UIxx::GLArea& area,int id)
			{
			fprintf(stderr,"Hello\n");
			area.glActivate();
			m_renderer.reset(new Renderer());
			}
		
	private:
		UIxx::UiContext& r_ctx;
		UIxx::Window m_mainwin;
			UIxx::TPaned<UIxx::TPanedLayout::SmallSmallLarge
				,UIxx::SourceView
				,SourcePanel
				,UIxx::GLArea> m_tp;
				
		std::unique_ptr<Renderer> m_renderer;
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
