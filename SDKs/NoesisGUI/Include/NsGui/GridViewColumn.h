////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRIDVIEWCOLUMN_H__
#define __GUI_GRIDVIEWCOLUMN_H__


#include <NsCore/Noesis.h>
#include <NsGui/ControlsApi.h>
#include <NsGui/DependencyObject.h>
#include <NsGui/IUITreeNode.h>


namespace Noesis
{

class Style;
class DataTemplate;
class DataTemplateSelector;
class BaseBindingExpression;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a column that displays data.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.gridviewcolumn.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CONTROLS_API GridViewColumn: public DependencyObject, public IUITreeNode
{
public:
    GridViewColumn();
    ~GridViewColumn();

    /// Gets the actual width of a GridViewColumn.
    float GetActualWidth() const;
    void SetActualWidth(float width);

    /// Event that is fired when ActualWidth changes
    typedef Delegate<void (GridViewColumn* sender)> ActualWidthChangedDelegate;
    ActualWidthChangedDelegate& ActualWidthChanged();

    /// Gets or sets the template to use to display the contents of a column cell
    //@{
    DataTemplate* GetCellTemplate() const;
    void SetCellTemplate(DataTemplate* dataTemplate);
    //@}

    /// Gets or sets a DataTemplateSelector that determines the template to use to display cells in
    /// a column
    //@{
    DataTemplateSelector* GetCellTemplateSelector() const;
    void SetCellTemplateSelector(DataTemplateSelector* selector);
    //@}

    /// Gets or sets the data item to bind to for this column
    //@{
    BaseBindingExpression* GetDisplayMemberBinding() const;
    void SetDisplayMemberBinding(BaseBindingExpression* binding);
    //@}
 
    /// Gets or sets the content of the header of a GridViewColumn
    //@{
    BaseComponent* GetHeader() const;
    void SetHeader(BaseComponent* header);
    //@} 
 
    /// Gets or sets the style to use for the header of the GridViewColumn
    //@{
    Style* GetHeaderContainerStyle() const;
    void SetHeaderContainerStyle(Style* style);
    //@}
 
    /// Gets or sets a composite string that specifies how to format the Header property if it is 
    /// displayed as a string
    //@{
    const char* GetHeaderStringFormat() const;
    void SetHeaderStringFormat(const char* format);
    //@}
 
    /// Gets or sets the template to use to display the content of the column header
    //@{
    DataTemplate* GetHeaderTemplate() const;
    void SetHeaderTemplate(DataTemplate* headerTemplate);
    //@}

    /// Gets or sets the DataTemplateSelector that provides logic to select the template to use to
    /// display the column header
    //@{
    DataTemplateSelector* GetHeaderTemplateSelector() const;
    void SetHeaderTemplateSelector(DataTemplateSelector* selector);
    //@}
 
    /// Gets or sets the width of the column. This is a dependency property.  
    //@{
    float GetWidth() const;
    void SetWidth(float width);
    //@}

    float GetDesiredWidth() const;
    float EnsureWidth(float width);

    enum MeasureState
    {
        /// Measure has not been done yet on this column
        MeasureState_None,
        /// Header measure has been done
        MeasureState_Header,
        /// Items measure has been done
        MeasureState_Items,
        /// Header and items measure has been done, or fixed width was set
        MeasureState_Fixed
    };
    
    MeasureState GetMeasureState() const;
    void SetMeasureState(MeasureState state); // To be used only from GridViewRowPresenterBase

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const override;
    void SetNodeParent(IUITreeNode* parent) override;
    BaseComponent* FindNodeResource(IResourceKey* key,
        bool fullElementSearch) const override;
    BaseComponent* FindNodeName(const char* name) const override;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* CellTemplateProperty;
    static const DependencyProperty* CellTemplateSelectorProperty;
    static const DependencyProperty* HeaderProperty;
    static const DependencyProperty* HeaderContainerStyleProperty;
    static const DependencyProperty* HeaderStringFormatProperty;
    static const DependencyProperty* HeaderTemplateProperty;
    static const DependencyProperty* HeaderTemplateSelectorProperty;
    static const DependencyProperty* WidthProperty;
    //@}

protected:
    /// From DependencyProperty
    //@{
    void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue) override;
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

private:
    IUITreeNode* mOwner;
    float mActualWidth;
    ActualWidthChangedDelegate mActualWidthChanged;
    
    // TODO: This should be BaseBinding instead on BaseBindingExpression
    Ptr<BaseBindingExpression> mDisplayMemberBinding;
    
    MeasureState mMeasureStateInternal;
    float mDesiredWidthInternal;
    //int mIndex;

    NS_DECLARE_REFLECTION(GridViewColumn, DependencyObject)
};

NS_WARNING_POP

}

#endif
