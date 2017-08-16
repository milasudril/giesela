//@	{
//@	 "targets":[{"name":"filenameselect.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"filenameselect.o","rel":"implementation"}]
//@	}

#ifndef UIXX_FILENAMESELECT_HPP
#define UIXX_FILENAMESELECT_HPP

#include "container.hpp"

#include <utility>
#include <string>
#include <memory>

namespace UIxx
	{
	enum class FilenameSelectMode:int{OPEN,SAVE};

	typedef bool (*FilenameSelectFilter)(void* cb_obj,const char* filename_check);

	bool filenameSelect(const Container& cnt,const char* working_dir
		,std::string& filename_in,FilenameSelectMode mode
		,FilenameSelectFilter cb,void* cb_obj,const char* filter_name);

	inline bool filenameSelect(const Container& cnt,const char* working_dir
		,std::string& filename_in,FilenameSelectMode mode)
		{
		auto cb=[](void* cb_obj,const char* filename_check)
			{return true;};
		return filenameSelect(cnt,working_dir,filename_in,mode,cb,nullptr,"");
		}

	template<class FilterCallback>
	inline bool filenameSelect(const Container& cnt,const char* working_dir
		,std::string& filename_in,FilenameSelectMode mode
		,FilterCallback&& filter,const char* filter_name)
		{
		auto cb=[](void* cb_obj,const char* filename_check)
			{
			auto filter=reinterpret_cast<FilterCallback*>(cb_obj);
			return (*filter)(filename_check);
			};
		return filenameSelect(cnt,working_dir,filename_in,mode,cb,&filter,filter_name);
		}
	}

#endif
