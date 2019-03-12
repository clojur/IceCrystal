////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_MTLRENDERDEVICEAPI_H__
#define __RENDER_MTLRENDERDEVICEAPI_H__


#include <NsCore/CompilerSettings.h>


#ifdef NS_RENDER_MTLRENDERDEVICE_PRIVATE
    #define NS_RENDER_MTLRENDERDEVICE_API
#else
    #ifdef NS_RENDER_MTLRENDERDEVICE_EXPORTS
        #define NS_RENDER_MTLRENDERDEVICE_API NS_DLL_EXPORT
    #else
        #define NS_RENDER_MTLRENDERDEVICE_API NS_DLL_IMPORT
    #endif
#endif


#endif
