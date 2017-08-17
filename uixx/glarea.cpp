//@	{
//@	 "targets":[{"name":"glarea.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "glarea.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace UIxx;

class GLArea::Impl final:private GLArea
	{
	public:
		explicit Impl(Container& cnt);
		~Impl();
		
		void minHeight(int h) noexcept
			{gtk_widget_set_size_request(GTK_WIDGET(m_handle),-1,h);}

		void minWidth(int w)
			{gtk_widget_set_size_request(GTK_WIDGET(m_handle),w,-1);}
			
		void minSize(int w,int h)
			{gtk_widget_set_size_request(GTK_WIDGET(m_handle),w,h);}

		void callback(const Vtable& vt,void* cb_obj,int id)
			{
			m_id=id;
			m_vt=vt;
			r_cb_obj=cb_obj;
			}

		int id() const noexcept
			{return m_id;}
					
		void glActivate()
			{gtk_gl_area_make_current(m_handle);}
			
		void versionRequest(int major,int minor)
			{gtk_gl_area_set_required_version(m_handle,major,minor);}

	private:
		int m_id;
		Vtable m_vt;
		void* r_cb_obj;	
		GtkGLArea* m_handle;
		
		static gboolean render(GtkGLArea* gl_area,GdkGLContext* context,void* user_data)
			{
			auto self=reinterpret_cast<Impl*>(user_data);
			if(self->r_cb_obj!=nullptr)
				{self->m_vt.render(self->r_cb_obj,*self);}
			return TRUE;
			}
			
		static gboolean resize(GtkGLArea* gl_area,int width,int height,void* user_data)
			{
			auto self=reinterpret_cast<Impl*>(user_data);
			if(self->r_cb_obj!=nullptr)
				{self->m_vt.resize(self->r_cb_obj,*self,width,height);}
			return TRUE;
			}
	};
	
GLArea::Impl::Impl(Container& cnt):GLArea(*this)
	{
	r_cb_obj=nullptr;
	auto gl_area=reinterpret_cast<GtkGLArea*>( gtk_gl_area_new() );
	g_signal_connect(gl_area,"render", G_CALLBACK(render),this);
	g_signal_connect(gl_area,"resize", G_CALLBACK(resize),this);
	gtk_gl_area_set_has_depth_buffer(gl_area,TRUE);
	gtk_gl_area_set_has_stencil_buffer(gl_area,TRUE);
	gtk_gl_area_set_has_alpha(gl_area,TRUE);
	g_object_ref_sink(gl_area);
	cnt.add(gl_area);
	
	m_handle=gl_area;
	}

GLArea::Impl::~Impl()
	{
	m_impl=nullptr;
	r_cb_obj=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	g_object_unref(m_handle);
	}
	
GLArea::GLArea(Container& cnt):m_impl(new Impl(cnt))
	{}

GLArea::~GLArea()
	{delete m_impl;}
	
GLArea& GLArea::minHeight(int h)
	{
	m_impl->minHeight(h);
	return *this;
	}

GLArea& GLArea::minWidth(int w)
	{
	m_impl->minWidth(w);
	return *this;
	}

GLArea& GLArea::minSize(int w,int h)
	{
	m_impl->minSize(w,h);
	return *this;
	}

int GLArea::id() const noexcept
	{return m_impl->id();}

GLArea& GLArea::glActivate()
	{
	m_impl->glActivate();
	return *this;
	}

GLArea& GLArea::versionRequest(int major,int minor)
	{
	m_impl->versionRequest(major,minor);
	return *this;
	}
