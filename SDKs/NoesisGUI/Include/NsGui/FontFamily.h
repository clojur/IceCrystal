////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FONTFAMILY_H__
#define __GUI_FONTFAMILY_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Enums.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/String.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a family of related fonts.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API FontFamily: public BaseComponent
{
public:
    FontFamily();
    FontFamily(const char* source);
    FontFamily(const char* baseUri, const char* source);
    ~FontFamily();

    /// Gets the resource identifier that is used to resolve a font family name
    const char* GetBaseUri() const;

    /// Gets font family name that is used to construct the FontFamily object
    const char* GetSource() const;

    /// Gets FontFamily name
    // TODO: return a collection of family names
    const char* GetName() const;

private:
    friend class UISystem;
    void EnsureName();

private:
    NsString mBaseUri;
    NsString mSource;
    NsString mName;

    NS_DECLARE_REFLECTION(FontFamily, BaseComponent)
};

NS_WARNING_POP

}

#endif
