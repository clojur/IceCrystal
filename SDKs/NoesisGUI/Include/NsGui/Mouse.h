////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MOUSE_H__
#define __GUI_MOUSE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Enums.h>
#include <NsGui/InputEnums.h>
#include <NsCore/Delegate.h>
#include <NsCore/Ptr.h>
#include <NsCore/Vector.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/BaseComponent.h>
#include <NsDrawing/Point.h>


namespace Noesis
{

NS_INTERFACE IUITreeNode;
class View;
class DependencyObject;
class UIElement;
class FrameworkElement;
class ContextMenu;
class ToolTip;
class RoutedEvent;
struct RoutedEventArgs;
struct DependencyPropertyChangedEventArgs;

////////////////////////////////////////////////////////////////////////////////////////////////////
enum CaptureMode
{
    /// No mouse capture. Mouse input goes to the element under the mouse
    CaptureMode_None,

    /// Mouse capture is applied to a single element. Mouse input goes to the captured element
    CaptureMode_Element,

    /// Mouse capture is applied to a subtree of elements. If the mouse is over a child of the
    /// element with capture, mouse input is sent to the child element. Otherwise, mouse input is
    /// sent to the element with mouse capture
    CaptureMode_SubTree
};

typedef Noesis::Delegate<void (UIElement*)> DirectlyOverChangedEvent;
typedef NsVector< Ptr<UIElement> > ParentElementList;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the mouse device.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Mouse: public BaseComponent
{
public:
    Mouse(View* view);
    ~Mouse();

    /// Resets mouse state
    void ResetState();

    /// Gets last mouse position
    Pointi GetPosition() const;

    /// Gets the mouse position relative to the specified element
    static Point GetPosition(UIElement* relativeTo);

    /// Gets the state of any mouse button
    MouseButtonState GetButtonState(MouseButton button) const;

    /// Tries to capture the mouse for the specified element
    bool Capture(UIElement* element, CaptureMode mode = CaptureMode_Element);

    /// Gets the captured mouse element
    UIElement* GetCaptured() const;

    /// Occurs when the element returned in a hit test corresponding to the position of the mouse
    /// pointer changes
    DirectlyOverChangedEvent& DirectlyOverChanged();

public:
    /// Attached routed events
    //@{
    static const RoutedEvent* GotMouseCaptureEvent;
    static const RoutedEvent* LostMouseCaptureEvent;
    static const RoutedEvent* MouseDownEvent;
    static const RoutedEvent* MouseEnterEvent;
    static const RoutedEvent* MouseLeaveEvent;
    static const RoutedEvent* MouseMoveEvent;
    static const RoutedEvent* MouseUpEvent;
    static const RoutedEvent* MouseWheelEvent;
    static const RoutedEvent* PreviewMouseDownEvent;
    static const RoutedEvent* PreviewMouseMoveEvent;
    static const RoutedEvent* PreviewMouseUpEvent;
    static const RoutedEvent* PreviewMouseWheelEvent;
    static const RoutedEvent* QueryCursorEvent;
    //@}

private:
    friend class View;
    friend class ViewLayout;
    friend class UIElement;
    friend class FrameworkElement;

    void ResetDirectlyOver();

    /// Indicates if mouse requires to refresh over/captured element state
    bool NeedsUpdate() const;
    void Update();

    void UpdateCursor();

    void RaiseButtonDownEvents(UIElement* element, MouseButton button);
    void ButtonDown(int x, int y, MouseButton button);
    void ButtonUp(int x, int y, MouseButton button);
    void DoubleClick(int x, int y, MouseButton button);
    void Move(int x, int y);
    void Wheel(int x, int y, int wheelRotation);
    void HWheel(int x, int y, int wheelRotation);
    void Scroll(float value);
    void HScroll(float value);

    void RaiseWheel(UIElement* element, int wheelRotation);

    struct HitInfo
    {
        Ptr<UIElement> enabledElement;
        Ptr<UIElement> hitVisibleElement;
    };

    void SetOver(const HitInfo& hit);

    void OnLastPressedControlDestroyed(DependencyObject* dob);

    void DirectlyOverModified(BaseComponent* source, 
        const DependencyPropertyChangedEventArgs& args);
    void UpdateOver();

    void OnCapturedIsEnabledChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);
    void OnCapturedIsVisibleChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);
    void OnCapturedIsHitTestVisibleChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);
    void UpdateCaptured();

    void RegisterCaptured();
    void UnregisterCaptured();

    void OpenContextMenu(UIElement* element, const Point& mousePosition);
    void OnContextMenuClosed(BaseComponent* sender, const RoutedEventArgs& args);

    void OpenToolTip(UIElement* element);
    void TryOpenToolTip();
    void TryCloseToolTip();
    void OnToolTipClosed(BaseComponent* sender, const RoutedEventArgs& args);
    void CancelToolTipTimer();
    void ResetToolTip();

    void RegisterToolTipOwner(UIElement* owner);
    void UnregisterToolTipOwner();
    void OnToolTipOwnerHidden(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);
    void OnToolTipOwnerDestroyed(DependencyObject* obj);

    uint32_t OnToolTipInitialTimeElapsed(void* state = 0);
    uint32_t OnToolTipDurationTimeElapsed(void* state = 0);
    uint32_t OnToolTipBetweenTimeElapsed(void* state = 0);

    HitInfo HitTest(const Point& point) const;
    HitInfo HitTest(View* view, const Point& point) const;
    struct HitTestContext;

    void UpdatePosition(int x, int y);
    bool UpdateButton(MouseButton button, MouseButtonState state);

private:
    View* mView;

    // Last mouse state
    Point mPosition;
    
    static const uint32_t NumButtons = 5;
    MouseButtonState mButtonStates[NumButtons];

    typedef NsVector<Ptr<UIElement> > Elements;

    // Captured element
    Ptr<UIElement> mCaptured;
    Elements mCaptureWithinElements;
    CaptureMode mCaptureMode;

    // Last top element under the mouse
    Ptr<UIElement> mDirectlyOver;
    Elements mOverElements;
    DirectlyOverChangedEvent mDirectlyOverChangedEvent;

    // Current cursor icon
    Cursor mCursor;

    // Filled by ButtonDown() and reused by DoubleClick()
    UIElement* mLastPressedControl;

    ContextMenu* mContextMenu;
    IUITreeNode* mContextMenuOwner;

    // TODO: Move this management to ToolTipService
    //@{
    FrameworkElement* mLastToolTipOwner;
    Ptr<ToolTip> mToolTip;
    uint32_t mToolTipTimer;
    //@}

    union
    {
        struct
        {
            bool capturedRegistered : 1;
            bool contextMenuOpened : 1;
            bool toolTipOwn : 1;
            bool toolTipBetween : 1;
            bool toolTipTimerCreated : 1;
            bool needsUpdateOver : 1;
            bool needsUpdateCaptured : 1;
        } mFlags;

        // To quickly set all flags to 0
        uint32_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(Mouse, BaseComponent)
};

NS_WARNING_POP

}

#endif
