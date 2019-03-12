////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TABPANEL_H__
#define __GUI_TABPANEL_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Panel.h>

namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Handles the layout of the TabItem objects on a TabControl.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.tabpanel.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TabPanel: public Panel
{
public:
    TabPanel();

protected:
    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize);
    Size ArrangeOverride(const Size& finalSize);
    //@}

    NS_DECLARE_REFLECTION(TabPanel, Panel)
};

}

#endif
