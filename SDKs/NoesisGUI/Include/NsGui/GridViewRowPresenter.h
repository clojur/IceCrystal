////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRIDVIEWROWPRESENTER_H__
#define __GUI_GRIDVIEWROWPRESENTER_H__


#include <NsCore/Noesis.h>
#include <NsGui/ControlsApi.h>
#include <NsGui/BaseGridViewRowPresenter.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an object that specifies the layout of a row of data.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.gridviewrowpresenter.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CONTROLS_API GridViewRowPresenter: public BaseGridViewRowPresenter
{
public:
    GridViewRowPresenter();
    ~GridViewRowPresenter();
    
    /// Gets or sets the data content to display in a row
    //@{
    BaseComponent* GetContent() const;
    void SetContent(BaseComponent* content);
    //@}
    
public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ContentProperty;
    //@}
    
protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize);
    Size ArrangeOverride(const Size& finalSize);
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const;
    //@}

private:
    void UpdateCells();
    Ptr<FrameworkElement> CreateCell(GridViewColumn* column, bool& contentHasParent);
    void UpdateContent(BaseComponent* content);
    void UpdateCellContent(FrameworkElement* cell, GridViewColumn* column,
        BaseComponent* content, bool& contentHasParent);

    NS_DECLARE_REFLECTION(GridViewRowPresenter, BaseGridViewRowPresenter)
};

}

#endif
