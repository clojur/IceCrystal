////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "MTLRenderDevice.h"

#include <NsRender/RenderTarget.h>
#include <NsRender/Texture.h>
#include <NsCore/Ptr.h>

#include <UIKit/UIKit.h>
#include <dispatch/data.h>


using namespace Noesis;
using namespace NoesisApp;


#if __has_feature(objc_arc)
  #error ARC must be disabled for this file!
#endif


#ifndef DYNAMIC_VB_SIZE
    #define DYNAMIC_VB_SIZE 512 * 1024
#endif

#ifndef DYNAMIC_IB_SIZE
    #define DYNAMIC_IB_SIZE 128 * 1024
#endif

#ifndef FRAMES_IN_FLIGHT
    #define FRAMES_IN_FLIGHT 3
#endif

#define MTL_RELEASE(o) \
    NS_MACRO_BEGIN \
        [o release]; \
        o = nil; \
    NS_MACRO_END

#if NS_INSTRUMENTED_ENABLED
    #define MTL_PUSH_DEBUG_GROUP(n) [mCommandEncoder pushDebugGroup: n]
    #define MTL_POP_DEBUG_GROUP() [mCommandEncoder popDebugGroup]
    #define MTL_NAME(o, ...) \
        NS_MACRO_BEGIN \
            o.label = Format(__VA_ARGS__); \
        NS_MACRO_END

    static NSString* Format(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        char message[256];
        vsprintf(message, format, args);
        NSString* string = [NSString stringWithUTF8String:message];
        va_end(args);
        return string;
    }

#else
    #define MTL_PUSH_DEBUG_GROUP(n) NS_NOOP
    #define MTL_POP_DEBUG_GROUP() NS_NOOP
    #define MTL_NAME(o, ...) NS_NOOP
#endif


