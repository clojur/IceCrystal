////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASEDEFINITION_H__
#define __GUI_BASEDEFINITION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
//#include <NsGui/FrameworkContentElement.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

// TODO: [sfernandez] I made BaseDefinition inherit from FrameworkElement until we add support for
//  cloning and templating on FrameworkContentElement

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines the functionality required to support a shared-size group that is used
/// by the ColumnDefinitionCollection and RowDefinitionCollection classes.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.definitionbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
//class BaseDefinition: public FrameworkContentElement
class NS_GUI_CORE_API BaseDefinition: public FrameworkElement
{
public:
    BaseDefinition();
    ~BaseDefinition();

    /// Gets or sets a value that identifies a ColumnDefinition or RowDefinition as a member of a 
    /// defined group that shares sizing properties.
    //@{
    const char* GetSharedSizeGroup() const;
    void SetSharedSizeGroup(const char* group);
    //@}
    

public:
    static const DependencyProperty* SharedSizeGroupProperty;

private:
    //NS_DECLARE_REFLECTION(BaseDefinition, FrameworkContentElement)
    NS_DECLARE_REFLECTION(BaseDefinition, FrameworkElement)
};

}

#endif
