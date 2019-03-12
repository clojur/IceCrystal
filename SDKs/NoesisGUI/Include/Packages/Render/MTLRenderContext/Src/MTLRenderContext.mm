////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "MTLRenderContext.h"

#include <NsCore/ReflectionImplement.h>
#include <NsCore/Category.h>
#include <NsCore/HighResTimer.h>
#include <NsCore/Log.h>
#include <NsRender/Image.h>
#include <NsRender/MTLFactory.h>
#include <NsRender/Texture.h>
#include <NsRender/RenderTarget.h>

#include <UIKit/UIKit.h>


using namespace Noesis;
using namespace NoesisApp;


#define MTL_RELEASE(o) \
    NS_MACRO_BEGIN \
        [o release]; \
        o = nil; \
    NS_MACRO_END


namespace
{

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
MTLRenderContext::MTLRenderContext(): mDevice(0), mDrawable(0), mCommandQueue(0), mCommandBuffer(0),
    mCommandEncoder(0), mMetalLayer(0), mPassDescriptor(0), mReadQuery(0), mWriteQuery(0),
    mGPUTime(0.0f)
{
    if (MTLCreateSystemDefaultDevice != nil)
    {
        mDevice = MTLCreateSystemDefaultDevice();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
MTLRenderContext::~MTLRenderContext()
{
    MTL_RELEASE(mPassDescriptor);
    MTL_RELEASE(mCommandQueue);
    MTL_RELEASE(mDevice);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const char* MTLRenderContext::Description() const
{
    return "Metal";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t MTLRenderContext::Score() const
{
    return 200;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool MTLRenderContext::Validate() const
{
    return mDevice != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderContext::Init(void* window, uint32_t& samples, bool vsync, bool sRGB)
{
    NS_LOG_DEBUG("Creating Metal render context");
    NS_LOG_DEBUG(" MTLDevice: %s", [mDevice.name UTF8String]);

    MTLPixelFormat colorFormat = sRGB ? MTLPixelFormatBGRA8Unorm_sRGB : MTLPixelFormatBGRA8Unorm;
    MTLPixelFormat depthFormat = MTLPixelFormatInvalid;
    MTLPixelFormat stencilFormat = MTLPixelFormatStencil8;
    samples = SupportedSampleCount(samples);

    UIView* view = (UIView*)window;

    mMetalLayer = [CAMetalLayer layer];
    mMetalLayer.device = mDevice;
    mMetalLayer.pixelFormat = colorFormat;
    mMetalLayer.framebufferOnly = YES;
    mMetalLayer.frame = view.layer.frame;

    const CGSize Size = view.bounds.size;
    const CGFloat ContentScale = [UIScreen mainScreen].scale;
    const CGFloat Width = Size.width * ContentScale;
    const CGFloat Height = Size.height * ContentScale;

    mMetalLayer.drawableSize = CGSizeMake(Width, Height);
    [view.layer addSublayer: mMetalLayer];

    mCommandQueue = [mDevice newCommandQueue];
    mCommandQueue.label = @"Noesis::CommandQueue";

    mPassDescriptor = [[MTLRenderPassDescriptor renderPassDescriptor] retain];
    mPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    mPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 0.0);

    MTLTextureDescriptor* textureDesc = [[MTLTextureDescriptor alloc] init];

    bool iOS9Runtime = iOSVersionEqualOrGreater("9.0.0");

    if (samples > 1)
    {
        // Color AA attachment
        textureDesc.textureType = MTLTextureType2DMultisample;
        textureDesc.pixelFormat = colorFormat;
        textureDesc.sampleCount = samples;
        textureDesc.width = (NSUInteger)Width;
        textureDesc.height = (NSUInteger)Height;

        if (iOS9Runtime)
        {
            textureDesc.resourceOptions = MTLResourceStorageModePrivate;
            textureDesc.storageMode = MTLStorageModePrivate;
            textureDesc.usage = MTLTextureUsageRenderTarget;
        }

        if ([mDevice supportsFeatureSet:MTLFeatureSet_iOS_GPUFamily1_v3])
        {
            textureDesc.storageMode = MTLStorageModeMemoryless;
        }

        id<MTLTexture> colorAA = [mDevice newTextureWithDescriptor:textureDesc];
        colorAA.label = @"Noesis::ColorAA";

        mPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionMultisampleResolve;
        mPassDescriptor.colorAttachments[0].texture = colorAA;
        MTL_RELEASE(colorAA);
    }
    else
    {
        // Color attachment
        mPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    }

    // Stencil attachment
    textureDesc.textureType = samples > 1 ? MTLTextureType2DMultisample : MTLTextureType2D;
    textureDesc.pixelFormat = stencilFormat;
    textureDesc.sampleCount = samples;
    textureDesc.width = (NSUInteger)Width;
    textureDesc.height = (NSUInteger)Height;

    if (iOS9Runtime)
    {
        textureDesc.resourceOptions = MTLResourceStorageModePrivate;
        textureDesc.storageMode = MTLStorageModePrivate;
        textureDesc.usage = MTLTextureUsageRenderTarget;
    }

    if ([mDevice supportsFeatureSet:MTLFeatureSet_iOS_GPUFamily1_v3])
    {
        textureDesc.storageMode = MTLStorageModeMemoryless;
    }

    id<MTLTexture> stencil = [mDevice newTextureWithDescriptor:textureDesc];
    stencil.label = @"Noesis::Stencil";

    mPassDescriptor.stencilAttachment.clearStencil = 0;
    mPassDescriptor.stencilAttachment.loadAction = MTLLoadActionClear;
    mPassDescriptor.stencilAttachment.storeAction = MTLStoreActionDontCare;
    mPassDescriptor.stencilAttachment.texture = stencil;
    MTL_RELEASE(stencil);

    MTL_RELEASE(textureDesc);

    mRenderer = MTLFactory::CreateDevice(mDevice, colorFormat, depthFormat, stencilFormat, samples);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
RenderDevice* MTLRenderContext::GetDevice() const
{
    return mRenderer.GetPtr();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderContext::BeginRender()
{
    mCommandBuffer = [mCommandQueue commandBuffer];
    mCommandBuffer.label = @"Noesis::Commands";

    [mCommandBuffer addScheduledHandler:^(id <MTLCommandBuffer>){ BeginQuery(); }];
    [mCommandBuffer addCompletedHandler:^(id <MTLCommandBuffer>){ EndQuery(); }];

    MTLFactory::SetOffScreenCommandBuffer(mRenderer.GetPtr(), mCommandBuffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderContext::EndRender()
{
    [mCommandEncoder endEncoding];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderContext::Resize()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
float MTLRenderContext::GetGPUTimeMs() const
{
    return mGPUTime * 1000.0f;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderContext::SetClearColor(float r, float g, float b, float a)
{
    mPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(r, g, b, a);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderContext::SetDefaultRenderTarget(uint32_t, uint32_t)
{
    mDrawable = mMetalLayer.nextDrawable;
    mDrawable.texture.label = @"Noesis::Color";

    if (mPassDescriptor.colorAttachments[0].storeAction == MTLStoreActionMultisampleResolve)
    {
        mPassDescriptor.colorAttachments[0].resolveTexture = mDrawable.texture;
    }
    else
    {
        mPassDescriptor.colorAttachments[0].texture = mDrawable.texture;
    }

    mCommandEncoder = [mCommandBuffer renderCommandEncoderWithDescriptor:mPassDescriptor];
    mCommandEncoder.label = @"Noesis::RenderCommandEncoder";

    MTLFactory::SetOnScreenEncoder(mRenderer.GetPtr(), mCommandEncoder);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<NoesisApp::Image> MTLRenderContext::CaptureRenderTarget(RenderTarget* surface) const
{
    struct MTLTexture: public Texture
    {
        id<MTLTexture> object;
    };

    id<MTLTexture> gpuTexture = ((MTLTexture*)surface->GetTexture())->object;
    uint32_t width = (uint32_t)gpuTexture.width;
    uint32_t height = (uint32_t)gpuTexture.height;

    MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:
        gpuTexture.pixelFormat width:width height:height mipmapped:NO];
    id<MTLTexture> cpuTexture = [mDevice newTextureWithDescriptor:desc];

    MTLOrigin origin = {0, 0, 0};
    MTLSize size = {width, height, 1};
    id<MTLBlitCommandEncoder> blitEncoder = [mCommandBuffer blitCommandEncoder];
    [blitEncoder copyFromTexture:gpuTexture sourceSlice:0 sourceLevel:0 sourceOrigin:origin
        sourceSize:size toTexture:cpuTexture destinationSlice:0 destinationLevel:0
        destinationOrigin:origin];
    [blitEncoder endEncoding];

    [mCommandBuffer commit];
    [mCommandBuffer waitUntilCompleted];

    Ptr<Image> image = *new Image(width, height);
    MTLRegion r = {{0, 0}, {width, height, 1}};
    [cpuTexture getBytes:image->Data() bytesPerRow:width*4 fromRegion:(r) mipmapLevel:0];
    MTL_RELEASE(cpuTexture);
    return image;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderContext::Swap()
{
    [mCommandBuffer presentDrawable:mDrawable];
    [mCommandBuffer commit];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t MTLRenderContext::SupportedSampleCount(uint32_t samples) const
{
    while (![mDevice supportsTextureSampleCount:samples])
    {
        samples--;
    }

    return samples;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderContext::BeginQuery()
{
    mQueries[mWriteQuery] = HighResTimer::Ticks();
    mWriteQuery = (mWriteQuery + 1) % NS_COUNTOF(mQueries);
    NS_ASSERT(mWriteQuery != mReadQuery);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLRenderContext::EndQuery()
{
    mGPUTime = (float)HighResTimer::Seconds(HighResTimer::Ticks() - mQueries[mReadQuery]);
    mReadQuery = (mReadQuery + 1) % NS_COUNTOF(mQueries);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_IMPLEMENT_REFLECTION(MTLRenderContext)
{
    NsMeta<TypeId>("MTLRenderContext");
    NsMeta<Category>("RenderContext");
}
