////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_KEYGESTURE_H__
#define __GUI_KEYGESTURE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/InputGesture.h>
#include <NsGui/InputEnums.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/NSTLForwards.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a keyboard combination that can be used to invoke a command.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.input.keygesture.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API KeyGesture: public InputGesture
{
public:
    /// Constructor
    KeyGesture();

    /// Constructor for code
    KeyGesture(Key key, uint32_t modifiers = 0);

    /// Destructor
    ~KeyGesture();

    /// Gets the key associated with this KeyGesture
    Key GetKey() const;

    /// Gets the modifier keys associated with this KeyGesture
    uint32_t GetModifiers() const;

    /// Gets a string to be displayed in UI
    /// \prop
    NsString GetDisplayString() const;

    /// From InputGesture
    //@{
    bool Matches(BaseComponent* target, const RoutedEventArgs& args);
    //@}

private:
    bool CheckModifiers(BaseComponent* target) const;

private:
    Key mKey;
    uint32_t mModifiers;

    NS_DECLARE_REFLECTION(KeyGesture, InputGesture)
};

}

#endif
