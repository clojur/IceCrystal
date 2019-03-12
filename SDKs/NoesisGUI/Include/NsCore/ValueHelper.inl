////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/CompilerTools.h>
#include <NsCore/TypeOf.h>


namespace Noesis
{
namespace Impl
{

NS_CORE_KERNEL_API NsString ToString(bool value);
NS_CORE_KERNEL_API NsString ToString(char value);
NS_CORE_KERNEL_API NsString ToString(uint8_t value);
NS_CORE_KERNEL_API NsString ToString(uint16_t value);
NS_CORE_KERNEL_API NsString ToString(uint32_t value);
NS_CORE_KERNEL_API NsString ToString(uint64_t value);
NS_CORE_KERNEL_API NsString ToString(int8_t value);
NS_CORE_KERNEL_API NsString ToString(int16_t value);
NS_CORE_KERNEL_API NsString ToString(int32_t value);
NS_CORE_KERNEL_API NsString ToString(int64_t value);
NS_CORE_KERNEL_API NsString ToString(float value);
NS_CORE_KERNEL_API NsString ToString(double value);
NS_CORE_KERNEL_API NsString ToString(const NsString& value);
NS_CORE_KERNEL_API NsString ToString(Symbol value);

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> NsString ToString(const T& value)
{
    return value.ToString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> NsString ToString(const T& value, Int2Type<0>)
{ 
    return Impl::ToString(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> NsString ToString(const T& value, Int2Type<1>)
{ 
    return TypeOf<T>()->GetValue((int)value).GetStr();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> NsString ToString(const T& value, Int2Type<2>)
{
    return Impl::ToString(*value);
}

NS_CORE_KERNEL_API uint32_t GetHashCode(bool value);
NS_CORE_KERNEL_API uint32_t GetHashCode(char value);
NS_CORE_KERNEL_API uint32_t GetHashCode(uint8_t value);
NS_CORE_KERNEL_API uint32_t GetHashCode(uint16_t value);
NS_CORE_KERNEL_API uint32_t GetHashCode(uint32_t value);
NS_CORE_KERNEL_API uint32_t GetHashCode(uint64_t value);
NS_CORE_KERNEL_API uint32_t GetHashCode(int8_t value);
NS_CORE_KERNEL_API uint32_t GetHashCode(int16_t value);
NS_CORE_KERNEL_API uint32_t GetHashCode(int32_t value);
NS_CORE_KERNEL_API uint32_t GetHashCode(int64_t value);
NS_CORE_KERNEL_API uint32_t GetHashCode(float value);
NS_CORE_KERNEL_API uint32_t GetHashCode(double value);
NS_CORE_KERNEL_API uint32_t GetHashCode(const NsString& value);
NS_CORE_KERNEL_API uint32_t GetHashCode(Symbol value);

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> uint32_t GetHashCode(const T& value)
{
    return value.GetHashCode();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> uint32_t GetHashCode(const T& value, Int2Type<0>)
{
    return Impl::GetHashCode(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> uint32_t GetHashCode(const T& value, Int2Type<1>)
{
    return Impl::GetHashCode(uint32_t(value));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> uint32_t GetHashCode(const T& value, Int2Type<2>)
{
    return Impl::GetHashCode(reinterpret_cast<uintptr_t>(value));
}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> NsString ToString(const T& value)
{
    enum { ValueTypeId = IsPointer<T>::Result ? 2 : IsEnum<T>::Result ? 1 : 0 };
    return Impl::ToString(value, Int2Type<ValueTypeId>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> uint32_t GetHashCode(const T& value)
{
    enum { ValueTypeId = IsPointer<T>::Result ? 2 : IsEnum<T>::Result ? 1 : 0 };
    return Impl::GetHashCode(value, Int2Type<ValueTypeId>());
}

}
