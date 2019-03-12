////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMPONENTCOMMANDS_H__
#define __GUI_COMPONENTCOMMANDS_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class RoutedUICommand;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a standard set of component-related commands.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API ComponentCommands
{
    /// Commands
    //@{
    static const RoutedUICommand* ExtendSelectionDownCommand;
    static const RoutedUICommand* ExtendSelectionLeftCommand;
    static const RoutedUICommand* ExtendSelectionRightCommand;
    static const RoutedUICommand* ExtendSelectionUpCommand;
    static const RoutedUICommand* MoveDownCommand;
    static const RoutedUICommand* MoveFocusBackCommand;
    static const RoutedUICommand* MoveFocusDownCommand;
    static const RoutedUICommand* MoveFocusForwardCommand;
    static const RoutedUICommand* MoveFocusPageDownCommand;
    static const RoutedUICommand* MoveFocusPageUpCommand;
    static const RoutedUICommand* MoveFocusUpCommand;
    static const RoutedUICommand* MoveLeftCommand;
    static const RoutedUICommand* MoveRightCommand;
    static const RoutedUICommand* MoveToEndCommand;
    static const RoutedUICommand* MoveToHomeCommand;
    static const RoutedUICommand* MoveToPageDownCommand;
    static const RoutedUICommand* MoveToPageUpCommand;
    static const RoutedUICommand* MoveUpCommand;
    static const RoutedUICommand* ScrollByLineCommand;
    static const RoutedUICommand* ScrollPageDownCommand;
    static const RoutedUICommand* ScrollPageLeftCommand;
    static const RoutedUICommand* ScrollPageRightCommand;
    static const RoutedUICommand* ScrollPageUpCommand;
    static const RoutedUICommand* SelectToEndCommand;
    static const RoutedUICommand* SelectToHomeCommand;
    static const RoutedUICommand* SelectToPageDownCommand;
    static const RoutedUICommand* SelectToPageUp;
    //@}

    NS_DECLARE_REFLECTION(ComponentCommands, NoParent)
};

}

#endif
