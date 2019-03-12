////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_REFLECTIONIMPLEMENTEMPTY_H__
#define __CORE_REFLECTIONIMPLEMENTEMPTY_H__


#include <NsCore/Noesis.h>
#include <NsCore/CompilerTools.h>
#include <NsCore/TypeCreate.h>
#include <NsCore/TypeInfo.h>
#include <NsCore/TypeClassCreatorEmpty.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
/// This type should be used as a null type when a class has no parent.
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Noesis
{
typedef NullType NoParent;
template<class T> struct T2T;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements reflection for a class outside class definition
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_REFLECTION_(classType) \
const Noesis::TypeClass* classType::StaticGetClassType(Noesis::T2T<classType>*)\
{\
    static const Noesis::TypeClass* type;\
\
    if (type == 0)\
    {\
        type = (const Noesis::TypeClass*)(Noesis::TypeCreate::Create(\
            NS_TYPEID(classType),\
            Noesis::TypeClassCreatorEmpty<SelfClass, ParentClass>::Create,\
            Noesis::TypeClassCreatorEmpty<SelfClass, ParentClass>::Fill));\
    }\
\
    return type;\
}\
\
const Noesis::TypeClass* classType::GetClassType() const\
{\
    return StaticGetClassType();\
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements static reflection functions for a class inside class definition
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_INLINE_STATIC_REFLECTION_(classType, parentType) \
public:\
    static const Noesis::TypeClass* StaticGetClassType(Noesis::T2T<classType>* = 0)\
    {\
        static const Noesis::TypeClass* type;\
\
        if (type == 0)\
        {\
            type = (const Noesis::TypeClass*)(Noesis::TypeCreate::Create(\
                NS_TYPEID(classType),\
                Noesis::TypeClassCreatorEmpty<SelfClass, ParentClass>::Create,\
                Noesis::TypeClassCreatorEmpty<SelfClass, ParentClass>::Fill));\
        }\
\
        return type;\
    }\
\
private:\
    typedef classType SelfClass;\
    typedef parentType ParentClass;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements reflection for a class inside class definition (templates must use this one)
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_INLINE_REFLECTION_(classType, parentType) \
public:\
    NS_WARNING_PUSH \
    NS_CLANG_WARNING_DISABLE("-Winconsistent-missing-override") \
        const Noesis::TypeClass* GetClassType() const\
        {\
            return StaticGetClassType();\
        } \
    NS_WARNING_POP \
    NS_IMPLEMENT_INLINE_STATIC_REFLECTION_(classType, parentType)

#endif
