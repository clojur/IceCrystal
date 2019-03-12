////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #1197]
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsGui/ValueStorageManagerImpl.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<PropertyMetadata> PropertyMetadata::Create()
{
    return *new PropertyMetadata();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<PropertyMetadata> PropertyMetadata::Create(const T& defaultValue)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new PropertyMetadata(defaultValueManager, &defaultValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<PropertyMetadata> PropertyMetadata::Create(const PropertyChangedCallback& changed)
{
    return *new PropertyMetadata(changed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<PropertyMetadata> PropertyMetadata::Create(CoerceValueCallback coerce)
{
    return *new PropertyMetadata(coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<PropertyMetadata> PropertyMetadata::Create(const T& defaultValue,
    const PropertyChangedCallback& changed)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new PropertyMetadata(defaultValueManager, &defaultValue, changed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<PropertyMetadata> PropertyMetadata::Create(const T& defaultValue,
    CoerceValueCallback coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new PropertyMetadata(defaultValueManager, &defaultValue, coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<PropertyMetadata> PropertyMetadata::Create(const T& defaultValue,
    const PropertyChangedCallback& changed, CoerceValueCallback coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new PropertyMetadata(defaultValueManager, &defaultValue, changed, coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool PropertyMetadata::HasDefaultValue() const
{
    return (mLocalFlags & PropertyFlags_Default) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const void* PropertyMetadata::GetDefaultValue() const
{
    NS_ASSERT(mDefaultValueManager != 0);
    return mDefaultValueManager ? mDefaultValueManager->ToValue(&mDefaultValue) : nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<BaseComponent> PropertyMetadata::GetDefaultValueObject() const
{
    NS_ASSERT(mDefaultValueManager != 0);
    return mDefaultValueManager ? mDefaultValueManager->Box(GetDefaultValue()) : nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ValueStorageManager* PropertyMetadata::GetValueManager() const
{
    return mDefaultValueManager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool PropertyMetadata::HasPropertyChangedCallback() const
{
    return (mLocalFlags & PropertyFlags_Changed) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const PropertyChangedCallback& PropertyMetadata::GetPropertyChangedCallback() const
{
    return mChanged;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool PropertyMetadata::HasCoerceValueCallback() const
{
    return (mLocalFlags & PropertyFlags_Coerce) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline CoerceValueCallback PropertyMetadata::GetCoerceValueCallback() const
{
    return mCoerce;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool PropertyMetadata::IsUncached() const
{
    return (mLocalFlags & PropertyFlags_Uncached) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool PropertyMetadata::Uncached() const
{
    return mUncached;
}

}
