////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMMANDBINDING_H__
#define __GUI_COMMANDBINDING_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/CommandManager.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Delegate.h>


namespace Noesis
{

NS_INTERFACE ICommand;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Binds a RoutedCommand to the event handlers that implement the command.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.input.commandbinding.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API CommandBinding: public BaseComponent
{
public:
    /// Constructor
    CommandBinding();

    /// Constructors for code
    //@{
    CommandBinding(ICommand* command);
    CommandBinding(ICommand* command, const CanExecuteRoutedEventHandler& canExecuteHandler);
    CommandBinding(ICommand* command, const ExecutedRoutedEventHandler& executedHandler);
    CommandBinding(ICommand* command, const CanExecuteRoutedEventHandler& canExecuteHandler,
        const ExecutedRoutedEventHandler& executedHandler);
    //@}

    /// Destructor
    ~CommandBinding();

    /// Gets or sets the ICommand associated with this CommandBinding
    //@{
    ICommand* GetCommand() const;
    void SetCommand(ICommand* command);
    //@}

    /// Occurs when the command associated with this CommandBinding initiates a check to determine 
    /// whether the command can be executed on the current command target
    DelegateEvent_<CanExecuteRoutedEventHandler> PreviewCanExecute();

    /// Occurs when the command associated with this CommandBinding initiates a check to determine
    /// whether the command can be executed on the command target
    DelegateEvent_<CanExecuteRoutedEventHandler> CanExecute();

    /// Occurs when the command associated with this CommandBinding executes
    DelegateEvent_<ExecutedRoutedEventHandler> PreviewExecuted();

    /// Occurs when the command associated with this CommandBinding executes
    DelegateEvent_<ExecutedRoutedEventHandler> Executed();

private:
    friend struct CommandManager;

    /// Raises specified event
    //@{
    void RaisePreviewCanExecute(BaseComponent* sender,
        const CanExecuteRoutedEventArgs& args) const;
    void RaiseCanExecute(BaseComponent* sender, const CanExecuteRoutedEventArgs& args) const;
    void RaisePreviewExecuted(BaseComponent* sender,
        const ExecutedRoutedEventArgs& args) const;
    void RaiseExecuted(BaseComponent* sender, const ExecutedRoutedEventArgs& args) const;
    //@}

    bool CheckCanExecute(BaseComponent* sender, const ExecutedRoutedEventArgs& args) const;

private:
    Ptr<ICommand> mCommand;

    CanExecuteRoutedEventHandler mPreviewCanExecute;
    CanExecuteRoutedEventHandler mCanExecute;
    ExecutedRoutedEventHandler mPreviewExecuted;
    ExecutedRoutedEventHandler mExecuted;

    NS_DECLARE_REFLECTION(CommandBinding, BaseComponent)
};

NS_WARNING_POP

}

#endif
