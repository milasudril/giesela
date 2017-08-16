//@	{
//@	 "targets":[{"name":"statusicons.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"statusicons.o","rel":"implementation"}]
//@	}

#ifndef UIXX_STATUSICONS_HPP
#define UIXX_STATUSICONS_HPP

#include <cstddef>
#include <utility>
#include <cstdint>

namespace UIxx
	{
	enum class StatusIcon:size_t{INFORMATION,WARNING,ERROR,USER_ERROR};
	constexpr auto StatusIconEnd=static_cast<size_t>( StatusIcon::USER_ERROR ) + 1;

	std::pair<const uint8_t*,const uint8_t*> statusIcon(StatusIcon id) noexcept;
	}

#endif
