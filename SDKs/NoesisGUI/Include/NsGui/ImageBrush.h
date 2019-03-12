////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IMAGEBRUSH_H__
#define __GUI_IMAGEBRUSH_H__


#include <NsCore/Noesis.h>
#include <NsGui/TileBrush.h>


namespace Noesis
{

class ImageSource;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Paints an area with an image.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.imagebrush.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ImageBrush: public TileBrush
{
public:
    /// Default constructor
    ImageBrush();
    ImageBrush(ImageSource* imageSource);

    /// Destructor
    ~ImageBrush();

    /// Gets or sets image source file
    //@{
    ImageSource* GetImageSource() const;
    void SetImageSource(ImageSource* imageSource);
    //@}

    /// From Freezable
    //@{
    Ptr<ImageBrush> Clone() const;
    Ptr<ImageBrush> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ImageSourceProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    bool OnSubPropertyChanged(const DependencyProperty* dp);
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const;
    //@}

private:
    enum UpdateFlags
    {
        UpdateFlags_ImageSource = TileBrush::UpdateFlags_Sentinel,
    };

    NS_DECLARE_REFLECTION(ImageBrush, TileBrush)
};

}

#endif
