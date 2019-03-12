////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_APPLICATIONLAUNCHERAPI_H__
#define __APP_APPLICATIONLAUNCHERAPI_H__


#include <NsCore/CompilerSettings.h>


#ifdef NS_APP_APPLICATIONLAUNCHER_PRIVATE
    #define NS_APP_APPLICATIONLAUNCHER_API
#else
    #ifdef NS_APP_APPLICATIONLAUNCHER_EXPORTS
        #define NS_APP_APPLICATIONLAUNCHER_API NS_DLL_EXPORT
    #else
        #define NS_APP_APPLICATIONLAUNCHER_API NS_DLL_IMPORT
    #endif
#endif


#endif
