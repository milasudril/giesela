//@	{"targets":[{"name":"sourceview.o","type":"object","pkgconfig_libs":["gtksourceview-3.0"]}]}

#include "sourceview.hpp"
#include "container.hpp"
#include "focussink.hpp"
#include <gtksourceview/gtksource.h>
#include <cstring>

using namespace UIxx;

class SourceView::Impl:private SourceView
	{
	public:
		explicit Impl(Container& cnt);
		~Impl();

		void lineNumbers(bool status) noexcept
			{gtk_source_view_set_show_line_numbers(m_handle,status);}

		void highlight(const char* filename_pattern);

		const char* content() const;

		void content(const char* text)
			{
			auto buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
			gtk_text_buffer_set_text(buffer,text,-1);
			if(m_content!=nullptr)
				{
				g_free(m_content);
				m_content=nullptr;
				}
			}
			
		void append(const char* text)
			{
			auto buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
			GtkTextIter i;
			gtk_text_buffer_get_end_iter(buffer,&i);
			gtk_text_buffer_insert(buffer,&i,text,-1);
			if(m_content!=nullptr)
				{
				g_free(m_content);
				m_content=nullptr;
				}
			}
			
		void scrollToEnd()
			{
			auto buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
			auto mark=gtk_text_buffer_get_insert(buffer);
			GtkTextIter i;
			gtk_text_buffer_get_end_iter(buffer,&i);
			gtk_text_buffer_move_mark(buffer,mark,&i);
			gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(m_handle),mark,0,FALSE,0.0,0.0);
			}

		void readonly(bool status)
			{
			gtk_text_view_set_editable(GTK_TEXT_VIEW(m_handle),!status);
			}

		int id() const noexcept
			{return m_id;}

		void callback(Callback cb,void* cb_obj,int id)
			{
			r_cb=cb;
			r_cb_obj=cb_obj;
			m_id=id;
			}

		void wordwrap(bool status)
			{
			gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(m_handle)
				,status?GTK_WRAP_WORD:GTK_WRAP_NONE);
			}
			
		void focus()
			{gtk_widget_grab_focus(GTK_WIDGET(m_handle));}
			
		void minSize(int width,int height)
			{
			gtk_widget_set_size_request(GTK_WIDGET(m_scroll),width,height);
			}
		

	private:
		Callback r_cb;
		void* r_cb_obj;
		int m_id;
		GtkSourceView* m_handle;
		GtkScrolledWindow* m_scroll;
		mutable char* m_content;

		static gboolean focus_callback(GtkWidget* widget,GdkEvent* event,gpointer data);

		static gboolean focus_in_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
			{
			auto root=gtk_widget_get_toplevel(widget);
			auto sink=reinterpret_cast<const FocusSink*>( g_object_get_data(G_OBJECT(root),"ui-focus-sink") );
			if(sink!=nullptr)
				{sink->action(sink->object);}
			return TRUE;
			}
	};

SourceView::SourceView(Container& cnt)
	{m_impl=new Impl(cnt);}

SourceView::~SourceView()
	{delete m_impl;}

SourceView& SourceView::lineNumbers(bool status)
	{
	m_impl->lineNumbers(1);
	return *this;
	}

SourceView& SourceView::highlight(const char* filename_pattern)
	{
	m_impl->highlight(filename_pattern);
	return *this;
	}

const char* SourceView::content() const
	{return m_impl->content();}

SourceView& SourceView::content(const char* text)
	{
	m_impl->content(text);
	return *this;
	}

SourceView& SourceView::callback(Callback cb,void* cb_obj,int id)
	{
	m_impl->callback(cb,cb_obj,id);
	return *this;
	}

int SourceView::id() const noexcept
	{return m_impl->id();}

SourceView& SourceView::wordwrap(bool status)
	{
	m_impl->wordwrap(status);
	return *this;
	}
	
