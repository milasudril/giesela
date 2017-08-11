//@	{"targets":[{"name":"giesela","type":"application","pkgconfig_libs":["gtk+-3.0"]}]}

#include <gtk/gtk.h>

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

int main()
	{
	gtk_init(NULL,NULL);
	
	MainwindowData window_data{true};
	auto mainwin=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(mainwin,"delete-event",G_CALLBACK(delete_callback),&window_data);
	
	
	gtk_widget_show(mainwin);

	while(window_data.m_running)
		{gtk_main_iteration_do(TRUE);}
	
	gtk_widget_destroy(mainwin);
	
	return 0;
	}