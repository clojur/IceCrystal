////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Package.h>
#include <NsRender/MTLFactory.h>
#include <NsRender/Texture.h>

#include "MTLRenderDevice.h"


using namespace Noesis;
using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<RenderDevice> MTLFactory::CreateDevice(id<MTLDevice> device, MTLPixelFormat colorFormat,
    MTLPixelFormat depthFormat, MTLPixelFormat stencilFormat, uint32_t sampleCount)
{
    return *new MTLRenderDevice(device, colorFormat, depthFormat, stencilFormat, sampleCount);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<Texture> MTLFactory::WrapTexture(id<MTLTexture> texture, uint32_t width, uint32_t height,
    uint32_t levels, TextureFormat::Enum format, bool isInverted)
{
    return MTLRenderDevice::WrapTexture(texture, width, height, levels, format, isInverted);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLFactory::SetOffScreenCommandBuffer(RenderDevice* device_, id<MTLCommandBuffer> commands)
{
    MTLRenderDevice* device = (MTLRenderDevice*)device_;
    device->SetOffScreenCommandBuffer(commands);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void MTLFactory::SetOnScreenEncoder(RenderDevice* device_, id<MTLRenderCommandEncoder> encoder)
{
    MTLRenderDevice* device = (MTLRenderDevice*)device_;
    device->SetOnScreenEncoder(encoder);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_REGISTER_REFLECTION(Render, MTLRenderDevice)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INIT_PACKAGE(Render, MTLRenderDevice)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_SHUTDOWN_PACKAGE(Render, MTLRenderDevice)
{
}
