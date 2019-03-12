////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsGui/ValueStorageManagerImpl.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<UIPropertyMetadata> UIPropertyMetadata::Create()
{
    return *new UIPropertyMetadata();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<UIPropertyMetadata> UIPropertyMetadata::Create(const T& defaultValue)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new UIPropertyMetadata(defaultValueManager, &defaultValue);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<UIPropertyMetadata> UIPropertyMetadata::Create(PropertyChangedCallback changed)
{
    return *new UIPropertyMetadata(changed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<UIPropertyMetadata> UIPropertyMetadata::Create(CoerceValueCallback coerce)
{
    return *new UIPropertyMetadata(coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<UIPropertyMetadata> UIPropertyMetadata::Create(const T& defaultValue,
    PropertyChangedCallback changed)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new UIPropertyMetadata(defaultValueManager, &defaultValue, changed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<UIPropertyMetadata> UIPropertyMetadata::Create(const T& defaultValue,
    CoerceValueCallback coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new UIPropertyMetadata(defaultValueManager, &defaultValue, coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<UIPropertyMetadata> UIPropertyMetadata::Create(const T& defaultValue,
    PropertyChangedCallback changed, CoerceValueCallback coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return *new UIPropertyMetadata(defaultValueManager, &defaultValue, changed, coerce);
}

}