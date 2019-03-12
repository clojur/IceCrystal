////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_LINEARGRADIENTBRUSH_H__
#define __GUI_LINEARGRADIENTBRUSH_H__


#include <NsCore/Noesis.h>
#include <NsGui/GradientBrush.h>
#include <NsGui/IRenderProxyCreator.h>


namespace Noesis
{

struct Point;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Paints an area with a linear gradient.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.lineargradientbrush.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API LinearGradientBrush: public GradientBrush
{
public:
    /// Constructor
    LinearGradientBrush();

    /// Gets or sets the starting coordinates of the linear gradient
    //@{
    const Point& GetStartPoint() const;
    void SetStartPoint(const Point& startPoint);
    //@}

    /// Gets or sets the ending coordinates of the linear gradient
    //@{
    const Point& GetEndPoint() const;
    void SetEndPoint(const Point& endPoint);
    //@}

    /// From Freezable
    //@{
    Ptr<LinearGradientBrush> Clone() const;
    Ptr<LinearGradientBrush> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    //@}

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* EndPointProperty;
    static const DependencyProperty* StartPointProperty;
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

private:
    enum UpdateFlags
    {
        UpdateFlags_StartPoint = GradientBrush::UpdateFlags_Sentinel,
        UpdateFlags_EndPoint
    };

    NS_DECLARE_REFLECTION(LinearGradientBrush, GradientBrush)
};

}

#endif
