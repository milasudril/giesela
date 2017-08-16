//@	{"targets":[{"name":"giesela","type":"application","pkgconfig_libs":["gtk+-3.0","glew","gtksourceview-3.0"]}]}

#include "renderer.hpp"
#include "mesh.hpp"
#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>
#include <memory>
#include <cassert>

using namespace Giesela;

struct MainwindowData
	{
	bool m_running;
	};

static gboolean delete_callback(GtkWidget* widget,GdkEvent* event,void* user_data)
	{
	auto self=reinterpret_cast<MainwindowData*>(user_data);
	self->m_running=false;
	return TRUE;
	}
	
static void gl_init(GtkWidget* gl_area,void* user_data)
	{
	auto self=reinterpret_cast<std::unique_ptr<Renderer>*>(user_data);
	auto widget=reinterpret_cast<GtkGLArea*>(gl_area);
	gtk_gl_area_make_current(widget);
	
	self->reset( new Renderer  );
	}

static gboolean render(GtkGLArea* gl_area,GdkGLContext* context,void* user_data)
	{
	auto self=reinterpret_cast<std::unique_ptr<Renderer>*>(user_data);
	assert(self);
	(*self)->render();
	return TRUE;
	}
	
static void resize(GtkGLArea* gl_area,int width,int height,void* user_data)
	{
	auto self=reinterpret_cast<std::unique_ptr<Renderer>*>(user_data);
	gtk_gl_area_make_current(gl_area);
	(*self)->viewport(width,height);
	}
	
static GtkWidget* orth_paned(GtkWidget* main,GtkWidget* a,GtkWidget* b)
	{
	auto ret=gtk_paned_new(GTK_ORIENTATION_VERTICAL);
	
	auto paned_inner=gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	
	gtk_paned_add1(GTK_PANED(paned_inner),a);
	gtk_paned_add2(GTK_PANED(paned_inner),b);
	gtk_paned_add1(GTK_PANED(ret),paned_inner);
	gtk_paned_add2(GTK_PANED(ret),main);
	
	return ret;
	}

	
int main()
	{
	try
		{
		gtk_disable_setlocale(); //GTK may break parsers
		gtk_init(NULL,NULL);
		
		MainwindowData window_data{true};
		auto mainwin=gtk_window_new(GTK_WINDOW_TOPLEVEL);
		g_signal_connect(mainwin,"delete-event",G_CALLBACK(delete_callback),&window_data);
		

		
		auto log=gtk_source_view_new();
		auto src_view=gtk_source_view_new();
		auto gl_area=reinterpret_cast<GtkGLArea*>( gtk_gl_area_new() );
		gtk_widget_set_size_request(GTK_WIDGET(gl_area),1,1); //Force GTK to send realize signal
		auto panels=orth_paned(log,src_view,GTK_WIDGET(gl_area));
		
		gtk_gl_area_set_has_depth_buffer(gl_area,TRUE);
		gtk_gl_area_set_has_stencil_buffer(gl_area,TRUE);
		gtk_gl_area_set_has_alpha(gl_area,FALSE);
		gtk_gl_area_set_required_version(gl_area,4,5);
			{
			std::unique_ptr<Renderer> renderer;
			g_signal_connect(gl_area,"realize",G_CALLBACK(gl_init),&renderer);
			g_signal_connect(gl_area,"render", G_CALLBACK(render),&renderer);
			g_signal_connect(gl_area,"resize", G_CALLBACK(resize),&renderer);
			
			gtk_container_add(GTK_CONTAINER(mainwin),GTK_WIDGET(panels));			
			gtk_widget_show_all(mainwin);
			
			assert(renderer);
			renderer->mesh(Mesh::fromWavefrontObj(stdin,"stdin"));
			
			while(window_data.m_running)
				{
				gtk_gl_area_queue_render(gl_area);
				gtk_main_iteration_do(TRUE);
				}
				
			gtk_gl_area_make_current(gl_area); //Activate OpenGL before DTOR runs
			}
		gtk_widget_destroy(mainwin);
		}
	catch(const char* message)
		{fprintf(stderr,"Error: %s\n", message);}
	return 0;
	}
