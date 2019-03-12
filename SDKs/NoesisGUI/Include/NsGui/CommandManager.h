////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMMANDMANAGER_H__
#define __GUI_COMMANDMANAGER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/RoutedEvent.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

template<class T> class Delegate;
NS_INTERFACE ICommand;
class UIElement;
struct KeyEventArgs;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the CanExecute and PreviewCanExecute routed events.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API CanExecuteRoutedEventArgs: public RoutedEventArgs
{
    /// The command associated with this event
    const ICommand* command;

    /// The command specific data
    Ptr<BaseComponent> parameter;

    /// Indicates whether the RoutedCommand associated with this event can be executed on the target
    mutable bool canExecute;

    /// Determines whether the input routed event that invoked the command should continue to route
    /// through the element tree
    mutable bool continueRouting;

    /// Constructor
    CanExecuteRoutedEventArgs(BaseComponent* s, const RoutedEvent* e, const ICommand* c,
        BaseComponent* p);

    /// Destructor
    ~CanExecuteRoutedEventArgs();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Noesis::Delegate<void (BaseComponent*, const CanExecuteRoutedEventArgs&)>
    CanExecuteRoutedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the Executed and PreviewExecuted routed events.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API ExecutedRoutedEventArgs: public RoutedEventArgs
{
    /// The command associated with this event
    const ICommand* command;

    /// The command specific data
    Ptr<BaseComponent> parameter;

    /// Constructor
    ExecutedRoutedEventArgs(BaseComponent* s, const RoutedEvent* e, const ICommand* c,
        BaseComponent* p);

    /// Destructor
    ~ExecutedRoutedEventArgs();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Noesis::Delegate<void (BaseComponent*, const ExecutedRoutedEventArgs&)>
    ExecutedRoutedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a command that implements ICommand and is routed through the element tree.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API CommandManager
{
    /// Attached events
    //@{
    static const RoutedEvent* PreviewCanExecuteEvent;
    static const RoutedEvent* CanExecuteEvent;
    static const RoutedEvent* PreviewExecutedEvent;
    static const RoutedEvent* ExecutedEvent;
    //@}

    /// Process keyboard input (for internal use only)
    static void ProcessKeyboardInput(UIElement* target, const KeyEventArgs& args);

private:
    static void ExecuteCommand(const ICommand* command, BaseComponent* param,
        UIElement* target, const KeyEventArgs& args);

    static void StaticOnPreviewCanExecute(BaseComponent* obj, const EventArgs& e);
    static void StaticOnCanExecute(BaseComponent* obj, const EventArgs& e);
    static void StaticOnPreviewExecuted(BaseComponent* obj, const EventArgs& e);
    static void StaticOnExecuted(BaseComponent* obj, const EventArgs& e);

    NS_DECLARE_REFLECTION(CommandManager, NoParent)
};

NS_WARNING_POP

}

#endif
