////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ICOMMAND_H__
#define __GUI_ICOMMAND_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsGui/Events.h>


namespace Noesis
{

class BaseComponent;
class Symbol;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a command
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE ICommand: public Interface
{
    /// Gets the name of the command as it is registered in the CommandManager
    /// \prop
    virtual Symbol GetName() const = 0;

    /// Gets the class type of the object that defines the command
    /// \prop
    virtual const TypeClass* GetOwnerType() const = 0;

    /// Occurs when changes occur that affect whether or not the command should execute
    virtual EventHandler& CanExecuteChanged() = 0;

    /// Determines whether the command can execute in its current state
    virtual bool CanExecute(BaseComponent* param) const = 0;

    /// This method is called when command is invoked
    virtual void Execute(BaseComponent* param) const = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(ICommand, Interface)
};

}

#endif
