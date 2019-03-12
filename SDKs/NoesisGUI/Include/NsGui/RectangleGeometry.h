////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RECTANGLEGEOMETRY_H__
#define __GUI_RECTANGLEGEOMETRY_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Geometry.h>
#include <NsDrawing/Rect.h>


namespace Noesis
{

class RectangleTest;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes a two-dimensional rectangle. 
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.rectanglegeometry.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API RectangleGeometry: public Geometry
{
public:
    /// Constructor
    RectangleGeometry();

    /// Constructor
    RectangleGeometry(const Rect& rect, float rX = 0.0f, float rY = 0.0f);

    /// Gets or sets the x-radius of the ellipse use to round the corners of the rectangle.
    //@{
    float GetRadiusX() const;
    void SetRadiusX(float value);
    //@}
    
    /// Gets or sets the y-radius of the ellipse use to round the corners of the rectangle.
    //@{
    float GetRadiusY() const;
    void SetRadiusY(float value);
    //@}
    
    /// Gets or sets the dimensions of the rectangle
    //@{
    const Rect& GetRect() const;
    void SetRect(const Rect& value);
    //@}

    /// From Geometry
    //@{
    bool IsEmpty() const;
    //@}

    /// From Freezable
    //@{
    Ptr<RectangleGeometry> Clone() const;
    Ptr<RectangleGeometry> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* RadiusXProperty;
    static const DependencyProperty* RadiusYProperty;
    static const DependencyProperty* RectProperty;
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

    /// From Geometry
    //@{
    Rect GetRenderBoundsOverride(Pen* pen) const;
    bool FillContainsOverride(const Point& point) const;
    bool StrokeContainsOverride(Pen* pen, const Point& point) const;
    //@}

private:
    friend class RectangleTest;

    enum UpdateFlags
    {
        UpdateFlags_Rect = Geometry::UpdateFlags_Sentinel,
        UpdateFlags_RadiusX,
        UpdateFlags_RadiusY
    };

    NS_DECLARE_REFLECTION(RectangleGeometry, Geometry)
};

}

#endif
