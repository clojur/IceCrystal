////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_D3D11FACTORY_H__
#define __RENDER_D3D11FACTORY_H__


#include <NsCore/Noesis.h>
#include <NsRender/D3D11RenderDeviceApi.h>
#include <NsRender/RenderDevice.h>


struct ID3D11DeviceContext;
struct ID3D11Texture2D;


namespace Noesis
{

template<class T> class Ptr;

}

namespace NoesisApp
{

struct NS_RENDER_D3D11RENDERDEVICE_API D3D11Factory
{
    static Noesis::Ptr<Noesis::RenderDevice> CreateDevice(ID3D11DeviceContext* context, bool sRGB);
    static Noesis::Ptr<Noesis::Texture> WrapTexture(ID3D11Texture2D* texture, uint32_t width,
        uint32_t height, uint32_t levels, Noesis::TextureFormat::Enum format, bool isInverted);
};

}

#endif
