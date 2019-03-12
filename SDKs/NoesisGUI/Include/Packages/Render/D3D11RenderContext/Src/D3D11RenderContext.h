////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_D3D11RENDERCONTEXT_H__
#define __RENDER_D3D11RENDERCONTEXT_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsCore/Vector.h>
#include <NsCore/Queue.h>
#include <NsRender/RenderContext.h>

#if defined(NS_PLATFORM_WINDOWS_DESKTOP) || defined(NS_PLATFORM_WINRT)
    #include <d3d11.h>
#endif


namespace Noesis
{

class RenderDevice;

}

namespace NoesisApp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
class D3D11RenderContext final: public RenderContext
{
public:
    D3D11RenderContext();
    ~D3D11RenderContext();

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
    DXGI_SAMPLE_DESC GetSampleDesc(uint32_t samples) const;
    void CreateSwapChain(void* window, uint32_t& samples, bool sRGB);
    void CreateBuffers();
    void CreateQueries();
    void UpdateGPUTime();

private:
    Noesis::Ptr<Noesis::RenderDevice> mRenderer;

#ifdef NS_PLATFORM_WINDOWS_DESKTOP
    HMODULE mD3D11;
    PFN_D3D11_CREATE_DEVICE D3D11CreateDevice;
#endif

    ID3D11Device* mDevice;
    ID3D11DeviceContext* mContext;

#if defined(NS_PLATFORM_WINDOWS_DESKTOP) || defined(NS_PLATFORM_WINRT)
    IDXGISwapChain* mSwapChain;
    HWND mHwnd;
#endif

    bool mVSync;
    ID3D11RenderTargetView* mColor;
    ID3D11DepthStencilView* mDepthStencil;

    float mClearColor[4];
    D3D11_VIEWPORT mViewport;

    struct Frame
    {
        ID3D11Query* begin;
        ID3D11Query* end;
        ID3D11Query* disjoint;
    };

    Frame mFrames[6];
    uint32_t mReadFrame;
    uint32_t mWriteFrame;
    float mGPUTime;

    NS_DECLARE_REFLECTION(D3D11RenderContext, RenderContext)
};

}

#endif
