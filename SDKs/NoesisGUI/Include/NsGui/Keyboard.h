////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_KEYBOARD_H__
#define __GUI_KEYBOARD_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IView.h>
#include <NsCore/Ptr.h>
#include <NsCore/Vector.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/BaseComponent.h>


namespace Noesis
{

class View;
class UIElement;
class RoutedEvent;
class KeyboardNavigation;
typedef NsVector< Ptr<UIElement> > ParentElementList;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the keyboard device.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Keyboard: public BaseComponent
{
public:
    Keyboard(View* view);
    ~Keyboard();

    /// Resets keyboard state
    void ResetState();

    /// Gets the set of ModifierKeys that are currently pressed
    uint32_t GetModifiers() const;

    /// Gets the set of key states for the specified key
    uint32_t GetKeyStates(Key key) const;

    /// Determines whether the specified key is pressed
    bool IsKeyDown(Key key) const;

    /// Determines whether the specified key is released
    bool IsKeyUp(Key key) const;

    /// Determines whether the specified key is toggled
    bool IsKeyToggled(Key key) const;

    /// Gets the element that has keyboard focus
    UIElement* GetFocused() const;

    /// Sets keyboard focus on the specified element
    /// \return The element that has focus after calling the function. May be an element other than
    /// that specified, even a null element
    UIElement* Focus(UIElement* element);

public:
    /// Attached routed events
    //@{
    static const RoutedEvent* GotKeyboardFocusEvent;
    static const RoutedEvent* KeyDownEvent;
    static const RoutedEvent* KeyUpEvent;
    static const RoutedEvent* LostKeyboardFocusEvent;
    static const RoutedEvent* PreviewGotKeyboardFocusEvent;
    static const RoutedEvent* PreviewKeyDownEvent;
    static const RoutedEvent* PreviewKeyUpEvent;
    static const RoutedEvent* PreviewLostKeyboardFocusEvent;
    //@}

private:
    friend class View;

    void Char(uint32_t ch);
    void KeyDown(Key key);
    void KeyUp(Key key);

    void UpdateKey(Key key, bool isKeyDown);

    friend class FrameworkElement;
    KeyboardNavigation* GetKeyboardNavigation() const;

private:
    // Last keyboard key states
    uint32_t mKeyStates[Key_Count];

    Ptr<UIElement> mFocusedElement;

    typedef NsVector<Ptr<UIElement> > Elements;
    Elements mFocusWithinElements;

    Ptr<KeyboardNavigation> mKeyboardNavigation;

    bool mKeyDownHandled;

    NS_DECLARE_REFLECTION(Keyboard, BaseComponent)
};

NS_WARNING_POP

}

#endif
