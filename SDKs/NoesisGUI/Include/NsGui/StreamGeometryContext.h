////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_STREAMGEOMETRYCONTEXT_H__
#define __GUI_STREAMGEOMETRYCONTEXT_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/Ptr.h>
#include <NsDrawing/SVGPath.h>


namespace Noesis
{

struct Point;
struct Size;
class StreamGeometry;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines the direction an elliptical arc is drawn.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum SweepDirection
{
    /// Specifies that arcs are drawn in a counter clockwise (negative-angle) direction
    SweepDirection_Counterclockwise,
    /// Specifies that arcs are drawn in a clockwise (positive-angle) direction
    SweepDirection_Clockwise
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes a geometry using drawing commands. This class is used with the StreamGeometry class
/// to create a lightweight geometry that does not support data binding, animation, or modification.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API StreamGeometryContext
{
public:
    /// Destructor
    ~StreamGeometryContext();

    /// Specifies the starting point for a new figure
    void BeginFigure(const Point& startPoint, bool isClosed) const;

    /// Draws a straight line to the specified Point
    void LineTo(const Point& point) const;
    
    /// Draws a cubic Bezier curve to the specified point
    void CubicTo(const Point& point1, const Point& point2,
        const Point& point3) const;

    /// Draws a quadratic Bezier curve to the specified point
    void QuadraticTo(const Point& point1, const Point& point2) const;
    
    /// Draws a G1 smooth cubic Bezier curve to the specified point
    void SmoothCubicTo(const Point& point1, const Point& point2) const;

    /// Draws a G1 smooth quadratic Bezier curve to the specified point
    void SmoothQuadraticTo(const Point& point) const;

    /// Draws an arc to the specified point
    void ArcTo(const Point& point, const Size& size, float rotationAngleDeg,
        bool isLargeArc, SweepDirection sweepDirection) const;

    /// Closes this context and flushes its content so that it can be rendered. 
    void Close() const;

private:
    friend class StreamGeometry;
    StreamGeometryContext(StreamGeometry* geometry);
    StreamGeometryContext& operator=(const StreamGeometryContext&);

    void CloseFigure() const;

private:
    Ptr<StreamGeometry> mGeometry;
    mutable PathContext mPathContext;
    mutable bool mCurrentFigureClosed;
};

NS_WARNING_POP

}

#endif
