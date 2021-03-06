#ifndef __LargescaleSceneMACRO__H
#define __LargescaleSceneMACRO__H

#include <algorithm>
#include <functional>

#if defined(_MSC_VER) && defined(DISABLE_MSVC_WARNINGS)
#pragma warning( disable : 4244 )
#pragma warning( disable : 4251 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4275 )
#pragma warning( disable : 4290 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4305 )
#pragma warning( disable : 4996 )
#pragma warning( disable: 4251 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__) || defined( __MWERKS__)
#  if defined(LargescaleScene_EXPORTS)
#    define LargescaleScene_API   __declspec(dllexport)
#  else
#    define LargescaleScene_API   __declspec(dllimport)
#endif
#endif

#endif // !__LargescaleSceneMACRO__H