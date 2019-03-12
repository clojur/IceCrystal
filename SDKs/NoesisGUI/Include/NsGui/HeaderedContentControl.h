////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_HEADEREDCONTENTCONTROL_H__
#define __GUI_HEADEREDCONTENTCONTROL_H__


#include <NsCore/Noesis.h>
#include <NsGui/ContentControl.h>


namespace Noesis
{

class DataTemplate;
class DataTemplateSelector;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides the base implementation for all controls that contain single
/// content and have a header.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.headeredcontentcontrol.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API HeaderedContentControl: public ContentControl
{
public:
    /// Constructor
    HeaderedContentControl();

    /// Destructor
    ~HeaderedContentControl();

    /// Gets a value that indicates whether the header is a null reference
    bool GetHasHeader() const;

    /// Gets or sets the data used for the header of each control
    //@{
    BaseComponent* GetHeader() const;
    void SetHeader(BaseComponent* header);
    void SetHeader(const char* header);
    //@}

    /// Gets or sets the template used to display the content of the control's header
    //@{
    DataTemplate* GetHeaderTemplate() const;
    void SetHeaderTemplate(DataTemplate* dataTemplate);
    //@}

    /// Gets or sets a data template selector that provides custom logic for choosing the template
    /// used to display the header
    //@{
    DataTemplateSelector* GetHeaderTemplateSelector() const;
    void SetHeaderTemplateSelector(DataTemplateSelector* selector);
    //@}

protected:
    
    virtual void OnHeaderChanged(BaseComponent* oldHeader, BaseComponent* newHeader);
    virtual void OnHeaderTemplateChanged(DataTemplate* oldHeaderTemplate,
        DataTemplate* newHeaderTemplate);
    virtual void OnHeaderTemplateSelectorChanged(DataTemplateSelector* oldHeaderTemplateSelector,
        DataTemplateSelector* newHeaderTemplateSelector);
        
    /// From FrameworkElement
    //@{
    uint32_t GetLogicalChildrenCount() const;
    BaseComponent* GetLogicalChild(uint32_t index) const;
    //@}
        
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* HasHeaderProperty;
    static const DependencyProperty* HeaderProperty;
    static const DependencyProperty* HeaderTemplateProperty;
    static const DependencyProperty* HeaderTemplateSelectorProperty;
    //@}

private:
    friend class ItemsControl;
    void PrepareContainer(BaseComponent* item, DataTemplate* itemTemplate,
        DataTemplateSelector* itemTemplateSelector);
    void ClearContainer(BaseComponent* item);

private:
    bool mHeaderAsLogicalChild;

    NS_DECLARE_REFLECTION(HeaderedContentControl, ContentControl)
};

}

#endif