SourceView& SourceView::focus()
	{
	m_impl->focus();
	return *this;
	}
	
SourceView& SourceView::minSize(int width,int height)
	{
	m_impl->minSize(width,height);
	return *this;
	}

SourceView& SourceView::append(const char* text)
	{
	m_impl->append(text);
	return *this;
	}

SourceView& SourceView::scrollToEnd()
	{
	m_impl->scrollToEnd();
	return *this;
	}
	

static GtkCssProvider* s_style=nullptr;
static size_t s_style_refcount=0;


SourceView::Impl::Impl(Container& cnt):SourceView(*this)
	{
	r_cb=nullptr;
	auto widget=gtk_source_view_new();
	m_handle=GTK_SOURCE_VIEW(widget);
	auto scroll=gtk_scrolled_window_new(NULL,NULL);
	m_scroll=GTK_SCROLLED_WINDOW(scroll);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll),GTK_SHADOW_IN);
	gtk_source_view_set_tab_width(m_handle,4);

	if(s_style_refcount==0)
		{
		s_style=gtk_css_provider_new();
		gtk_css_provider_load_from_data(s_style,"*{font-size:1em; font-family:\"Inconsolata\",monospace}",-1,NULL);
		}
	++s_style_refcount;

	auto context=gtk_widget_get_style_context(widget);
	gtk_style_context_add_provider(context,GTK_STYLE_PROVIDER(s_style),
		GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	g_signal_connect(widget,"focus-out-event",G_CALLBACK(focus_callback),this);
	g_signal_connect(widget,"focus-in-event",G_CALLBACK(focus_in_callback),this);
	g_object_ref_sink(m_handle);
	gtk_container_add(GTK_CONTAINER(scroll),widget);
	g_object_ref_sink(m_scroll);
	cnt.add(scroll);
	m_content=nullptr;
	}

SourceView::Impl::~Impl()
	{
	r_cb=nullptr;
	m_impl=nullptr;
	if(m_content!=nullptr)
		{g_free(m_content);}

	if(s_style_refcount!=0)
		{
		auto context=gtk_widget_get_style_context(GTK_WIDGET(m_handle));
		gtk_style_context_remove_provider(context,GTK_STYLE_PROVIDER(s_style));
		}
	else
		{
		--s_style_refcount;
		if(s_style_refcount==0)
			{g_object_unref(s_style);}
		}

	gtk_widget_destroy(GTK_WIDGET(m_handle));
	g_object_unref(m_handle);
	gtk_widget_destroy(GTK_WIDGET(m_scroll));
	g_object_unref(m_scroll);
	}

const char* SourceView::Impl::content() const
	{
	if(m_content!=nullptr)
		{return m_content;}
	auto buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
	GtkTextIter start;
	GtkTextIter end;
	gtk_text_buffer_get_start_iter(buffer,&start);
	gtk_text_buffer_get_end_iter(buffer,&end);
	m_content=gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
	return m_content;
	}

void SourceView::Impl::highlight(const char* filename_pattern)
	{
	auto manager=gtk_source_language_manager_get_default();
	auto lang=gtk_source_language_manager_guess_language(manager,filename_pattern,NULL);
	auto buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(m_handle));
	gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(buffer),lang);
	}

gboolean SourceView::Impl::focus_callback(GtkWidget* widget,GdkEvent* event,gpointer data)
	{
	auto self=reinterpret_cast<Impl*>(data);
	if(self->m_content!=nullptr)
		{
		g_free(self->m_content);
		self->m_content=nullptr;
		}
	//We must defer the event signal until next pass, otherwise GTK widgets break
	g_idle_add([](void* impl)
		{
		auto self=reinterpret_cast<Impl*>(impl);
		if(self->r_cb!=nullptr)
			{
			self->r_cb(self->r_cb_obj,*self);
			}
		return G_SOURCE_REMOVE;
		},data);
	return FALSE;
	}
