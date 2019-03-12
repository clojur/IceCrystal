////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MSCORESYSTEMTYPES_H__
#define __GUI_MSCORESYSTEMTYPES_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IMarkupExtension.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Class representing bool values in xaml
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API SystemBoolean: public BaseComponent, public IMarkupExtension
{
public:
    /// From IMarkupExtension
    //@{
    Ptr<BaseComponent> ProvideValue(const void* context) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    Ptr<BaseComponent> mValue;
    
    NS_DECLARE_REFLECTION(SystemBoolean, BaseComponent)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Class representing int values in xaml
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API SystemInt16: public BaseComponent, public IMarkupExtension
{
public:
    /// From IMarkupExtension
    //@{
    Ptr<BaseComponent> ProvideValue(const void* context) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    Ptr<BaseComponent> mValue;

    NS_DECLARE_REFLECTION(SystemInt16, BaseComponent)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Class representing int values in xaml
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API SystemInt32: public BaseComponent, public IMarkupExtension
{
public:
    /// From IMarkupExtension
    //@{
    Ptr<BaseComponent> ProvideValue(const void* context) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    Ptr<BaseComponent> mValue;

    NS_DECLARE_REFLECTION(SystemInt32, BaseComponent)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Class representing int values in xaml
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API SystemUInt16: public BaseComponent, public IMarkupExtension
{
public:
    /// From IMarkupExtension
    //@{
    Ptr<BaseComponent> ProvideValue(const void* context) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    Ptr<BaseComponent> mValue;

    NS_DECLARE_REFLECTION(SystemUInt16, BaseComponent)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Class representing int values in xaml
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API SystemUInt32: public BaseComponent, public IMarkupExtension
{
public:
    /// From IMarkupExtension
    //@{
    Ptr<BaseComponent> ProvideValue(const void* context) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    Ptr<BaseComponent> mValue;

    NS_DECLARE_REFLECTION(SystemUInt32, BaseComponent)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Class representing float values in xaml
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API SystemSingle: public BaseComponent, public IMarkupExtension
{
public:
    /// From IMarkupExtension
    //@{
    Ptr<BaseComponent> ProvideValue(const void* context) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    Ptr<BaseComponent> mValue;

    NS_DECLARE_REFLECTION(SystemSingle, BaseComponent)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Class representing double values in xaml
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API SystemDouble: public BaseComponent, public IMarkupExtension
{
public:
    /// From IMarkupExtension
    //@{
    Ptr<BaseComponent> ProvideValue(const void* context) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    Ptr<BaseComponent> mValue;

    NS_DECLARE_REFLECTION(SystemDouble, BaseComponent)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Class representing a string object in xaml
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API SystemString: public BaseComponent, public IMarkupExtension
{
public:
    /// From IMarkupExtension
    //@{
    Ptr<BaseComponent> ProvideValue(const void* context) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    Ptr<BaseComponent> mValue;

    NS_DECLARE_REFLECTION(SystemString, BaseComponent)
};

NS_WARNING_POP

}

#endif
