////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/CompilerTools.h>
#include <NsCore/TypeClassBuilder.h>
#include <NsCore/Error.h>


namespace Noesis
{

class TypeInfo;
typedef NullType NoParent;
template<class T> struct T2T;

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class ClassT, class BaseT>
TypeClassCreatorEmpty<ClassT, BaseT>::TypeClassCreatorEmpty(TypeClassBuilder* typeClass):
    mTypeClass(typeClass)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class ClassT, class BaseT>
Type* TypeClassCreatorEmpty<ClassT, BaseT>::Create(const TypeInfo& typeInfo)
{
    return new TypeClass(typeInfo, IsInterface<ClassT>::Result);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class ClassT, class BaseT>
void TypeClassCreatorEmpty<ClassT, BaseT>::Fill(Type* type)
{
    TypeClassCreatorEmpty<ClassT, BaseT> helper((TypeClassBuilder*)(type));

    helper.Base<BaseT>(Int2Type<IsSame<BaseT, NoParent>::Result>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class ClassT, class BaseT>
template<class T>
void TypeClassCreatorEmpty<ClassT, BaseT>::Base(Int2Type<0>)
{
    enum { isDerived = IsDerived<ClassT, T>::Result };
    static_assert(isDerived, "class does not derive from specified base");

    mTypeClass->AddBase(T::StaticGetClassType((T2T<T>*)0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class ClassT, class BaseT>
template<class T>
void TypeClassCreatorEmpty<ClassT, BaseT>::Base(Int2Type<1>)
{
}

}
