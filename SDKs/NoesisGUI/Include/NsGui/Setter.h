////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SETTER_H__
#define __GUI_SETTER_H__


#include <NsCore/Noesis.h>
#include <NsGui/BaseSetter.h>
#include <NsCore/String.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class DependencyProperty;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a setter that applies a property value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.setter.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Setter: public BaseSetter
{
public:
    /// Constructor
    Setter();

    /// Gets or sets the name of the target element
    //@{
    const char* GetTargetName() const;
    void SetTargetName(const char* targetName);
    //@}

    /// Gets or sets property
    //@{
    const DependencyProperty* GetProperty() const;
    void SetProperty(const DependencyProperty* property);
    //@}

    /// Gets or sets value
    //@{
    BaseComponent* GetValue() const;
    void SetValue(BaseComponent* value);
    //@}

    /// From BaseSetter
    //@{
    void Seal() override;
    //@}

private:
    NsString mTargetName;
    const DependencyProperty* mProperty;
    Ptr<BaseComponent> mValue;

    NS_DECLARE_REFLECTION(Setter, BaseSetter)
};

NS_WARNING_POP

}

#endif
