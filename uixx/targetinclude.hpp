//@	{"targets":[{"name":"targetinclude.hpp","type":"include"}]}

#ifndef UIXX_TARGETINCLUDE_HPP
#define UIXX_TARGETINCLUDE_HPP

#define _UIXX_TARGET(targetdir,current_dir,x) _STRINGIFY(targetdir/current_dir/x)
#define _STRINGIFY(x) #x

#if defined(MAIKE_TARGET_DIRECTORY) && defined(MAIKE_CURRENT_DIRECTORY)
#define UIXX_TARGET(x) _UIXX_TARGET(MAIKE_TARGET_DIRECTORY,MAIKE_CURRENT_DIRECTORY,x)
#else
#error "MAIKE_TARGET_DIRECTORY or MAIKE_CURRENT_DIRECTORY not set"
#endif

#endif
