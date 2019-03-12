////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Noesis.h>
#ifdef NS_PLATFORM_IPHONE

#include "EAGL.h"
#include "BuildSettings.h"

#import <OpenGLES/EAGL.h>
#import <QuartzCore/CAEAGLLayer.h>
#include <UIKit/UIKit.h>


using namespace Noesis;


////////////////////////////////////////////////////////////////////////////////////////////////////
void* NoesisApp::EAGLCreateContext(uint32_t& renderingAPI)
{
    EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (context == nil)
    {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    }

    if (context != 0)
    {
        renderingAPI = (uint32_t)[context API];
    }

    return context;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void NoesisApp::EAGLDeleteContext(void* context)
{
    [(EAGLContext*)context release];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool NoesisApp::EAGLSetCurrentContext(void* context)
{
    return [EAGLContext setCurrentContext:(EAGLContext*)context];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool NoesisApp::EAGLRenderbufferStorageFromDrawable(void* context, GLenum target, void* view_)
{
    UIView* view = (UIView*)view_;

    CAEAGLLayer* layer = [CAEAGLLayer layer];
    layer.frame = view.layer.frame;
    layer.contentsScale = [UIScreen mainScreen].scale;

    [view.layer addSublayer: layer];

    layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
#if RENDERTARGETS_16BPP
        kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat,
#else
        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
#endif
        Nil];

    return [(EAGLContext*)context renderbufferStorage:target fromDrawable:layer];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool NoesisApp::EAGLPresentRenderbuffer(void* context, GLenum target)
{
    return [(EAGLContext*)context presentRenderbuffer:target];
}

#endif
