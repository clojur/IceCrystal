////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_KEYBINDING_H__
#define __GUI_KEYBINDING_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/InputBinding.h>
#include <NsGui/InputEnums.h>


namespace Noesis
{

class KeyGesture;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Binds a KeyGesture to a Command.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.input.keybinding.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API KeyBinding: public InputBinding
{
public:
    KeyBinding();
    KeyBinding(ICommand* command, KeyGesture* gesture);
    KeyBinding(ICommand* command, Key key, uint32_t modifiers);
    ~KeyBinding();

    /// Gets or sets the Key of the KeyGesture associated with this KeyBinding
    //@{
    Key GetKey() const;
    void SetKey(Key key);
    //@}

    /// Gets the modifier keys (one or more Alt, Ctrl, Shift) of the associated KeyGesture
    //@{
    uint32_t GetModifiers() const;
    void SetModifiers(uint32_t modifiers);
    //@}

    /// From Freezable
    //@{
    Ptr<KeyBinding> Clone() const;
    Ptr<KeyBinding> CloneCurrentValue() const;
    //@}

public:
    static const DependencyProperty* KeyProperty;
    static const DependencyProperty* ModifiersProperty;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const;
    //@}

    /// From InputBinding
    //@{
    void OnGestureChanged(InputGesture* oldGesture, InputGesture* newGesture);
    //@}

private:
    void UpdateKeyAndModifers(KeyGesture* keyGesture);
    void UpdateGesture(Key key, uint32_t modifiers);

    static void OnKeyChanged(DependencyObject* d, const DependencyPropertyChangedEventArgs& e);
    static void OnModifiersChanged(DependencyObject* d,
        const DependencyPropertyChangedEventArgs& e);

    NS_DECLARE_REFLECTION(KeyBinding, InputBinding)
};

}

#endif
