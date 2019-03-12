////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DOCKPANEL_H__
#define __GUI_DOCKPANEL_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Panel.h>


namespace Noesis
{

enum Dock;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an area where you can arrange child elements either horizontally or
/// vertically, relative to each other.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.dockpanel.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API DockPanel: public Panel
{
public:
    /// Constructor
    DockPanel();

    /// Gets a value that indicates the position of a child element within a DockPanel
    static Dock GetDock(DependencyObject* element);
    /// Sets a value that indicates the position of a child element within a DockPanel
    static void SetDock(DependencyObject* element, Dock dock);

    /// Gets or sets a value that indicates whether the last child element within a DockPanel
    /// stretches to fill the remaining available space
    //@{
    bool GetLastChildFill() const;
    void SetLastChildFill(bool fill);
    //@}

private:
    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize);
    Size ArrangeOverride(const Size& finalSize);
    //@}

    // Detects docking changes in children elements
    static void StaticDockChanged(DependencyObject* obj,
        const DependencyPropertyChangedEventArgs& args);

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* DockProperty; // attached property
    static const DependencyProperty* LastChildFillProperty;
    //@}

private:
    NS_DECLARE_REFLECTION(DockPanel, Panel)
};

}

#endif
