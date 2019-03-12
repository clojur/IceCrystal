////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsGui/ValueStorageManagerImpl.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create()
{
    return *new FrameworkPropertyMetadata();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(int32_t options)
{
    return *new FrameworkPropertyMetadata(options);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(
    PropertyChangedCallback changed)
{
    return *new FrameworkPropertyMetadata(changed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(
    CoerceValueCallback coerce)
{
    return *new FrameworkPropertyMetadata(coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    int32_t options)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new FrameworkPropertyMetadata(defaultValueManager, &defaultValue, options);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    PropertyChangedCallback changed)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new FrameworkPropertyMetadata(defaultValueManager, &defaultValue, changed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    CoerceValueCallback coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new FrameworkPropertyMetadata(defaultValueManager, &defaultValue, coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    int32_t options, PropertyChangedCallback changed)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new FrameworkPropertyMetadata(defaultValueManager, &defaultValue, options,
        changed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    int32_t options, CoerceValueCallback coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new FrameworkPropertyMetadata(defaultValueManager, &defaultValue, options,
        coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    int32_t options, PropertyChangedCallback changed, CoerceValueCallback coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new FrameworkPropertyMetadata(defaultValueManager, &defaultValue, options,
        changed, coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::AffectsMeasure() const
{
    return (mFrameworkOptions & FrameworkOptions_AffectsMeasure) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::AffectsArrange() const
{
    return (mFrameworkOptions & FrameworkOptions_AffectsArrange) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::AffectsParentMeasure() const
{
    return (mFrameworkOptions & FrameworkOptions_AffectsParentMeasure) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::AffectsParentArrange() const
{
    return (mFrameworkOptions & FrameworkOptions_AffectsParentArrange) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::AffectsRender() const
{
    return (mFrameworkOptions & FrameworkOptions_AffectsRender) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::Inherits() const
{
    return (mFrameworkOptions & FrameworkOptions_Inherits) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::OverridesInheritanceBehavior() const
{
    return (mFrameworkOptions & FrameworkOptions_OverridesInheritanceBehavior) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::IsNotDataBindable() const
{
    return (mFrameworkOptions & FrameworkOptions_NotDataBindable) != 0;
}

/// Indicates whether the property binds two-way by default
inline bool FrameworkPropertyMetadata::BindsTwoWayByDefault() const
{
    return (mFrameworkOptions & FrameworkOptions_BindsTwoWayByDefault) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//inline bool FrameworkPropertyMetadata::Journal() const
//{
//    return (mFrameworkOptions & FrameworkOptions_Jornal) != 0;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::SubPropertiesDoNotAffectRender() const
{
    return (mFrameworkOptions & FrameworkOptions_SubPropertiesDoNotAffectRender) != 0;
}

}
