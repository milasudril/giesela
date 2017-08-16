//@	{
//@	 "targets":
//@		[{
//@		"name":"statusicons.o","type":"object"
//@		,"dependencies":
//@			[
//@				 {"ref":"message_error.png","rel":"misc"}
//@				,{"ref":"message_info.png","rel":"misc"}
//@				,{"ref":"message_warning.png","rel":"misc"}
//@				,{"ref":"message_usererror.png","rel":"misc"}
//@			]
//@		,"cxxoptions":{"cflags_extra":["fno-lto"]}
//@		}]
//@	}

#include "statusicons.hpp"

using namespace UIxx;

#include "blob.hpp"
#include "targetinclude.hpp"

UIXX_BLOB(uint8_t,message_error,UIXX_TARGET(message_error.png));
UIXX_BLOB(uint8_t,message_usererror,UIXX_TARGET(message_usererror.png));
UIXX_BLOB(uint8_t,message_info,UIXX_TARGET(message_info.png));
UIXX_BLOB(uint8_t,message_warning,UIXX_TARGET(message_warning.png));

std::pair<const uint8_t*,const uint8_t*> UIxx::statusIcon(StatusIcon id) noexcept
	{
	switch(id)
		{
		case StatusIcon::ERROR:
			return {message_error_begin,message_error_end};

		case StatusIcon::USER_ERROR:
			return {message_usererror_begin,message_usererror_end};

		case StatusIcon::INFORMATION:
			return {message_info_begin,message_info_end};

		case StatusIcon::WARNING:
			return {message_warning_begin,message_warning_end};
		}
	return {nullptr,nullptr};
	}

