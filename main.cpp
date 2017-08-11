//@	{"targets":[{"name":"giesela","type":"application","pkgconfig_libs":["gtk+-3.0","glew"]}]}

#include "renderer.hpp"
#include <gtk/gtk.h>
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

static gboolean render(GtkWidget* area,GdkGLContext* context,void* user_data)
	{
	auto self=reinterpret_cast<std::unique_ptr<Renderer>*>(user_data);
	assert(self);
	(*self)->render();
	return TRUE;
	}
	
int main()
	{
	gtk_init(NULL,NULL);
	
	MainwindowData window_data{true};
	auto mainwin=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(mainwin,"delete-event",G_CALLBACK(delete_callback),&window_data);
	
	auto gl_area=reinterpret_cast<GtkGLArea*>( gtk_gl_area_new() );
	gtk_gl_area_set_has_depth_buffer(gl_area,TRUE);
	gtk_gl_area_set_has_stencil_buffer(gl_area,TRUE);
	gtk_gl_area_set_required_version(gl_area,4,5);
		{
		std::unique_ptr<Renderer> renderer;
		g_signal_connect(gl_area,"realize",G_CALLBACK(gl_init),&renderer);
		g_signal_connect(gl_area, "render", G_CALLBACK (render),&renderer);
		gtk_container_add(GTK_CONTAINER(mainwin),GTK_WIDGET(gl_area));
		gtk_widget_show_all(mainwin);
		while(window_data.m_running)
			{gtk_main_iteration_do(TRUE);}
			
		gtk_gl_area_make_current(gl_area); //Activate OpenGL before DTOR runs
		}
	gtk_widget_destroy(mainwin);
	
	return 0;
	}