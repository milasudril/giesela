//@	{"targets":[{"name":"giesela","type":"application"}]}

#include "renderer.hpp"
#include "mesh.hpp"
#include "error.hpp"
#include "uixx/uicontext.hpp"
#include "uixx/window.hpp"
#include "uixx/tpaned.hpp"
#include "uixx/sourceview.hpp"
#include "uixx/glarea.hpp"
#include "uixx/box.hpp"
#include "uixx/label.hpp"
#include "uixx/button.hpp"
#include "uixx/filenameselect.hpp"
#include "uixx/slider.hpp"

using namespace Giesela;

struct SourcePanel
	{
	SourcePanel(UIxx::Container& cnt):m_box(cnt,true)
		,m_label(m_box,"Fragment shader:")
		,m_src(m_box.insertMode({0,UIxx::Box::FILL|UIxx::Box::EXPAND}))
			,m_buttons(m_box.insertMode({2,0}),false)
				,m_reset(m_buttons.insertMode({2,UIxx::Box::FILL|UIxx::Box::EXPAND}),"Reset")
				,m_compile(m_buttons,"Compile")
		{}
		
	UIxx::Box m_box;
		UIxx::Label m_label;
		UIxx::SourceView m_src;
		UIxx::Box m_buttons;
			UIxx::Button m_reset;
			UIxx::Button m_compile;
	};
	
struct PreviewPanel
	{
	PreviewPanel(UIxx::Container& cnt):m_box(cnt,true)
		,m_model(m_box,"Model")
			,m_color(m_box,false)
				,m_color_hue(m_color.insertMode({2,UIxx::Box::FILL|UIxx::Box::EXPAND}),false)
					,m_hue_label(m_color_hue,"Hue:")
					,m_hue_slider(m_color_hue.insertMode({0,UIxx::Box::FILL|UIxx::Box::EXPAND}),false)
				,m_color_sat(m_color,false)
					,m_sat_label(m_color_sat,"Saturation:")
					,m_sat_slider(m_color_sat.insertMode({0,UIxx::Box::FILL|UIxx::Box::EXPAND}),false)
				,m_color_lightness(m_color,false)
					,m_lightness_label(m_color_lightness,"Lightness:")
					,m_lightness_slider(m_color_lightness.insertMode({0,UIxx::Box::FILL|UIxx::Box::EXPAND}),false)
		,m_view(m_box.insertMode({0,UIxx::Box::FILL|UIxx::Box::EXPAND}))
		{}
		
	UIxx::Box m_box;
		UIxx::Button m_model;
		UIxx::Box m_color;
			UIxx::Box m_color_hue;
				UIxx::Label m_hue_label;
				UIxx::Slider m_hue_slider;
			UIxx::Box m_color_sat;
				UIxx::Label m_sat_label;
				UIxx::Slider m_sat_slider;
			UIxx::Box m_color_lightness;
				UIxx::Label m_lightness_label;
				UIxx::Slider m_lightness_slider;
		UIxx::GLArea m_view;
	};
	
struct FileDeleter
	{
	void operator()(FILE* fptr) noexcept
		{
		if(fptr!=NULL)
			{fclose(fptr);}
		}
	};


static std::pair<float,int> max(glm::vec3 v)
	{
	auto ret=std::pair<float,int>{v[0],0};
	for(int i=ret.second + 1;i < 3; ++i)
		{
		if(v[i] > ret.first)
			{ret=std::pair<float,int>{v[i],i};}
		}
	return ret;
	}

static glm::vec3 hsl2rgb(float hue,float sat,float lightness)
	{
	auto L=lightness;
	auto H=6.0f*hue;
	auto C=(1.0f - std::abs(2.0f*L - 1))*sat;
	auto X=C*(1.0f - std::abs(glm::mod(H,2.0f) - 1.0f) );
	glm::vec3 rgblut[7]=
		{
		 glm::vec3(C,X,0)
		,glm::vec3(X,C,0)
		,glm::vec3(0,C,X)
		,glm::vec3(0,X,C)
		,glm::vec3(X,0,C)
		,glm::vec3(C,0,X)
		,glm::vec3(C,X,0)
		};
	auto m=L - 0.5f*C;
	return rgblut[int(H)] + glm::vec3(m,m,m);	
	}
	
