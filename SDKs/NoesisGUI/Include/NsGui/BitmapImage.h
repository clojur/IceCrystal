////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BITMAPIMAGE_H__
#define __GUI_BITMAPIMAGE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BitmapSource.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/String.h>


namespace Noesis
{

class BitmapImageTest;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a BitmapSource created from an image file located at the specifed URI.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.media.imaging.bitmapimage.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BitmapImage: public BitmapSource
{
public:
    BitmapImage();
    BitmapImage(const char* uriSource);
    ~BitmapImage();

    /// Gets or sets the image URI source
    //@{
    const char* GetUriSource() const;
    void SetUriSource(const char* uriSource);
    //@}

    /// From Freezable
    //@{
    Ptr<BitmapImage> Clone() const;
    Ptr<BitmapImage> CloneCurrentValue() const;
    //@}

private:
    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    void UnregisterRenderer(ViewId viewId);
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const;
    void CloneCommonCore(const Freezable* source);
    //@}

    /// From BitmapSource
    //@{
    float GetDpiXCore() const;
    float GetDpiYCore() const;
    int32_t GetPixelWidthCore() const;
    int32_t GetPixelHeightCore() const;
    //@}

    void UpdateImageInfo();

private:
    friend BitmapImageTest;

    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_Source
    };

    NsString mUriSource;
    float mDpiX;
    float mDpiY;
    int32_t mPixelWidth;
    int32_t mPixelHeight;

    NS_DECLARE_REFLECTION(BitmapImage, BitmapSource)
};

NS_WARNING_POP

}

#endif
