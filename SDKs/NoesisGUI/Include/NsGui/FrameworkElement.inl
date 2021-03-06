////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* FrameworkElement::GetTemplateChild(const char* name) const
{
    return NsStaticCast<T*>(GetTemplateChild(name));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* FrameworkElement::FindName(const char* name) const
{
    return NsStaticCast<T*>(FindName(name));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* FrameworkElement::FindResource(IResourceKey* key) const
{
    return NsStaticCast<T*>(FindResource(key));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* FrameworkElement::TryFindResource(IResourceKey* key) const
{
    return NsStaticCast<T*>(TryFindResource(key));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* FrameworkElement::FindResource(const char* key) const
{
    return NsStaticCast<T*>(FindResource(key));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* FrameworkElement::TryFindResource(const char* key) const
{
    return NsStaticCast<T*>(TryFindResource(key));
}

}
