////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_NSGL_H__
#define __RENDER_NSGL_H__


#include <NsCore/Noesis.h>


namespace NoesisApp
{

void* NSGLCreateContext(uint32_t& samples, bool vsync);
void NSGLDeleteContext(void* context);
void NSGLSetCurrentContext(void* context);
void NSGLClearCurrentContext();
void NSGLSetView(void* context, void* view);
void NSGLFlushBuffer(void* context);
void NSGLUpdate(void* context);

}

#endif
