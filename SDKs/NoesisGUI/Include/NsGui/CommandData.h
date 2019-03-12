////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMMANDDATA_H__
#define __GUI_COMMANDDATA_H__


#include <NsCore/Noesis.h>
#include <NsCore/HashMap.h>
#include <NsCore/TypeMetaData.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ICommand.h>
#include <NsGui/CommandManager.h>


namespace Noesis
{

class CommandBinding;
class InputBinding;
class InputGesture;
template<class T> class TypedCollection;
typedef Noesis::TypedCollection<Noesis::CommandBinding> CommandBindingCollection;
typedef Noesis::TypedCollection<Noesis::InputBinding> InputBindingCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// CommandData. Used to register UI commands.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API CommandData: public TypeMetaData
{
public:
    /// Constructor
    CommandData(const TypeClass* ownerType);

    /// Destructor
    ~CommandData();

    /// Registers a routed event for a type
    template<class T>
    const T* RegisterCommand(const Ptr<T>& command);

    /// Finds a command for the owner type given its name
    const ICommand* FindCommand(NsSymbol name) const;

    /// Registers handlers for the specified command
    //@{
    void RegisterCommandHandler(const ICommand* command,
        const ExecutedRoutedEventHandler& executedHandler);
    void RegisterCommandHandler(const ICommand* command,
        const ExecutedRoutedEventHandler& executedHandler, const Ptr<InputGesture>& inputGesture);
    void RegisterCommandHandler(const ICommand* command,
        const ExecutedRoutedEventHandler& executedHandler, const Ptr<InputGesture>& inputGesture1,
        const Ptr<InputGesture>& inputGesture2);
    void RegisterCommandHandler(const ICommand* command,
        const CanExecuteRoutedEventHandler& canExecuteHandler,
        const ExecutedRoutedEventHandler& executedHandler);
    void RegisterCommandHandler(const ICommand* command,
        const CanExecuteRoutedEventHandler& canExecuteHandler,
        const ExecutedRoutedEventHandler& executedHandler, const Ptr<InputGesture>& inputGesture);
    void RegisterCommandHandler(const ICommand* command,
        const CanExecuteRoutedEventHandler& canExecuteHandler,
        const ExecutedRoutedEventHandler& executedHandler, const Ptr<InputGesture>& inputGesture1,
        const Ptr<InputGesture>& inputGesture2);
    //@}

    /// Gets command bindings associated with the owner type
    /// \remarks Returned collection can be null
    const CommandBindingCollection* GetCommandBindings() const;

    /// Gets input bindings associated with the owner type
    /// \remarks Returned collection can be null
    const InputBindingCollection* GetInputBindings() const;

private:
    static void DefaultCanExecute(BaseComponent* sender,
        const CanExecuteRoutedEventArgs& args);

    void InsertCommand(const Ptr<const ICommand>& command);

    void RegisterCommandBinding(const Ptr<CommandBinding>& binding);
    void RegisterInputBinding(const Ptr<InputBinding>& binding);

    void EnsureCommandBindings();
    void EnsureInputBindings();

private:
    const TypeClass* mOwnerType;

    struct Adapter;

    typedef NsHashMap<NsSymbol, Ptr<const ICommand> > CommandMap;
    CommandMap mCommands;

    Ptr<CommandBindingCollection> mCommandBindings;
    Ptr<InputBindingCollection> mInputBindings;

    NS_DECLARE_REFLECTION(CommandData, TypeMetaData)
};

NS_WARNING_POP

}

#include <NsGui/CommandData.inl>

#endif