static std::tuple<float,float,float> rgb2hsl(glm::vec3 color)
	{
	auto Mp=max(color);
	auto m=std::min(std::min(color.r,color.g),color.b);
	
	auto C=Mp.first - m;
	float hue[3]={
		 glm::mod((color.g - color.b)/C,6.0f)
		,(color.b - color.r)/C + 2.0f
		,(color.r - color.g)/C + 4.0f
		};
	auto H=hue[Mp.second]/6.0f;
	auto L=0.5f*(Mp.first + m);
	auto S=L<1.0f? C/(1.0f - std::abs(2*L - 1)) : 0.0f;
	
	return std::tuple<float,float,float>{H,S,L};
	}

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
					{return PreviewPanel(cnt);})
			{
			m_tp.b().m_view.versionRequest(4,5).callback(*this,0);
			m_tp.b().m_model.callback(*this,2);
			m_tp.b().m_hue_slider.callback(*this,0);
			m_tp.b().m_sat_slider.callback(*this,1);
			m_tp.b().m_lightness_slider.callback(*this,1);
			m_tp.a().m_src.content(Renderer::defaultShader()).lineNumbers(true)
				.highlight(".glslf");
			m_tp.a().m_label.alignment(0.0f);
			
			m_mainwin.defaultSize(480+640,700).callback(*this,0).show();
			m_tp.ab().position(480);
			
			m_tp.a().m_src.focus();
			m_tp.a().m_reset.callback(*this,0);
			m_tp.a().m_compile.callback(*this,1);
			}
		
		UIxx::UiContext::RunStatus idle(UIxx::UiContext& context)
			{
			m_tp.b().m_view.redraw();
			return UIxx::UiContext::RunStatus::WAIT;
			}
		
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
			try
				{
				area.glActivate();
				m_renderer.reset(new Renderer(*this));
				
				auto hsl=rgb2hsl(m_renderer->color());
				m_tp.b().m_hue_slider.value(std::get<0>(hsl));
				m_tp.b().m_sat_slider.value(std::get<1>(hsl));
				m_tp.b().m_lightness_slider.value(std::get<2>(hsl));
				}
			catch(const Error& err)
				{log(err.message());}
			}
		
		void log(const char* message)
			{
			m_tp.main().append(message).append("\n").scrollToEnd();
			fprintf(stderr,"%s\n",message);
			}
		
		void changed(UIxx::Slider& slider,int id)
			{
			switch(id)
				{
				case 0:
				case 1:
				case 2:
					if(m_renderer)
						{
						m_tp.b().m_view.glActivate();
						auto c=hsl2rgb(m_tp.b().m_hue_slider.value()
							,m_tp.b().m_sat_slider.value()
							,m_tp.b().m_lightness_slider.value());
						m_renderer->color(c);
						}
					break;
				}
			}
	
		void clicked(UIxx::Button& btn,int id)
			{
			try
				{
				switch(id)
					{
					case 0:
						m_tp.a().m_src.content(Renderer::defaultShader());
						if(m_renderer)
							{
							m_tp.b().m_view.glActivate();
							m_renderer->shader(m_tp.a().m_src.content());
							log("(i) Shader activated");
							}
						break;
						
					case 1:
						if(m_renderer)
							{
							m_tp.b().m_view.glActivate();
							m_renderer->shader(m_tp.a().m_src.content());
							log("(i) Shader activated");
							}
						break;

					case 2:
						if(m_renderer)
							{
							std::string filename;
							if(UIxx::filenameSelect(m_mainwin,".",filename,UIxx::FilenameSelectMode::OPEN))
								{
								std::unique_ptr<FILE,FileDeleter> src(fopen(filename.c_str(),"rb"));
								m_tp.b().m_view.glActivate();
								m_renderer->mesh(Mesh::fromWavefrontObj(src.get(),filename.c_str()));
								log("(i) Model loaded");
								}
							}
						break;
					}
				}
			catch(const Error& err)
				{log(err.message());}
			btn.state(false);
			}
		
	private:
		UIxx::UiContext& r_ctx;
		UIxx::Window m_mainwin;
			UIxx::TPaned<UIxx::TPanedLayout::SmallSmallLarge
				,UIxx::SourceView
				,SourcePanel
				,PreviewPanel> m_tp;
				
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