namespace
{

////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t Hash(Shader shader, RenderState state)
{
    // In Metal only colorEnable and blendMode are part of the pipeline object
    RenderState mask = {{0, 1, 3, 0, 0, 0}};

    state.v = state.v & mask.v;
    return (uint16_t)(shader.v << 8 | state.v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool iOSVersionEqualOrGreater(const char* version)
{
#ifdef NS_PLATFORM_IPHONE
    NSString* systemVersion = [[UIDevice currentDevice] systemVersion];
    return ([systemVersion compare:@(version) options:NSNumericSearch] != NSOrderedAscending);
#else
    return false;
#endif
}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
class MTLTexture final: public Texture
{
public:
    MTLTexture(id<MTLTexture> object_, uint32_t width_, uint32_t height_, uint32_t levels_,
        TextureFormat::Enum format_, bool isInverted_): object(object_), width(width_),
        height(height_), levels(levels_), format(format_), isInverted(isInverted_) {}

    ~MTLTexture()
    {
        MTL_RELEASE(object);
    }

    uint32_t GetWidth() const override { return width; }
    uint32_t GetHeight() const override { return height; }
    TextureFormat::Enum GetFormat() const override { return format; };
    bool HasMipMaps() const override { return levels > 1; }
    bool IsInverted() const override { return isInverted; }

    id<MTLTexture> object;

    const uint32_t width;
    const uint32_t height;
    const uint32_t levels;
    const TextureFormat::Enum format;
    const bool isInverted;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class MTLRenderTarget final: public RenderTarget
{
public:
    MTLRenderTarget(MTLRenderPassDescriptor* passDesc_, MTLTexture* texture_, uint32_t width_,
        uint32_t height_): passDesc(passDesc_), texture(texture_), width(width_), height(height_) {}

    ~MTLRenderTarget()
    {
        MTL_RELEASE(passDesc);
    }

    Texture* GetTexture() override { return texture.GetPtr(); }

    MTLRenderPassDescriptor* passDesc;
    Ptr<MTLTexture> texture;

    const uint32_t width;
    const uint32_t height;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
MTLRenderDevice::MTLRenderDevice(id<MTLDevice> device, MTLPixelFormat colorFormat,
    MTLPixelFormat depthFormat, MTLPixelFormat stencilFormat, uint32_t sampleCount):
    mColorFormat(colorFormat), mDepthFormat(depthFormat), mStencilFormat(stencilFormat),
    mSampleCount(sampleCount), mDevice(device), mCommandBuffer(0), mCommandEncoder(0)
{
    FillCaps();

    mTextureDesc = [[MTLTextureDescriptor alloc] init];

    CreateBuffers();
    CreateSamplers();
    CreatePipelines();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MTLRenderDevice::~MTLRenderDevice()
{
    MTL_RELEASE(mTextureDesc);
    MTL_RELEASE(mDynamicVB.object);
    MTL_RELEASE(mDynamicIB.object);
    MTL_RELEASE(mClearPipeline);

    for (auto& pipeline: mPipelines)
    {
        MTL_RELEASE(pipeline.second);
    }

    for (uint32_t i = 0; i < NS_COUNTOF(mSamplers); i++)
    {
        MTL_RELEASE(mSamplers[i]);
    }

    for (uint32_t i = 0; i < NS_COUNTOF(mDepthStencilStates); i++)
    {
        MTL_RELEASE(mDepthStencilStates[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<Texture> MTLRenderDevice::WrapTexture(id<MTLTexture> texture, uint32_t width, uint32_t height,
    uint32_t levels, TextureFormat::Enum format, bool isInverted)
{
    [texture retain];
    return *new MTLTexture(texture, width, height, levels, format, isInverted);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetOffScreenCommandBuffer(id<MTLCommandBuffer> commands)
{
    mCommandBuffer = commands;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetOnScreenEncoder(id<MTLRenderCommandEncoder> encoder)
{
    mCommandEncoder = encoder;
    InvalidateStateCache();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const DeviceCaps& MTLRenderDevice::GetCaps() const
{
    return mCaps;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<RenderTarget> MTLRenderDevice::CreateRenderTarget(const char* label, uint32_t width,
    uint32_t height, uint32_t sampleCount)
{
    NS_ASSERT(mSampleCount == sampleCount);

    MTLTextureUsage usage = MTLTextureUsageRenderTarget;

    // ColorAA
    id<MTLTexture> colorAA = nil;

    if (sampleCount > 1)
    {
        colorAA = CreateAttachment(width, height, sampleCount, mColorFormat, usage);
        MTL_NAME(colorAA, "%s_AA", label);
    }

    // Stencil
    id<MTLTexture> stencil = CreateAttachment(width, height, sampleCount, mStencilFormat, usage);
    MTL_NAME(stencil, "%s_Stencil", label);

    Ptr<RenderTarget> rt = CreateRenderTarget(label, width, height, colorAA, stencil);
    MTL_RELEASE(colorAA);
    MTL_RELEASE(stencil);
    return rt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<RenderTarget> MTLRenderDevice::CloneRenderTarget(const char* label, RenderTarget* surface_)
{
    MTLRenderTarget* surface = (MTLRenderTarget*)surface_;

    id<MTLTexture> colorAA = mSampleCount > 1 ? surface->passDesc.colorAttachments[0].texture : 0;
    id<MTLTexture> stencil = surface->passDesc.stencilAttachment.texture;

    return CreateRenderTarget(label, surface->width, surface->height, colorAA, stencil);
}

namespace
{

////////////////////////////////////////////////////////////////////////////////////////////////////
MTLPixelFormat ToMTL(TextureFormat::Enum format, bool sRGB)
{
    switch (format)
    {
        case TextureFormat::BGRA8:
        case TextureFormat::BGRX8:
        {
            return sRGB? MTLPixelFormatBGRA8Unorm_sRGB : MTLPixelFormatBGRA8Unorm;
        }
        case TextureFormat::R8:
        {
            return MTLPixelFormatR8Unorm;
        }
        default:
        {
            NS_ASSERT_UNREACHABLE;
        }
    }
}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<Texture> MTLRenderDevice::CreateTexture(const char* label, uint32_t width, uint32_t height,
    uint32_t numLevels, TextureFormat::Enum format)
{
    mTextureDesc.textureType = MTLTextureType2D;
    mTextureDesc.width = width;
    mTextureDesc.height = height;
    mTextureDesc.sampleCount = 1;
    mTextureDesc.mipmapLevelCount = numLevels;
    mTextureDesc.pixelFormat = ToMTL(format, mCaps.linearRendering);

    if (miOS9Runtime)
    {
        mTextureDesc.resourceOptions = MTLResourceStorageModeShared;
        mTextureDesc.storageMode = MTLStorageModeShared;
        mTextureDesc.usage = MTLTextureUsageShaderRead;
    }

    id<MTLTexture> texture = [mDevice newTextureWithDescriptor:mTextureDesc];
    MTL_NAME(texture, label);

    return *new MTLTexture(texture, width, height, numLevels, format, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::UpdateTexture(Texture* texture_, uint32_t level, uint32_t x, uint32_t y,
    uint32_t width, uint32_t height, const void* data)
{
    MTLTexture* texture = (MTLTexture*)texture_;
    MTLRegion region = {{x, y}, {width, height, 1}};
    NSUInteger pitch = texture->format == TextureFormat::R8 ? width : width * 4;
    [texture->object replaceRegion:region mipmapLevel:level withBytes:data bytesPerRow:pitch];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::BeginRender(bool offscreen)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetRenderTarget(RenderTarget* surface_)
{
    MTLRenderTarget* surface = (MTLRenderTarget*)surface_;

    NS_ASSERT(mCommandBuffer != 0);
    mCommandEncoder = [mCommandBuffer renderCommandEncoderWithDescriptor:surface->passDesc];
    MTL_NAME(mCommandEncoder, "RenderTarget");

    InvalidateStateCache();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::BeginTile(const Tile& tile, uint32_t surfaceWidth, uint32_t surfaceHeight)
{
    MTL_PUSH_DEBUG_GROUP(@"Tile");
    MTL_PUSH_DEBUG_GROUP(@"Clear");
    SetRenderPipelineState(mClearPipeline);
    SetDepthStencilState(mDepthStencilStates[4], 0);
    SetFillMode(MTLTriangleFillModeFill);

    MTLScissorRect rect = {tile.x, surfaceHeight - (tile.y + tile.height), tile.width, tile.height};
    [mCommandEncoder setScissorRect:rect];
    [mCommandEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
    MTL_POP_DEBUG_GROUP();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::EndTile()
{
    MTL_POP_DEBUG_GROUP();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::ResolveRenderTarget(RenderTarget* surface_, const Tile* tiles, uint32_t size)
{
    [mCommandEncoder endEncoding];
    mCommandEncoder = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::EndRender()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void* MTLRenderDevice::MapVertices(uint32_t bytes)
{
    return MapBuffer(mDynamicVB, bytes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::UnmapVertices()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void* MTLRenderDevice::MapIndices(uint32_t bytes)
{
    return MapBuffer(mDynamicIB, bytes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::UnmapIndices()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::DrawBatch(const Batch& batch)
{
    SetRenderState(batch);
    SetBuffers(batch);
    SetTextures(batch);

    NSUInteger offset = batch.startIndex * 2 + mDynamicIB.drawPos;
    [mCommandEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:batch.numIndices
        indexType:MTLIndexTypeUInt16 indexBuffer:mDynamicIB.object indexBufferOffset:offset];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::InvalidateStateCache()
{
    mCachedVertexCBHash = 0;
    mCachedFragmentCBHash = 0;
    mCachedPipelineState = 0;
    mCachedDepthStencilState = 0;
    mCachedStencilRef = (uint32_t)-1;
    mCachedFillMode = (MTLTriangleFillMode)-1;
    memset(mCachedSamplerStates, 0, sizeof(mCachedSamplerStates));
    memset(mCachedTextures, 0, sizeof(mCachedTextures));

    NS_ASSERT(mCommandEncoder != 0);
    [mCommandEncoder setVertexBuffer:mDynamicVB.object offset:0 atIndex:0];
    [mCommandEncoder setCullMode:MTLCullModeNone];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetRenderPipelineState(id<MTLRenderPipelineState> pipeline)
{
    if (mCachedPipelineState != pipeline)
    {
        [mCommandEncoder setRenderPipelineState:pipeline];
        mCachedPipelineState = pipeline;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetDepthStencilState(id<MTLDepthStencilState> state, uint32_t stencilRef)
{
    if (mCachedDepthStencilState != state)
    {
        [mCommandEncoder setDepthStencilState:state];
        mCachedDepthStencilState = state;
    }

    if (mCachedStencilRef != stencilRef)
    {
        [mCommandEncoder setStencilReferenceValue:stencilRef];
        mCachedStencilRef = stencilRef;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetFillMode(MTLTriangleFillMode fillMode)
{
    if (mCachedFillMode != fillMode)
    {
        [mCommandEncoder setTriangleFillMode:fillMode];
        mCachedFillMode = fillMode;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetRenderState(const Batch& batch)
{
    NS_ASSERT(mPipelines.find(::Hash(batch.shader, batch.renderState)) != mPipelines.end());
    SetRenderPipelineState(mPipelines[::Hash(batch.shader, batch.renderState)]);

    NS_ASSERT(batch.renderState.f.stencilMode < NS_COUNTOF(mDepthStencilStates));
    SetDepthStencilState(mDepthStencilStates[batch.renderState.f.stencilMode], batch.stencilRef);

    SetFillMode(batch.renderState.f.wireframe ? MTLTriangleFillModeLines: MTLTriangleFillModeFill);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetBuffers(const Batch& batch)
{
    // Vertices
    [mCommandEncoder setVertexBufferOffset:batch.vertexOffset + mDynamicVB.drawPos atIndex:0];

    // Vertex Constants
    if (mCachedVertexCBHash != batch.projMtxHash)
    {
        [mCommandEncoder setVertexBytes:batch.projMtx length:16 * sizeof(float) atIndex:1];
        mCachedVertexCBHash = batch.projMtxHash;
    }

    // Fragment Constants
    if (batch.rgba != 0 || batch.radialGrad != 0 || batch.opacity != 0)
    {
        uint32_t hash = batch.rgbaHash ^ batch.radialGradHash ^ batch.opacityHash;
        if (mCachedFragmentCBHash != hash)
        {
            alignas(16) uint8_t buffer[128];
            uint32_t offset = 0;

            if (batch.rgba != 0)
            {
                memcpy(buffer + offset, batch.rgba, 4 * sizeof(float));
                offset += 4 * sizeof(float);
            }

            if (batch.radialGrad != 0)
            {
                memcpy(buffer + offset, batch.radialGrad, 8 * sizeof(float));
                offset += 8 * sizeof(float);
            }

            if (batch.opacity != 0)
            {
                memcpy(buffer + offset, batch.opacity, sizeof(float));
                offset += sizeof(float);
            }

            NS_ASSERT(offset != 0);
            NS_ASSERT(offset <= sizeof(buffer));
            [mCommandEncoder setFragmentBytes:buffer length:offset atIndex:0];
            mCachedFragmentCBHash = hash;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetFragmentTexture(uint32_t index, id<MTLTexture> texture)
{
    NS_ASSERT(index < NS_COUNTOF(mCachedTextures));
    if (mCachedTextures[index] != texture)
    {
        [mCommandEncoder setFragmentTexture:texture atIndex:index];
        mCachedTextures[index] = texture;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetFragmentSamplerState(uint32_t index, id<MTLSamplerState> sampler)
{
    NS_ASSERT(index < NS_COUNTOF(mCachedSamplerStates));
    if (mCachedSamplerStates[index] != sampler)
    {
        [mCommandEncoder setFragmentSamplerState:sampler atIndex:index];
        mCachedSamplerStates[index] = sampler;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetTexture(uint32_t index, MTLTexture* texture, SamplerState state)
{
    NS_ASSERT(state.v < NS_COUNTOF(mSamplers));
    SetFragmentSamplerState(index, mSamplers[state.v]);
    SetFragmentTexture(index, texture->object);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::SetTextures(const Batch& batch)
{
    if (batch.pattern != 0)
    {
        SetTexture(TextureIndex::Pattern, (MTLTexture*)batch.pattern, batch.patternSampler);
    }

    if (batch.ramps != 0)
    {
        SetTexture(TextureIndex::Ramps, (MTLTexture*)batch.ramps, batch.rampsSampler);
    }

    if (batch.image != 0)
    {
        SetTexture(TextureIndex::Image, (MTLTexture*)batch.image, batch.imageSampler);
    }

    if (batch.glyphs != 0)
    {
        SetTexture(TextureIndex::Glyphs, (MTLTexture*)batch.glyphs, batch.glyphsSampler);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
id<MTLTexture> MTLRenderDevice::CreateAttachment(uint32_t width, uint32_t height,
    uint32_t sampleCount, MTLPixelFormat format, MTLTextureUsage usage) const
{
    mTextureDesc.textureType = sampleCount > 1 ? MTLTextureType2DMultisample : MTLTextureType2D;
    mTextureDesc.width = width;
    mTextureDesc.height = height;
    mTextureDesc.sampleCount = sampleCount;
    mTextureDesc.mipmapLevelCount = 1;
    mTextureDesc.pixelFormat = format;

    if (miOS9Runtime)
    {
        mTextureDesc.resourceOptions = MTLResourceStorageModePrivate;
        mTextureDesc.storageMode = MTLStorageModePrivate;
        mTextureDesc.usage = usage;
    }

    if ([mDevice supportsFeatureSet:MTLFeatureSet_iOS_GPUFamily1_v3])
    {
        if ((usage & MTLTextureUsageShaderRead) == 0)
        {
            mTextureDesc.storageMode = MTLStorageModeMemoryless;
        }
    }

    return [mDevice newTextureWithDescriptor:mTextureDesc];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<RenderTarget> MTLRenderDevice::CreateRenderTarget(const char* label, uint32_t width,
    uint32_t height, id<MTLTexture> colorAA, id<MTLTexture> stencil) const
{
    MTLRenderPassDescriptor* passDesc = [[MTLRenderPassDescriptor renderPassDescriptor] retain];

    id<MTLTexture> color = CreateAttachment(width, height, 1, mColorFormat,
        MTLTextureUsageShaderRead | (mSampleCount == 1 ? MTLTextureUsageRenderTarget : 0));
    MTL_NAME(color, label);

    if (mSampleCount > 1)
    {
        NS_ASSERT(colorAA != 0);
        passDesc.colorAttachments[0].texture = colorAA;
        passDesc.colorAttachments[0].loadAction = MTLLoadActionDontCare;
        passDesc.colorAttachments[0].storeAction = MTLStoreActionMultisampleResolve;
        passDesc.colorAttachments[0].resolveTexture = color;
    }
    else
    {
        NS_ASSERT(colorAA == 0);
        passDesc.colorAttachments[0].texture = color;
        passDesc.colorAttachments[0].loadAction = MTLLoadActionDontCare;
        passDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
    }

    passDesc.stencilAttachment.texture = stencil;
    passDesc.stencilAttachment.loadAction = MTLLoadActionDontCare;
    passDesc.stencilAttachment.storeAction = MTLStoreActionDontCare;

    // For depth-stencil formats we also need to set the depth texture (even if unused)
    if (mDepthFormat == mStencilFormat)
    {
        passDesc.depthAttachment.texture = stencil;
        passDesc.depthAttachment.loadAction = MTLLoadActionDontCare;
        passDesc.depthAttachment.storeAction = MTLStoreActionDontCare;
    }

    Ptr<MTLTexture> texture = *new MTLTexture(color, width, height, 1, TextureFormat::BGRA8, false);
    return *new MTLRenderTarget(passDesc, texture.GetPtr(), width, height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::CreateBuffers()
{
    mDynamicVB.size = DYNAMIC_VB_SIZE * FRAMES_IN_FLIGHT;
    mDynamicVB.object = [mDevice newBufferWithLength:mDynamicVB.size options:0];
    mDynamicVB.pos = 0;
    mDynamicVB.drawPos = 0;
    MTL_NAME(mDynamicVB.object, "Noesis::Vertices");

    mDynamicIB.size = DYNAMIC_IB_SIZE * FRAMES_IN_FLIGHT;
    mDynamicIB.object = [mDevice newBufferWithLength:mDynamicIB.size options:0];
    mDynamicIB.pos = 0;
    mDynamicIB.drawPos = 0;
    MTL_NAME(mDynamicIB.object, "Noesis::Indices");
}

namespace
{

////////////////////////////////////////////////////////////////////////////////////////////////////
MTLSamplerAddressMode MTLAddressModeS(SamplerState sampler)
{
    switch (sampler.f.wrapMode)
    {
        case WrapMode::ClampToEdge:
        {
            return MTLSamplerAddressModeClampToEdge;
        }
        case WrapMode::ClampToZero:
        {
            return MTLSamplerAddressModeClampToZero;
        }
        case WrapMode::Repeat:
        {
            return MTLSamplerAddressModeRepeat;
        }
        case WrapMode::MirrorU:
        {
            return MTLSamplerAddressModeMirrorRepeat;
        }
        case WrapMode::MirrorV:
        {
            return MTLSamplerAddressModeRepeat;
        }
        case WrapMode::Mirror:
        {
            return MTLSamplerAddressModeMirrorRepeat;
        }
        default:
        {
            NS_ASSERT_UNREACHABLE;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MTLSamplerAddressMode MTLAddressModeT(SamplerState sampler)
{
    switch (sampler.f.wrapMode)
    {
        case WrapMode::ClampToEdge:
        {
            return MTLSamplerAddressModeClampToEdge;
        }
        case WrapMode::ClampToZero:
        {
            return MTLSamplerAddressModeClampToZero;
        }
        case WrapMode::Repeat:
        {
            return MTLSamplerAddressModeRepeat;
        }
        case WrapMode::MirrorU:
        {
            return MTLSamplerAddressModeRepeat;
        }
        case WrapMode::MirrorV:
        {
            return MTLSamplerAddressModeMirrorRepeat;
        }
        case WrapMode::Mirror:
        {
            return MTLSamplerAddressModeMirrorRepeat;
        }
        default:
        {
            NS_ASSERT_UNREACHABLE;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MTLSamplerMipFilter MTLMipFilter(SamplerState sampler)
{
    switch (sampler.f.mipFilter)
    {
        case MipFilter::Disabled:
        {
            return MTLSamplerMipFilterNotMipmapped;
        }
        case MipFilter::Nearest:
        {
            return MTLSamplerMipFilterNearest;
        }
        case MipFilter::Linear:
        {
            return MTLSamplerMipFilterLinear;
        }
        default:
        {
            NS_ASSERT_UNREACHABLE;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MTLSamplerMinMagFilter MTLMinMagFilter(SamplerState sampler)
{
    switch (sampler.f.minmagFilter)
    {
        case MinMagFilter::Nearest:
        {
            return MTLSamplerMinMagFilterNearest;
        }
        case MinMagFilter::Linear:
        {
            return MTLSamplerMinMagFilterLinear;
        }
        default:
        {
            NS_ASSERT_UNREACHABLE;
        }
    }
}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::CreateSamplers()
{
    memset(mSamplers, 0, sizeof(mSamplers));
    MTLSamplerDescriptor *desc = [[MTLSamplerDescriptor alloc] init];

    for (uint8_t minmag = 0; minmag < MinMagFilter::Count; minmag++)
    {
        for (uint8_t mip = 0; mip < MipFilter::Count; mip++)
        {
            for (uint8_t uv = 0; uv < WrapMode::Count; uv++)
            {
                SamplerState s = {{uv, minmag, mip}};

                desc.mipFilter = MTLMipFilter(s);
                desc.minFilter = MTLMinMagFilter(s);
                desc.magFilter = MTLMinMagFilter(s);
                desc.sAddressMode = MTLAddressModeS(s);
                desc.tAddressMode = MTLAddressModeT(s);

                NS_ASSERT(s.v < NS_COUNTOF(mSamplers));
                mSamplers[s.v] = [mDevice newSamplerStateWithDescriptor:desc];
            }
        }
    }

    MTL_RELEASE(desc);
}

namespace
{
#include "Shaders.h"
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::CreatePipelines()
{
    // Library creation
    NSError* error;
    dispatch_data_t data = dispatch_data_create(Shaders, sizeof(Shaders), 0, ^{ ; });
    id<MTLLibrary> library = [mDevice newLibraryWithData:data error:&error];
    [data autorelease];

    if (error != 0)
    {
        NS_ERROR("Library creation: %s", [error.localizedDescription UTF8String]);
    }

    MTL_NAME(library, "Noesis");

    // Vertex shader functions creation
    NSString* vShaders[] =
    {
        @"Pos_VS",
        @"PosColor_VS",
        @"PosTex0_VS",
        @"PosColorCoverage_VS",
        @"PosTex0Coverage_VS",
        @"PosColorTex1_VS",
        @"PosTex0Tex1_VS"
    };

    id <MTLFunction> vShaderFuncs[NS_COUNTOF(vShaders)];

    for (uint32_t i = 0; i < NS_COUNTOF(vShaders); i++)
    {
        vShaderFuncs[i] = [library newFunctionWithName:vShaders[i]];
        NS_ASSERT(vShaderFuncs[i] != nil);
    }

    // Vertex descriptors creation
    const uint32_t VFPos = 0;
    const uint32_t VFColor = 1;
    const uint32_t VFTex0 = 2;
    const uint32_t VFTex1 = 4;
    const uint32_t VFCoverage = 8;

    const uint8_t vFormats[] =
    {
        VFPos,
        VFPos | VFColor,
        VFPos | VFTex0,
        VFPos | VFColor | VFCoverage,
        VFPos | VFTex0  | VFCoverage,
        VFPos | VFColor | VFTex1,
        VFPos | VFTex0  | VFTex1,
    };

    MTLVertexDescriptor* vertexDescriptors[NS_COUNTOF(vFormats)];

    for (uint32_t i = 0; i < NS_COUNTOF(vFormats); i++)
    {
        NSUInteger offset = 0;
        vertexDescriptors[i] = [MTLVertexDescriptor vertexDescriptor];

        vertexDescriptors[i].attributes[0].format = MTLVertexFormatFloat2;
        vertexDescriptors[i].attributes[0].offset = offset;
        offset += 8;

        if ((vFormats[i] & VFColor) > 0)
        {
            vertexDescriptors[i].attributes[1].format = MTLVertexFormatUChar4Normalized;
            vertexDescriptors[i].attributes[1].offset = offset;
            offset += 4;
        }

        if ((vFormats[i] & VFTex0) > 0)
        {
            vertexDescriptors[i].attributes[2].format = MTLVertexFormatFloat2;
            vertexDescriptors[i].attributes[2].offset = offset;
            offset += 8;
        }

        if ((vFormats[i] & VFTex1) > 0)
        {
            vertexDescriptors[i].attributes[3].format = MTLVertexFormatFloat2;
            vertexDescriptors[i].attributes[3].offset = offset;
            offset += 8;
        }

        if ((vFormats[i] & VFCoverage) > 0)
        {
            vertexDescriptors[i].attributes[4].format = MTLVertexFormatFloat;
            vertexDescriptors[i].attributes[4].offset = offset;
            offset += 4;
        }

        vertexDescriptors[i].layouts[0].stride = offset;
    }

    // Fragment shader functions creation
    NSString* fShaders[] =
    {
        @"RGBA_FS",
        @"PathSolid_FS",
        @"PathLinear_FS",
        @"PathRadial_FS",
        @"PathPattern_FS",
        @"PathAASolid_FS",
        @"PathAALinear_FS",
        @"PathAARadial_FS",
        @"PathAAPattern_FS",
        @"ImagePaintOpacitySolid_FS",
        @"ImagePaintOpacityLinear_FS",
        @"ImagePaintOpacityRadial_FS",
        @"ImagePaintOpacityPattern_FS",
        @"TextSolid_FS",
        @"TextLinear_FS",
        @"TextRadial_FS",
        @"TextPattern_FS"
    };

    id <MTLFunction> fShaderFuncs[NS_COUNTOF(fShaders)];

    for (uint32_t i = 0; i < NS_COUNTOF(fShaders); i++)
    {
        fShaderFuncs[i] = [library newFunctionWithName:fShaders[i]];
        NS_ASSERT(fShaderFuncs[i] != nil);
    }

    // Pipeline objects creation
    struct Program
    {
        int8_t vShaderIdx;
        int8_t fShaderIdx;
        const char* label;
    };

    const Program programs[] =
    {
        { 0, 0, "RGBA" },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { 0, -1, "Mask" },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { 1, 1, "PathSolid" },
        { 2, 2, "PathLinear"},
        { 2, 3, "PathRadial" },
        { 2, 4, "PathPattern" },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { 3, 5, "PathAASolid" },
        { 4, 6, "PathAALinear" },
        { 4, 7, "PathAARadial" },
        { 4, 8, "PathAAPattern" },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { 5, 9, "ImagePaintOpacitySolid" },
        { 6, 10, "ImagePaintOpacityLinear" },
        { 6, 11, "ImagePaintOpacityRadial" },
        { 6, 12, "ImagePaintOpacityPattern" },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { -1, -1 },
        { 5, 13, "TextSolid" },
        { 6, 14, "TextLinear" },
        { 6, 15, "TextRadial" },
        { 6, 16, "TextPattern" },
    };

    MTLRenderPipelineDescriptor *pipelineDesc = [[MTLRenderPipelineDescriptor alloc] init];

    // Pipeline creation for each valid combination of shader and renderstate
    for (uint32_t i = 0; i < NS_COUNTOF(programs); i++)
    {
        const Program& program = programs[i];

        if (program.vShaderIdx == -1)
        {
            continue;
        }

        Shader shader;
        shader.v = (uint8_t)i;

        for (uint32_t j = 0; j < 256; j++)
        {
            RenderState state;
            state.v = (uint8_t)j;

            if (!IsValidState(shader, state))
            {
                continue;
            }

            eastl::pair<Pipelines::iterator, bool> f = mPipelines.insert(::Hash(shader, state));
            if (!f.second)
            {
                continue;
            }

            bool disableColor = program.fShaderIdx == -1;

            [pipelineDesc reset];
            pipelineDesc.vertexDescriptor = vertexDescriptors[program.vShaderIdx];
            pipelineDesc.vertexFunction = vShaderFuncs[program.vShaderIdx];
            pipelineDesc.fragmentFunction = disableColor ? nil: fShaderFuncs[program.fShaderIdx];
            pipelineDesc.sampleCount = mSampleCount;
            pipelineDesc.depthAttachmentPixelFormat = mDepthFormat;
            pipelineDesc.stencilAttachmentPixelFormat = mStencilFormat;
            pipelineDesc.colorAttachments[0].pixelFormat = mColorFormat;

            if (state.f.blendMode == BlendMode::SrcOver)
            {
                MTLRenderPipelineColorAttachmentDescriptor* c = pipelineDesc.colorAttachments[0];
                c.blendingEnabled = true;
                c.rgbBlendOperation = MTLBlendOperationAdd;
                c.alphaBlendOperation = MTLBlendOperationAdd;
                c.sourceRGBBlendFactor = MTLBlendFactorOne;
                c.sourceAlphaBlendFactor = MTLBlendFactorOne;
                c.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
                c.destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
            }

            MTL_NAME(pipelineDesc, "Noesis::%s%s", program.label,
                state.f.blendMode == BlendMode::Src ? "" : "_blend");

            NSError* error;
            id<MTLRenderPipelineState> pipeline =
                [mDevice newRenderPipelineStateWithDescriptor:pipelineDesc error:&error];

            if (error != 0)
            {
                NS_ERROR("Pipeline creation: %s", [error.localizedDescription UTF8String]);
            }

            f.first->second = pipeline;
        }
    }

    // Clear shader
    id <MTLFunction> clearVS = [library newFunctionWithName:@"ClearVS"];
    id <MTLFunction> clearFS = [library newFunctionWithName:@"ClearFS"];

    [pipelineDesc reset];
    pipelineDesc.vertexFunction = clearVS;
    pipelineDesc.fragmentFunction = clearFS;
    pipelineDesc.sampleCount = mSampleCount;
    pipelineDesc.depthAttachmentPixelFormat = mDepthFormat;
    pipelineDesc.stencilAttachmentPixelFormat = mStencilFormat;
    pipelineDesc.colorAttachments[0].pixelFormat = mColorFormat;
    MTL_NAME(pipelineDesc, "Noesis::Clear");

    mClearPipeline = [mDevice newRenderPipelineStateWithDescriptor:pipelineDesc error:&error];

    MTL_RELEASE(clearVS);
    MTL_RELEASE(clearFS);
    MTL_RELEASE(pipelineDesc);

    // Stencil states creation
    MTLDepthStencilDescriptor *stencilDesc = [[MTLDepthStencilDescriptor alloc] init];

    stencilDesc.backFaceStencil = nil;
    stencilDesc.frontFaceStencil = nil;
    MTL_NAME(stencilDesc, "Noesis::Stencil_Off");
    mDepthStencilStates[0] = [mDevice newDepthStencilStateWithDescriptor:stencilDesc];

    stencilDesc.backFaceStencil.stencilCompareFunction = MTLCompareFunctionEqual;
    stencilDesc.backFaceStencil.depthStencilPassOperation = MTLStencilOperationKeep;
    stencilDesc.frontFaceStencil = stencilDesc.backFaceStencil;
    MTL_NAME(stencilDesc, "Noesis::Stencil_On");
    mDepthStencilStates[1] = [mDevice newDepthStencilStateWithDescriptor:stencilDesc];

    stencilDesc.backFaceStencil.stencilCompareFunction = MTLCompareFunctionEqual;
    stencilDesc.backFaceStencil.depthStencilPassOperation = MTLStencilOperationIncrementWrap;
    stencilDesc.frontFaceStencil = stencilDesc.backFaceStencil;
    MTL_NAME(stencilDesc, "Noesis::Stencil_Incr");
    mDepthStencilStates[2] = [mDevice newDepthStencilStateWithDescriptor:stencilDesc];

    stencilDesc.backFaceStencil.stencilCompareFunction = MTLCompareFunctionEqual;
    stencilDesc.backFaceStencil.depthStencilPassOperation = MTLStencilOperationDecrementWrap;
    stencilDesc.frontFaceStencil = stencilDesc.backFaceStencil;
    MTL_NAME(stencilDesc, "Noesis::Stencil_Decr");
    mDepthStencilStates[3] = [mDevice newDepthStencilStateWithDescriptor:stencilDesc];

    stencilDesc.backFaceStencil.stencilCompareFunction = MTLCompareFunctionAlways;
    stencilDesc.backFaceStencil.depthStencilPassOperation = MTLStencilOperationZero;
    stencilDesc.frontFaceStencil = stencilDesc.backFaceStencil;
    MTL_NAME(stencilDesc, "Noesis::Stencil_Clear");
    mDepthStencilStates[4] = [mDevice newDepthStencilStateWithDescriptor:stencilDesc];

    MTL_RELEASE(stencilDesc);

    // Release transient objects
    for (uint32_t i = 0; i < NS_COUNTOF(fShaderFuncs); i++)
    {
        MTL_RELEASE(fShaderFuncs[i]);
    }

    for (uint32_t i = 0; i < NS_COUNTOF(vShaderFuncs); i++)
    {
        MTL_RELEASE(vShaderFuncs[i]);
    }

    MTL_RELEASE(library);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderDevice::FillCaps()
{
    mCaps = {};

    mCaps.centerPixelOffset = 0.0f;
    mCaps.dynamicVerticesSize = DYNAMIC_VB_SIZE;
    mCaps.dynamicIndicesSize = DYNAMIC_IB_SIZE;
    mCaps.linearRendering = mColorFormat == MTLPixelFormatBGRA8Unorm_sRGB;

    mCaps.supportedTextureFormats[TextureFormat::BGRA8] = true;
    mCaps.supportedTextureFormats[TextureFormat::BGRX8] = true;
    mCaps.supportedTextureFormats[TextureFormat::R8] = true;

    miOS9Runtime = iOSVersionEqualOrGreater("9.0.0");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void* MTLRenderDevice::MapBuffer(DynamicBuffer& buffer, uint32_t size) const
{
    NS_ASSERT(size <= buffer.size);

    // We are not making here any check for CPU-GPU synchronization. To do that we would have to
    // use fences or use command buffer completed handler. Anyway, the perfect solution would imply
    // forcing a stall in the CPU, something that is undesirable and improbable. So for now the best
    // solution is adjusting DYNAMIC_VB_SIZE, DYNAMIC_IB_SIZE and FRAMES_IN_FLIGHT accordingly.

    if (size > buffer.size - buffer.pos)
    {
        buffer.pos = 0;
    }

    buffer.drawPos = buffer.pos;
    buffer.pos += size;
    return (uint8_t*)[buffer.object contents] + buffer.drawPos;
}
