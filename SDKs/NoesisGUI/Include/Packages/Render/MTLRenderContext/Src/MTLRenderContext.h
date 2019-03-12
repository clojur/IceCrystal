////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_MTLRENDERCONTEXT_H__
#define __RENDER_MTLRENDERCONTEXT_H__


#include <NsCore/Noesis.h>
#include <NsCore/Ptr.h>
#include <NsRender/RenderContext.h>

#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>


namespace NoesisApp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
class MTLRenderContext final: public RenderContext
{
public:
    MTLRenderContext();
    ~MTLRenderContext();

    /// From RenderContext
    //@{
    const char* Description() const override;
    uint32_t Score() const override;
    bool Validate() const override;
    void Init(void* window, uint32_t& samples, bool vsync, bool sRGB) override;
    Noesis::RenderDevice* GetDevice() const override;
    void BeginRender() override;
    void EndRender() override;
    void Resize() override;
    float GetGPUTimeMs() const override;
    void SetClearColor(float r, float g, float b, float a) override;
    void SetDefaultRenderTarget(uint32_t width, uint32_t height) override;
    Noesis::Ptr<Image> CaptureRenderTarget(Noesis::RenderTarget* surface) const override;
    void Swap() override;
    //@}

private:
    uint32_t SupportedSampleCount(uint32_t samples) const;
    void BeginQuery();
    void EndQuery();

private:
    id<MTLDevice> mDevice;
    id<CAMetalDrawable> mDrawable;
    id<MTLCommandQueue> mCommandQueue;
    id<MTLCommandBuffer> mCommandBuffer;
    id<MTLRenderCommandEncoder> mCommandEncoder;
    CAMetalLayer* mMetalLayer;
    MTLRenderPassDescriptor* mPassDescriptor;

    uint64_t mQueries[6];
    uint32_t mReadQuery;
    uint32_t mWriteQuery;
    float mGPUTime;

    Noesis::Ptr<Noesis::RenderDevice> mRenderer;

    NS_DECLARE_REFLECTION(MTLRenderContext, RenderContext)
};

}

#endif
