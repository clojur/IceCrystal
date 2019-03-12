////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_RENDERCONTEXTAPI_H__
#define __RENDER_RENDERCONTEXTAPI_H__


#include <NsCore/CompilerSettings.h>


#ifdef NS_RENDER_RENDERCONTEXT_PRIVATE
    #define NS_RENDER_RENDERCONTEXT_API
#else
    #ifdef NS_RENDER_RENDERCONTEXT_EXPORTS
        #define NS_RENDER_RENDERCONTEXT_API NS_DLL_EXPORT
    #else
        #define NS_RENDER_RENDERCONTEXT_API NS_DLL_IMPORT
    #endif
#endif


#endif
