////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_PATH_H__
#define __GUI_PATH_H__


#include <NsCore/Noesis.h>
#include <NsGui/Shape.h>


namespace Noesis
{

class Geometry;
class GeometryGroup;
class MatrixTransform;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Draws a series of connected lines and curves.
///
/// .. code-block:: c++
///
///    // Geometry
///    Ptr<StreamGeometry> geometry = *new StreamGeometry();
///    {
///        StreamGeometryContext context = geometry->Open();
///        context.BeginFigure(Point(260.0f, 200.0f), true);
///        context.ArcTo(Point(140.0f, 200.0f), Size(60.0f, 60.0f), 0, false, true);
///        context.ArcTo(Point(260.0f, 200.0f), Size(60.0f, 60.0f), 0, false, true);
///    }
///
///    // Path
///    Ptr<Path> path = *new Path();
///    Ptr<Brush> red = *new SolidColorBrush(Color::Red);
///    path->SetFill(red.GetPtr());
///    path->SetData(geometry.GetPtr());
///
///    // Root canvas
///    Ptr<Canvas> root = *new Canvas();
///    Ptr<Brush> gray = *new SolidColorBrush(Color::LightGray);
///    root->SetBackground(gray.GetPtr());
///    root->GetChildren()->Add(path.GetPtr());
///
/// http://msdn.microsoft.com/en-us/library/system.windows.shapes.path.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Path: public Shape
{
public:
    /// Constructor
    Path();

    /// Destructor
    ~Path();

    /// Gets or sets the geometry that specifies the shape to be drawn
    //@{
    Geometry* GetData() const;
    void SetData(Geometry* data);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* DataProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

    /// From UIElement
    //@{
    void OnRender(DrawingContext* context);
    //@}

    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize);
    Size ArrangeOverride(const Size& finalSize);
    //@}

private:
    Size GetMinimumSize() const;

private:
    Ptr<GeometryGroup> mRenderGeometry;
    Ptr<MatrixTransform> mStretchMatrix;

    NS_DECLARE_REFLECTION(Path, Shape)
};

NS_WARNING_POP

}

#endif
