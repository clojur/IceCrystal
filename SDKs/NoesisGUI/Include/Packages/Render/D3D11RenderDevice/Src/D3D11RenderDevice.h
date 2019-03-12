////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_D3D11RENDERDEVICE_H__
#define __RENDER_D3D11RENDERDEVICE_H__


#include <NsRender/RenderDevice.h>

#if defined(NS_PLATFORM_WINDOWS_DESKTOP) || defined(NS_PLATFORM_WINRT)
    #include <d3d11_1.h>
#endif


namespace Noesis
{

template<class T> class Ptr;

}

namespace NoesisApp
{

struct MSAA
{
    enum Enum
    {
        x1,
        x2,
        x4,
        x8,
        x16,

        Count
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// D3D11RenderDevice
////////////////////////////////////////////////////////////////////////////////////////////////////
class D3D11RenderDevice final: public Noesis::RenderDevice
{
public:
    D3D11RenderDevice(ID3D11DeviceContext* context, bool sRGB = false);
    ~D3D11RenderDevice();

    // Creates a Noesis texture from a D3D11 texture. Reference count is incremented by one
    static Noesis::Ptr<Noesis::Texture> WrapTexture(ID3D11Texture2D* texture, uint32_t width,
        uint32_t height, uint32_t levels, Noesis::TextureFormat::Enum format, bool isInverted);

private:
    /// From RenderDevice
    //@{
    const Noesis::DeviceCaps& GetCaps() const override;
    Noesis::Ptr<Noesis::RenderTarget> CreateRenderTarget(const char* label, uint32_t width,
        uint32_t height, uint32_t sampleCount) override;
    Noesis::Ptr<Noesis::RenderTarget> CloneRenderTarget(const char* label,
        Noesis::RenderTarget* surface) override;
    Noesis::Ptr<Noesis::Texture> CreateTexture(const char* label, uint32_t width, uint32_t height,
        uint32_t numLevels, Noesis::TextureFormat::Enum format) override;
    void UpdateTexture(Noesis::Texture* texture, uint32_t level, uint32_t x, uint32_t y,
        uint32_t width, uint32_t height, const void* data) override;
    void BeginRender(bool offscreen) override;
    void SetRenderTarget(Noesis::RenderTarget* surface) override;
    void BeginTile(const Noesis::Tile& tile, uint32_t surfaceWidth,
        uint32_t surfaceHeight) override;
    void EndTile() override;
    void ResolveRenderTarget(Noesis::RenderTarget* surface, const Noesis::Tile* tiles,
        uint32_t numTiles) override;
    void EndRender() override;
    void* MapVertices(uint32_t bytes) override;
    void UnmapVertices() override;
    void* MapIndices(uint32_t bytes) override;
    void UnmapIndices() override;
    void DrawBatch(const Noesis::Batch& batch) override;
    //@}

private:
    struct DynamicBuffer;
    void CreateDynamicBuffer(DynamicBuffer& buffer, uint32_t size, D3D11_BIND_FLAG f);
    void CreateBuffers();
    struct VertexStage;
    void CreateVertexStage(uint8_t format, const char* label, const void* code, uint32_t size,
        VertexStage& stage);
    void CreateStateObjects();
    void CreateShaders();
    Noesis::Ptr<Noesis::RenderTarget> CreateRenderTarget(const char* label, uint32_t width,
        uint32_t height, MSAA::Enum msaa, ID3D11Texture2D* colorAA, ID3D11Texture2D* stencil);

    void CheckMultisample();
    void FillCaps(bool sRGB);

    void* MapDynamicBuffer(DynamicBuffer& buffer, uint32_t size) const;
    void UnmapDynamicBuffer(DynamicBuffer& buffer) const;

    void InvalidateStateCache();

    void SetInputLayout(ID3D11InputLayout* layout);
    void SetVertexShader(ID3D11VertexShader* shader);
    void SetPixelShader(ID3D11PixelShader* shader);
    void SetRasterizerState(ID3D11RasterizerState* state);
    void SetBlendState(ID3D11BlendState* state);
    void SetDepthStencilState(ID3D11DepthStencilState* state, unsigned int stencilRef);
    void SetSampler(unsigned int slot, ID3D11SamplerState* sampler);
    void SetTexture(unsigned int slot, ID3D11ShaderResourceView* texture);
    void ClearRenderTarget();
    void ClearTextures();

    void SetShaders(const Noesis::Batch& batch);
    void SetBuffers(const Noesis::Batch& batch);
    void SetRenderState(const Noesis::Batch& batch);
    void SetTextures(const Noesis::Batch& batch);

private:
    ID3D11Device* mDevice;
    ID3D11DeviceContext* mContext;
    ID3DUserDefinedAnnotation* mGroupMarker;

    DXGI_SAMPLE_DESC mSampleDescs[MSAA::Count];
    Noesis::DeviceCaps mCaps;

    struct DynamicBuffer
    {
        uint32_t size;
        uint32_t pos;
        uint32_t drawPos;
        ID3D11Buffer* buffer;
    };

    DynamicBuffer mVertices;
    DynamicBuffer mIndices;
    ID3D11Buffer* mVertexCB;
    uint32_t mVertexCBHash;
    ID3D11Buffer* mPixelCB;
    uint32_t mPixelCBHash;

    ID3D11InputLayout* mResolveLayout;

    struct VertexStage
    {
        ID3D11VertexShader* shader;
        ID3D11InputLayout* layout;
        uint32_t stride;
    };

    VertexStage mVertexStages[7];
    ID3D11PixelShader* mPixelShaders[18];
    ID3D11VertexShader* mQuadVS;
    ID3D11PixelShader* mClearPS;
    ID3D11PixelShader* mResolvePS[MSAA::Count - 1];

    ID3D11RasterizerState* mRasterizerStates[4];
    ID3D11BlendState* mBlendStates[4];
    ID3D11DepthStencilState* mDepthStencilStates[5];
    ID3D11SamplerState* mSamplerStates[64];

    Noesis::RenderTarget* mBoundRenderTarget;

    struct TextureSlot
    {
        enum Enum
        {
            Pattern,
            Ramps,
            Image,
            Glyphs,

            Count
        };
    };

    //Cached state
    //@{
    ID3D11InputLayout* mLayout;
    ID3D11VertexShader* mVertexShader;
    ID3D11PixelShader* mPixelShader;
    ID3D11RasterizerState* mRasterizerState;
    ID3D11BlendState* mBlendState;
    ID3D11DepthStencilState* mDepthStencilState;
    unsigned int mStencilRef;
    ID3D11ShaderResourceView* mTextures[TextureSlot::Count];
    ID3D11SamplerState* mSamplers[TextureSlot::Count];
    //@}
};

}

#endif
