////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASEGRIDVIEWROWPRESENTER_H__
#define __GUI_BASEGRIDVIEWROWPRESENTER_H__


#include <NsCore/Noesis.h>
#include <NsGui/ControlsApi.h>
#include <NsGui/FrameworkElement.h>

#include <NsCore/Vector.h>


namespace Noesis
{

class GridViewColumn;
class UIElementCollection;
class ListView;
struct DependencyPropertyChangedEventArgs;
struct NotifyCollectionChangedEventArgs;
template<class T> class TypedCollection;

typedef Noesis::TypedCollection<Noesis::GridViewColumn> GridViewColumnCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

///////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the base class for classes that define the layout for a row of data where different
/// data items are displayed in different columns.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.gridviewrowpresenterbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CONTROLS_API BaseGridViewRowPresenter: public FrameworkElement
{
public:
    BaseGridViewRowPresenter();
    ~BaseGridViewRowPresenter();
    
    /// Gets or sets a GridViewColumnCollection
    //@{
    GridViewColumnCollection* GetColumns() const;
    void SetColumns(GridViewColumnCollection* columns);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ColumnsProperty;
    //@}
    
protected:
    /// From DependencyObject
    //@{
    void OnInit();
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

    // From Visual
    //@{
    uint32_t GetVisualChildrenCount() const;
    Visual* GetVisualChild(uint32_t index) const;
    //@}

protected:
    Ptr<UIElementCollection> mVisualChildren;
    
    // NOTE [srodriguez] This is used to force an InvalidateMeasure from GridViewHeaderRowPresenter
    // or GridViewRowPresenter when trying to determine the automatic width of the columns. This
    // probably could be optimized forcing the Invalidate only in the main scrollviewer
    ListView* mListView;

    bool mColumnsChanged;

private:
    void RegisterColumns(GridViewColumnCollection* columns);
    void UnregisterColumns(GridViewColumnCollection* columns);
    void OnColumnActualWidthChanged(GridViewColumn* column);
    void OnColumnsCollectionChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& e);
    void InvalidateColumns();

    NS_DECLARE_REFLECTION(BaseGridViewRowPresenter, FrameworkElement)
};

NS_WARNING_POP

}

#endif
