////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DEPENDENCYSYSTEMAPI_H__
#define __GUI_DEPENDENCYSYSTEMAPI_H__


#include <NsCore/CompilerSettings.h>


#ifdef NS_GUI_DEPENDENCYSYSTEM_PRIVATE
    #define NS_GUI_DEPENDENCYSYSTEM_API
#else
    #ifdef NS_GUI_DEPENDENCYSYSTEM_EXPORTS
        #define NS_GUI_DEPENDENCYSYSTEM_API NS_DLL_EXPORT
    #else
        #define NS_GUI_DEPENDENCYSYSTEM_API NS_DLL_IMPORT
    #endif
#endif


#endif
