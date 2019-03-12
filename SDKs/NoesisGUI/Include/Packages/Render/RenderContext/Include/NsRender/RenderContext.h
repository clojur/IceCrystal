////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_RENDERCONTEXT_H__
#define __RENDER_RENDERCONTEXT_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsRender/RenderContextApi.h>


namespace Noesis
{

class RenderDevice;
class RenderTarget;
template<class T> class Ptr;

}

namespace NoesisApp
{

class Image;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// RenderContext implementation is in charge of the initialization of a rendering device
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_RENDER_RENDERCONTEXT_API RenderContext: public Noesis::BaseComponent
{
public:
    /// Brief description provided by the implementation
    virtual const char* Description() const = 0;

    /// When looking for the best render context, those with higher score are tried first
    virtual uint32_t Score() const = 0;

    /// Returns whether the implementation is valid
    virtual bool Validate() const = 0;

    /// Initializes the rendering context with the given window and multisampling samples
    virtual void Init(void* window, uint32_t& samples, bool vsync, bool sRGB) = 0;

    /// Returns the rendering device maintained by this context
    virtual Noesis::RenderDevice* GetDevice() const = 0;

    /// Called prior to rendering
    virtual void BeginRender() = 0;

    /// Called after the rendering
    virtual void EndRender() = 0;

    /// Should be called when the window is resized
    virtual void Resize() = 0;

    /// Returns the milliseconds taken by the last frame executed in the GPU
    virtual float GetGPUTimeMs() const = 0;

    /// Sets the clear color (in sRGB space)
    virtual void SetClearColor(float r, float g, float b, float a) = 0;

    /// Binds the render targets associated with the window swap chain and clears it
    virtual void SetDefaultRenderTarget(uint32_t width, uint32_t height) = 0;

    /// Grabs an image with the content of current render target
    virtual Noesis::Ptr<Image> CaptureRenderTarget(Noesis::RenderTarget* surface) const = 0;

    /// Copy the content of the back-buffer to the front buffer
    virtual void Swap() = 0;

    NS_DECLARE_REFLECTION(RenderContext, BaseComponent)
};

NS_RENDER_RENDERCONTEXT_API Noesis::Ptr<RenderContext> FindBestRenderContext();
NS_RENDER_RENDERCONTEXT_API Noesis::Ptr<RenderContext> CreateRenderContext(const char* name);

}

#endif
