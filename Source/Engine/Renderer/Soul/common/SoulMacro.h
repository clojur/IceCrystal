#ifndef __SoulMACRO__H
#define __SoulMACRO__H

#if defined(_MSC_VER) && defined(DISABLE_MSVC_WARNINGS)
#pragma warning( disable : 4244 )
#pragma warning( disable : 4251 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4275 )
#pragma warning( disable : 4290 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4305 )
#pragma warning( disable : 4996 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__) || defined( __MWERKS__)
#  if defined(Soul_EXPORTS)
#    define Soul_API   __declspec(dllexport)
#  else
#    define Soul_API   __declspec(dllimport)
#endif
#endif

#endif // !__SoulMACRO__H

