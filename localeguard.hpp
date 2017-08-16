//@	{
//@	 "targets":[{"name":"localeguard.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"localeguard.o","rel":"implementation"}]
//@	}

#ifndef LOCALEGUARD_HPP
#define LOCALEGUARD_HPP

class LocaleGuard
	{
	public:
		LocaleGuard(const LocaleGuard&)=delete;
		LocaleGuard& operator=(const LocaleGuard&)=delete;

		explicit LocaleGuard(const char* locale_new);
		~LocaleGuard();

	private:
		char* m_locale_old;
	};

#endif
