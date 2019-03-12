////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VISUALTREEHELPER_H__
#define __GUI_VISUALTREEHELPER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class Visual;
class Geometry;
template<class T> class Delegate;
struct HitTestResult;
struct Rect;
struct Point;
struct Size;

typedef Noesis::Delegate<void (Visual*)> HitTestDelegate;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides utility methods that perform common tasks involving nodes in a visual tree.
///
/// http://msdn.microsoft.com/en-us/library/System.Windows.Media.VisualTreeHelper.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API VisualTreeHelper
{
    /// Returns the root of the visual tree where the specified visual is
    static Visual* GetRoot(const Visual* visual);

    /// Returns a DependencyObject value that represents the parent of the visual object
    static Visual* GetParent(const Visual* visual);

    /// Returns the number of children that a parent visual contains
    static uint32_t GetChildrenCount(const Visual* visual);

    /// Returns the child visual object of a parent at the specified index
    static Visual* GetChild(const Visual* visual, uint32_t childIndex);

    /// Returns the cached bounding box rectangle for the Visual
    static Rect GetContentBounds(const Visual* visual);
    
    /// Returns the union of all the content bounding boxes for all the descendants of the visual
    /// object, which includes the content bounding box of the Visual
    static Rect GetDescendantBounds(const Visual* visual);

    /// Returns the offset of the Visual
    static const Point& GetOffset(const Visual* visual);

    /// Returns the offset of the Visual
    static const Size& GetSize(const Visual* visual);

    /// Returns the offset of the Visual
    static Geometry* GetClip(const Visual* visual);

    /// Returns the top-most Visual object of a hit test. The coordinate value you pass as the
    /// point parameter has to be relative to the coordinate space of the visual object
    static HitTestResult HitTest(Visual* visual, const Point& point);
    static HitTestResult HitTest(Visual* visual, const Point& point, 
        const HitTestDelegate& hitTestDelegate);
};

}

#endif
