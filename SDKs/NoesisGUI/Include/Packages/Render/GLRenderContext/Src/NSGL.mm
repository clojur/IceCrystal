////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


//#undef NS_MINIMUM_LOG_LEVEL
//#define NS_MINIMUM_LOG_LEVEL 0


#include <NsCore/Noesis.h>

#ifdef NS_PLATFORM_OSX

#include "NSGL.h"
#include "BuildSettings.h"

#include <NsCore/Error.h>
#include <NsCore/Log.h>

#import <AppKit/NSOpenGL.h>


using namespace Noesis;


////////////////////////////////////////////////////////////////////////////////////////////////////
void* NoesisApp::NSGLCreateContext(uint32_t& samples, bool vsync)
{
    @autoreleasepool
    {
        NSOpenGLPixelFormatAttribute attrs[] =
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFAAccelerated,
#ifdef RENDERTARGETS_16BPP
            NSOpenGLPFAColorSize, 16,
#else
            NSOpenGLPFAColorSize, 24,
#endif

            NSOpenGLPFAMultisample,
            NSOpenGLPFASampleBuffers, samples > 1,
            NSOpenGLPFASamples, samples,

            NSOpenGLPFAAlphaSize, 0,
            NSOpenGLPFADepthSize, 0,
            NSOpenGLPFAStencilSize, 8,
            0
        };

        NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        if (pf == 0)
        {
            attrs[1] = NSOpenGLProfileVersionLegacy;
            pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        }

        NS_ASSERT(pf != 0);

        // Dump Pixel Format
        GLint cSize;
        [pf getValues:&cSize forAttribute:NSOpenGLPFAColorSize forVirtualScreen:0];

        GLint aSize;
        [pf getValues:&aSize forAttribute:NSOpenGLPFAAlphaSize forVirtualScreen:0];

        GLint zSize;
        [pf getValues:&zSize forAttribute:NSOpenGLPFADepthSize forVirtualScreen:0];

        GLint stencilSize;
        [pf getValues:&stencilSize forAttribute:NSOpenGLPFAStencilSize forVirtualScreen:0];

        GLint msaa;
        [pf getValues:&msaa forAttribute:NSOpenGLPFASamples forVirtualScreen:0];
        samples = msaa;

        NS_LOG_DEBUG(" PixelFormat: C%dA%d D%dS%d %dx", cSize, aSize, zSize, stencilSize, msaa);

        // Create Context
        NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
        [pf release];

        if (context != 0)
        {
            GLint swapInterval = vsync ? 1 : 0;
            [context setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
        }

        return context;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void NoesisApp::NSGLDeleteContext(void* context)
{
    [(NSOpenGLContext*)context release];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void NoesisApp::NSGLSetCurrentContext(void* context)
{
    [(NSOpenGLContext*)context makeCurrentContext];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void NoesisApp::NSGLClearCurrentContext()
{
    [NSOpenGLContext clearCurrentContext];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void NoesisApp::NSGLSetView(void* context, void* view)
{
    [(NSOpenGLContext*)context setView:(NSView*)view];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void NoesisApp::NSGLFlushBuffer(void* context)
{
    [(NSOpenGLContext*)context flushBuffer];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void NoesisApp::NSGLUpdate(void* context)
{
    [(NSOpenGLContext*)context update];
}

#endif
