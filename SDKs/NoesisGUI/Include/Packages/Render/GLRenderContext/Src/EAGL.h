////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_EAGL_H__
#define __RENDER_EAGL_H__


#include <NsCore/Noesis.h>
#include <OpenGLES/ES2/gl.h>


namespace NoesisApp
{

void* EAGLCreateContext(uint32_t& renderingAPI);
void EAGLDeleteContext(void* context);
bool EAGLSetCurrentContext(void* context);
bool EAGLRenderbufferStorageFromDrawable(void* context, GLenum target, void* view);
bool EAGLPresentRenderbuffer(void* context, GLenum target);

}

#endif
