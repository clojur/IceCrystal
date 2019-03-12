////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_UIPROPERTYMETADATA_H__
#define __GUI_UIPROPERTYMETADATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/PropertyMetadata.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// UIPropertyMetadata. Provides property metadata for non-framework properties that do have
/// rendering/user interface impact at the core level.
///
/// WPF UIPropertyMetadata fields:
///  - IsAnimationProhibited
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API UIPropertyMetadata: public PropertyMetadata
{
public:
    /// Helper functions to create a new PropertyMetadata
    //@{
    inline static Ptr<UIPropertyMetadata> Create();
    template<class T>
    inline static Ptr<UIPropertyMetadata> Create(const T& defaultValue);
    inline static Ptr<UIPropertyMetadata> Create(PropertyChangedCallback changed);
    inline static Ptr<UIPropertyMetadata> Create(CoerceValueCallback coerce);
    template<class T>
    inline static Ptr<UIPropertyMetadata> Create(const T& defaultValue,
        PropertyChangedCallback changed);
    template<class T>
    inline static Ptr<UIPropertyMetadata> Create(const T& defaultValue,
        CoerceValueCallback coerce);
    template<class T>
    inline static Ptr<UIPropertyMetadata> Create(const T& defaultValue,
        PropertyChangedCallback changed, CoerceValueCallback coerce);
    //@}

protected:
    // Constructors
    //@{
    UIPropertyMetadata();
    UIPropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue);
    UIPropertyMetadata(PropertyChangedCallback changed);
    UIPropertyMetadata(CoerceValueCallback coerce);
    UIPropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        PropertyChangedCallback changed);
    UIPropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        CoerceValueCallback coerce);
    UIPropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        PropertyChangedCallback changed, CoerceValueCallback coerce);
    //@}

    NS_DECLARE_REFLECTION(UIPropertyMetadata, PropertyMetadata)
};

}

/// Inline Include
#include <NsGui/UIPropertyMetadata.inl>

#endif
