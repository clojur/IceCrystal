////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SOLIDCOLORBRUSH_H__
#define __GUI_SOLIDCOLORBRUSH_H__


#include <NsCore/Noesis.h>
#include <NsGui/Brush.h>
#include <NsGui/IRenderProxyCreator.h>


namespace Noesis
{

// Forward declaration
//@{
class Color;
//@}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Paints an area with a solid color.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.solidcolorbrush.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API SolidColorBrush: public Brush
{
public:
    /// Constructor
    SolidColorBrush();

    /// Constructor with color
    SolidColorBrush(const Color& color);

    /// Gets or sets the string content of a ContentControl
    //@{
    const Color& GetColor() const;
    void SetColor(const Color& color);
    //@}

    /// Parses a string value to create an instance of a solid color brush
    static bool TryParse(const char* str, Ptr<SolidColorBrush>& result);

    /// From BaseObject
    //@{
    NsString ToString() const;
    //@}

    /// From Freezable
    //@{
    Ptr<SolidColorBrush> Clone() const;
    Ptr<SolidColorBrush> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ColorProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const;
    //@}

    /// From Brush
    //@{
    bool IsTransparentOverride() const;
    //@}

private:
    enum UpdateFlags
    {
        UpdateFlags_Color = Brush::UpdateFlags_Sentinel
    };

    NS_DECLARE_REFLECTION(SolidColorBrush, Brush)
};

}

#endif
