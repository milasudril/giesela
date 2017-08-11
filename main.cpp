//@	{"targets":[{"name":"giesela","type":"application","pkgconfig_libs":["gtk+-3.0","glew"]}]}

#include "renderer.hpp"
#include <gtk/gtk.h>
#include <memory>

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

int main()
	{
	gtk_init(NULL,NULL);
	
	MainwindowData window_data{true};
	auto mainwin=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(mainwin,"delete-event",G_CALLBACK(delete_callback),&window_data);
	
	auto gl_area=gtk_gl_area_new();
		{
		std::unique_ptr<Renderer> renderer;
		g_signal_connect(gl_area,"realize",G_CALLBACK(gl_init),&renderer);
		gtk_container_add(GTK_CONTAINER(mainwin),gl_area);
		gtk_widget_show(mainwin);
		while(window_data.m_running)
			{gtk_main_iteration_do(TRUE);}
		}
	gtk_widget_destroy(mainwin);
	
	return 0;
	}