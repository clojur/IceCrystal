////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_MTLFACTORY_H__
#define __RENDER_MTLFACTORY_H__


#ifdef __OBJC__


#include <NsCore/Noesis.h>
#include <NsRender/MTLRenderDeviceApi.h>
#include <NsRender/RenderDevice.h>

#include <Metal/Metal.h>


namespace Noesis
{

template<class T> class Ptr;

}

namespace NoesisApp
{

struct NS_RENDER_MTLRENDERDEVICE_API MTLFactory
{
    static Noesis::Ptr<Noesis::RenderDevice> CreateDevice(id<MTLDevice> device,
        MTLPixelFormat colorFormat, MTLPixelFormat depthFormat, MTLPixelFormat stencilFormat,
        uint32_t sampleCount);
    static Noesis::Ptr<Noesis::Texture> WrapTexture(id<MTLTexture> texture, uint32_t width,
        uint32_t height, uint32_t levels, Noesis::TextureFormat::Enum format, bool isInverted);
    static void SetOffScreenCommandBuffer(Noesis::RenderDevice* device, id<MTLCommandBuffer> commands);
    static void SetOnScreenEncoder(Noesis::RenderDevice* device, id<MTLRenderCommandEncoder> encoder);
};

}

#endif

#endif
