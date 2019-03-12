////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_IMAGE_H__
#define __RENDER_IMAGE_H__


#include <NsCore/Noesis.h>
#include <NsRender/RenderContextApi.h>
#include <NsCore/BaseComponent.h>


namespace NoesisApp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pixel data for A8R8G8B8 graphics image
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_RENDER_RENDERCONTEXT_API Image: public Noesis::BaseComponent
{
public:
    Image(uint32_t width, uint32_t height);
    ~Image();

    /// Gets image width
    uint32_t Width() const;

    /// Gets image height
    uint32_t Height() const;

    /// The number of bytes from one row of pixels to the next row of pixels
    uint32_t Stride() const;

    /// Returns internal bits of the image
    uint8_t* Data();

private:
    uint8_t* mData;
    uint32_t mWidth;
    uint32_t mHeight;
};

}

#endif
