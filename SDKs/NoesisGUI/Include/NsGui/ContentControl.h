////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CONTENTCONTROL_H__
#define __GUI_CONTENTCONTROL_H__


#include <NsCore/Noesis.h>
#include <NsGui/Control.h>


namespace Noesis
{

/// Forward declaration
//@{
class DataTemplate;
class DataTemplateSelector;
class ItemsControl;
//@}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control with a single piece of content.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.contentcontrol.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ContentControl: public Control
{
public:
    /// Constructor
    ContentControl();

    /// Destructor
    ~ContentControl();

    /// Gets or sets the content of a ContentControl
    //@{
    BaseComponent* GetContent() const;
    void SetContent(BaseComponent* content);
    void SetContent(const char* content);
    //@}

    /// Gets or sets the template used to display the content of the ContentControl
    //@{
    DataTemplate* GetContentTemplate() const;
    void SetContentTemplate(DataTemplate* dataTemplate);
    //@}

    /// Gets or sets a data template selector that provides custom logic for choosing the template
    /// used to display the content
    //@{
    DataTemplateSelector* GetContentTemplateSelector() const;
    void SetContentTemplateSelector(DataTemplateSelector* selector);
    //@}

    /// Gets a value that indicates whether the ContentControl contains content
    bool GetHasContent() const;

    /// From BaseObject
    //@{
    NsString ToString() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ContentProperty;
    static const DependencyProperty* ContentTemplateProperty;
    static const DependencyProperty* ContentTemplateSelectorProperty;
    static const DependencyProperty* HasContentProperty;
    //@}

protected:
    /// Content property value changed event
    //@{
    virtual void OnContentChanged(BaseComponent* oldContent, BaseComponent* newContent);
    //@}

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

    /// From FrameworkElement
    //@{
    uint32_t GetLogicalChildrenCount() const;
    BaseComponent* GetLogicalChild(uint32_t index) const;
    //@}

protected:
    bool mContentAsLogicalChild;


    friend class ItemsControl;
    void PrepareContainer(BaseComponent* item, DataTemplate* itemTemplate,
        DataTemplateSelector* itemTemplateSelector);
    void ClearContainer(BaseComponent* item);

    NS_DECLARE_REFLECTION(ContentControl, Control)
};

}

#endif
