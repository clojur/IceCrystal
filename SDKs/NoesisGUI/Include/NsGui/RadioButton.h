////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RADIOBUTTON_H__
#define __GUI_RADIOBUTTON_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ToggleButton.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a button that can be selected, but not cleared, by a user.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.radiobutton.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API RadioButton: public ToggleButton
{
public:
    /// Constructor
    RadioButton();
    
    /// Destructor
    ~RadioButton();

    /// Gets or sets the name that specifies which RadioButton controls are mutually exclusive
    //@{
    const char* GetGroupName() const;
    void SetGroupName(const char* name);
    //@}

    /// Dependency properties
    //@{
    static const DependencyProperty* GroupNameProperty;
    //@}

protected:

    // From BaseButton
    //@{
    void OnToggle();
    //@}
    
    // From UIElement
    //@{
    void OnChecked(const RoutedEventArgs& e);
    //@}
    
    // From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}
    
private:
    void RegisterGroup(const NsString& name);
    void UnregisterGroup(const NsString& name);

    NS_DECLARE_REFLECTION(RadioButton, ToggleButton)
};

}

#endif
