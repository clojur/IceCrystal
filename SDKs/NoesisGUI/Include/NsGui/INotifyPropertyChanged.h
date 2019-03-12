////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INOTIFYPROPERTYCHANGED_H__
#define __GUI_INOTIFYPROPERTYCHANGED_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Events.h>


namespace Noesis
{

class BaseComponent;
class Symbol;
template<class T> class Ptr;
template<class T> class Delegate;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Contains state information for the PropertyChanged event.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct PropertyChangedEventArgs: public EventArgs
{
    NsSymbol propertyName;

    PropertyChangedEventArgs(NsSymbol name): propertyName(name) { }

    NS_IMPLEMENT_INLINE_REFLECTION_(PropertyChangedEventArgs, EventArgs)
};

typedef Delegate<void (BaseComponent*, const PropertyChangedEventArgs&)>
    PropertyChangedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Notifies clients that a property value has changed.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE INotifyPropertyChanged: public Interface
{
    /// Occurs when a property value changes
    //@{
    virtual PropertyChangedEventHandler& PropertyChanged() = 0;
    //@}

    NS_IMPLEMENT_INLINE_REFLECTION_(INotifyPropertyChanged, Interface)
};

}

#endif
