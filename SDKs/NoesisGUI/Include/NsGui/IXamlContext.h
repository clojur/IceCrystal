////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IXAMLCONTEXT_H__
#define __GUI_IXAMLCONTEXT_H__


#include <NsCore/Noesis.h>
#include <NsCore/Symbol.h>
#include <NsCore/NSTLForwards.h>


namespace Noesis
{

class BaseComponent;
class XamlNode;
class AmbientStack;
NS_INTERFACE IResourceKey;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes the current context of a XAML tree parsing process
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IXamlContext
{
    virtual BaseComponent* GetCurrentInstance() const = 0;
    virtual BaseComponent* FindName(const char* name) const = 0;
    virtual const char* GetFileName() const = 0;
    virtual const AmbientStack& GetAmbient() const = 0;
    virtual const char* GetTypedPrefix(const char* prefix) const = 0;
    virtual NsString GetCurrentLineInfo() const = 0;
};

}

#endif
