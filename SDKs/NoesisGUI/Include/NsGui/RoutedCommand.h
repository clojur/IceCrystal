////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ROUTEDCOMMAND_H__
#define __GUI_ROUTEDCOMMAND_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ICommand.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Delegate.h>
#include <NsCore/Symbol.h>


namespace Noesis
{

class ConfigValueList;
class InputGesture;
class KeyGestureConverter;
class UIElement;

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class TypedCollection;
typedef Noesis::TypedCollection<Noesis::InputGesture> InputGestureCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A command that is routed through the element tree.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.input.routedcommand.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API RoutedCommand: public BaseComponent, public ICommand
{
public:
    /// Constructor
    RoutedCommand(NsSymbol name, const TypeClass* owner,
        InputGestureCollection* inputGestures);

    /// Destructor
    ~RoutedCommand();

    // Creates a routed command
    //@{
    static Ptr<RoutedCommand> Create(const char* name, const TypeClass* owner);
    static Ptr<RoutedCommand> Create(const char* name, const TypeClass* owner,
        InputGesture* inputGesture);
    static Ptr<RoutedCommand> Create(const char* name, const TypeClass* owner,
        InputGesture* inputGesture1, InputGesture* inputGesture2);
    //@}

    /// Gets the collection of InputGesture objects that are associated with this command
    InputGestureCollection* GetInputGestures() const;

    /// Determines whether this RoutedCommand can execute in its current state
    bool CanExecute(BaseComponent* param, UIElement* target) const;

    /// Executes the RoutedCommand on the current command target
    void Execute(BaseComponent* param, UIElement* target) const;

    /// From ICommand
    //@{
    NsSymbol GetName() const override;
    const TypeClass* GetOwnerType() const override;
    EventHandler& CanExecuteChanged() override;
    bool CanExecute(BaseComponent* param) const override;
    void Execute(BaseComponent* param) const override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    static Ptr<InputGestureCollection> CreateInputGestures(InputGesture* inputGesture1,
        InputGesture* inputGesture2);

private:
    friend struct CommandManager;
    bool CanExecute(BaseComponent* param, UIElement* target,
        bool& continueRouting) const;

private:
    NsSymbol mName;
    const TypeClass* mOwnerType;

    EventHandler mCanExecuteChanged;

    Ptr<InputGestureCollection> mInputGestures;

    NS_DECLARE_REFLECTION(RoutedCommand, BaseComponent)
};

NS_WARNING_POP

}

#endif
