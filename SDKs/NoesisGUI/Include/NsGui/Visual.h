////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #1213]
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VISUAL_H__
#define __GUI_VISUAL_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/DependencyObject.h>
#include <NsGui/Enums.h>
#include <NsDrawing/Point.h>
#include <NsDrawing/Size.h>
#include <NsDrawing/Rect.h>
#include <NsCore/List.h>


namespace Noesis
{

class Visual;
class DrawingCommands;
class Geometry;
class Brush;
class Transform;
class Projection;
class RenderTreeUpdater;
NS_INTERFACE IView;
template<class T> class Matrix4;
typedef Noesis::Matrix4<float> Matrix4f;
template<class T> class Transform2;
typedef Noesis::Transform2<float> Transform2f;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Indicates the visual that was hit in a HitTest operation
////////////////////////////////////////////////////////////////////////////////////////////////////
struct HitTestResult
{
    Visual* visualHit;

    HitTestResult() : visualHit(0) { }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Callback that is called each time a visual is hit in a HitTest operation
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Noesis::Delegate<void (Visual*)> HitTestDelegate;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides support for: output display (rendering the persisted, serialized drawing 
/// content of a visual), transformations (performing transformations on a visual), clipping 
/// (providing clipping region support for a visual), hit testing (determining whether a 
/// coordinate or geometry is contained within the bounds of a visual), and bounding box
/// calculations (determining the bounding rectangle of a visual).
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.visual.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Visual: public DependencyObject
{
public:
    NS_DISABLE_COPY(Visual)

    Visual();
    virtual ~Visual() = 0;

    /// Determines whether the visual object is an ancestor of the descendant visual object
    /// \remarks This object is not considered to be an ancestor of "visual" if they are the same
    bool IsAncestorOf(const Visual* visual) const;

    /// Determines whether the visual object is a descendant of the ancestor visual object
    /// \remarks This object is not considered to be a descendant of "visual" if they are the same
    bool IsDescendantOf(const Visual* visual) const;

    /// Finds the common ancestor of two visuals objects
    /// \remarks If "visual" is the same object as this, the common ancestor will be the parent
    Visual* FindCommonVisualAncestor(const Visual* visual) const;

    /// Converts a Point in screen coordinates into a Point that represents the current coordinate
    /// system of the Visual
    Point PointFromScreen(const Point& point) const;
    
    /// Converts a Point that represents the current coordinate system of the Visual into a Point
    /// in screen coordinates
    Point PointToScreen(const Point& point) const;

    /// Returns a transform that can be used to transform coordinates from the Visual to the
    /// specified ancestor of the visual object
    Noesis::Matrix4f TransformToAncestor(const Visual* ancestor) const;

    /// Returns a transform that can be used to transform coordinates from the Visual to the
    /// specified visual object descendant
    Noesis::Matrix4f TransformToDescendant(const Visual* descendant) const;

    /// Returns a transform that can be used to transform coordinates from the Visual to the
    /// specified visual object
    Noesis::Matrix4f TransformToVisual(const Visual* visual) const;

    // Raise subtree render commands changed
    typedef Noesis::Delegate<void ()> SubtreeDrawingCommandsChangedDelegate;
    SubtreeDrawingCommandsChangedDelegate& SubtreeDrawingCommandsChanged();

    /// Indicates if this visual is invalidated and should send updates to the render tree
    bool IsInvalidated() const;

    /// Indicates if this visual has been connected to a View
    bool IsConnectedToView() const;

    /// Enables or disables visual for render
    //@{
    void ConnectToView(IView* view);
    void DisconnectFromView();
    //@}

    /// Gets the view where this visual is connected to
    IView* GetView() const;

    /// Indicates if a render node has been created in the RenderTree for this visual
    bool IsInRenderTree() const;

    /// Gets render tree id
    uint32_t GetRenderTreeId() const;

    // Pushes all the layout updates into the UI render thread queue to update render nodes
    void UpdateRender(RenderTreeUpdater& updater);

    /// Sets the flag that indicates that this Visual contains a VisualBrush
    void SetVisualBrushFlag();

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Protected functions used by child classes or VisualTreeHelper
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //@{
    /// Gets the parent of the visual object
    Visual* GetVisualParent() const;

    /// Sets up the parent-child relationship between two visuals
    void AddVisualChild(Visual* child);

    /// Removes the parent-child relationship between two visuals
    void RemoveVisualChild(Visual* child);

    /// Gets the bounding box rectangle
    Rect GetContentBounds() const;

    /// Returns a bounding box that encloses this visual and all its children
    Rect GetDescendantBounds() const;

    /// Gets the root of the View tree where this visual is connected to
    /// \return Null if this visual is not connected to a View
    Visual* GetRoot() const;

    /// Gets the surface size of the View where this visual is connected to
    Size GetSurfaceSize() const;

    /// Gets or sets the BitmapScalingMode for the Visual
    //@{
    BitmapScalingMode GetVisualBitmapScalingMode() const;
    void SetVisualBitmapScalingMode(BitmapScalingMode mode);
    //@}

    /// Gets or sets the clip region of the Visual as a Geometry value
    //@{
    Geometry* GetVisualClip() const;
    void SetVisualClip(Geometry* clip);
    //@}

    /// Gets or sets the offset value of the visual object
    //@{
    const Point& GetVisualOffset() const;
    void SetVisualOffset(const Point& offset);
    //@}

    /// Gets or sets the size value of the visual object
    //@{
    const Size& GetVisualSize() const;
    void SetVisualSize(const Size& size);
    //@}

    /// Gets or sets the opacity of the Visual
    //@{
    float GetVisualOpacity() const;
    void SetVisualOpacity(float opacity);
    //@}

    /// Gets or sets the Brush value that represents the opacity mask of the Visual
    //@{
    Brush* GetVisualOpacityMask() const;
    void SetVisualOpacityMask(Brush* mask);
    //@}

    /// Gets or sets the Transform value for the Visual
    //@{
    Transform* GetVisualTransform() const;
    void SetVisualTransform(Transform* transform);
    //@}

    /// Gets or sets the Transform value for the Visual
    //@{
    Projection* GetVisualProjection() const;
    void SetVisualProjection(Projection* projection);
    //@}

    /// Invalidate visual render commands
    void InvalidateDrawingCommands() const;

    /// Indicates to the visual that content bounds changed
    void InvalidateContentBounds() const;

    /// Invalidates visual if the visual or its children intersect the invalidation rectangle
    void InvalidateRect(const Rect& rect) const;

    /// Invalidates children Z-order
    void InvalidateChildrenZOrder() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Protected virtual functions to be reimplemented by child classes
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //@{
    /// Gets the number of child elements for the Visual
    /// \remarks Each visual implementation will decide how to store visual children
    virtual uint32_t GetVisualChildrenCount() const;

    /// Returns the specified Visual in the parent VisualCollection
    /// \remarks Each visual implementation will decide how to store visual children
    virtual Visual* GetVisualChild(uint32_t index) const;

    /// Called when the VisualCollection of the visual object is modified
    virtual void OnVisualChildrenChanged(Visual* added, Visual* removed);

    /// Called when the parent of the visual object is changed
    virtual void OnVisualParentChanged(Visual* oldParent);

    /// Called when this visual is connected/disconnected to/from a parent that belongs to a View
    //@{
    virtual void OnConnectToView(IView* view);
    virtual void OnDisconnectFromView();
    //@}

    /// Called when children of this visual are connected/disconnected to/from render tree
    //@{
    virtual void OnConnectToViewChildren();
    virtual void OnDisconnectFromViewChildren();
    //@}

    /// Called when the VisualOffset of the visual object is modified
    virtual void OnVisualOffsetChanged();

    /// Called when the VisualSize of the visual object is modified
    virtual void OnVisualSizeChanged();

    /// Called when the VisualTransform of the visual object is modified
    virtual void OnVisualTransformChanged();

    /// Called when the VisualProjection of the visual object is modified
    virtual void OnVisualProjectionChanged();

    /// Gets the bounding box rectangle
    virtual Rect GetContentBoundsCore() const;

    /// Function to be implemented by Inheriting classes. This function is invoked whenever
    /// the content bound is hit in a HitTest operation
    virtual HitTestResult HitTestCore(const Point& point);

    /// Updates render commands in the corresponding render node
    virtual DrawingCommands* GetDrawingCommands() const;
    //@}

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    bool OnSubPropertyChanged(const DependencyProperty* prop);
    //@}

    // Layout requests are stored in the element
    typedef eastl::ListNodeBase* LayoutRequest;

private:
    friend struct VisualTreeHelper;
    friend class VisualCollection;
    friend class ViewLayout;

    void Invalidate();

    /// Invalidates descendant bounds on a branch
    void InvalidateBranchBounds(bool raiseDrawingCommandsChanged) const;

    /// Fires SubtreeDrawingCommandsChanged event
    void RaiseSubtreeDrawingCommandsChanged() const;

    Point ToProjectedSurface(const Point& point, const Matrix4f& m) const;
    Point FromProjectedSurface(const Point& point, const Matrix4f& m) const;

    Matrix4f InternalTransformToRoot() const;
    Matrix4f InternalTransformToAncestor(const Visual* ancestor,
        const Size& surface) const;
    Matrix4f InternalTransformToParent(const Size& surface) const;

    /// Returns the topmost Visual object of a hit test by specifying a Point in local coordinates
    HitTestResult HitTest(const Point& point);
    HitTestResult InternalHitTest(const Point& point, bool isProjectionIdentity,
        const Size& surface);

    /// Initiates a hit test on this Visual, with caller-defined HitTestDelegate allowing to
    /// retrieve all of the visuals under the specified point, not just the topmost one.
    HitTestResult HitTest(const Point& point, const HitTestDelegate& hitTestDelegate);
    HitTestResult InternalHitTest(const Point& point, bool isProjectionIdentity,
        const Size& surface, const HitTestDelegate& hitTestDelegate);

    // Pushes all the necessary commands into the UI render thread queue to update corresponding
    // render nodes
    //@{
    void UpdateRenderTree(RenderTreeUpdater& updater);
    void UpdateFlags(RenderTreeUpdater& updater);
    void UpdateBounds(RenderTreeUpdater& updater);
    void UpdateTransform(RenderTreeUpdater& updater);
    void UpdateProjection(RenderTreeUpdater& updater);
    void UpdateBitmapScalingMode(RenderTreeUpdater& updater);
    void UpdateClip(RenderTreeUpdater& updater);
    void UpdateOffset(RenderTreeUpdater& updater);
    void UpdateSize(RenderTreeUpdater& updater);
    void UpdateOpacity(RenderTreeUpdater& updater);
    void UpdateOpacityMask(RenderTreeUpdater& updater);
    void UpdateDrawingCommands(RenderTreeUpdater& updater);
    void UpdateChildren(RenderTreeUpdater& updater);

    void CreateRenderNode(RenderTreeUpdater& updater);
    void DestroyRenderNode(RenderTreeUpdater& updater);
    void AddNode(RenderTreeUpdater& updater);
    void RemoveNode(RenderTreeUpdater& updater, uint32_t parentIndex);
    void RemoveAllNodes(RenderTreeUpdater& updater);
    //@}

    /// Visual flag management
    //@{
    void SetVisualFlag(uint32_t flag) const;
    void ClearVisualFlag(uint32_t flag) const;
    bool CheckVisualFlag(uint32_t flag) const;
    //@}

    /// Manages invalidation of visual in renderer for next update
    //@{
    void AddInvalidatedVisual() const;
    void RemoveInvalidatedVisual() const;
    void CancelUpdateRequest();
    //@}

private:
    IView* mView;
    uint32_t mIndex;

    Visual* mVisualParent;
    Point mVisualOffset;
    Size mVisualSize;

    mutable Rect mContentBoundsCache;
    mutable Rect mDescendantBoundsCache;

    mutable uint32_t mVisualFlags;

    LayoutRequest mUpdateRequest;

    SubtreeDrawingCommandsChangedDelegate mSubtreeDrawingCommandsChanged;

    /// Private properties
    //@{
    static const DependencyProperty* VisualBitmapScalingModeProperty;
    static const DependencyProperty* VisualClipProperty;
    static const DependencyProperty* VisualOpacityProperty;
    static const DependencyProperty* VisualOpacityMaskProperty;
    static const DependencyProperty* VisualTransformProperty;
    static const DependencyProperty* VisualProjectionProperty;
    //@}

    NS_DECLARE_REFLECTION(Visual, DependencyObject)
};

NS_WARNING_POP

}

#endif
