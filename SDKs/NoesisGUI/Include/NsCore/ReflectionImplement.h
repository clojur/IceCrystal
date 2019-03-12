////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_REFLECTIONIMPLEMENT_H__
#define __CORE_REFLECTIONIMPLEMENT_H__


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Shortcuts for defining reflection members
////////////////////////////////////////////////////////////////////////////////////////////////////

/// Adds metadatas
#define NsMeta helper.Meta

/// Indicates that type implements an interface
#define NsImpl Rebind_(helper).template Impl

/// Adds properties
#define NsProp helper.Prop

/// Adds events
#define NsEvent helper.Event


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements reflection for a class outside class definition
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_REFLECTION(classType) \
const Noesis::TypeClass* classType::StaticGetClassType(Noesis::T2T<classType>*)\
{\
    static const Noesis::TypeClass* type;\
\
    if (type == 0)\
    {\
        type = static_cast<const Noesis::TypeClass*>(Noesis::TypeCreate::Create(\
            NS_TYPEID(classType),\
            Noesis::TypeClassCreator::Create<SelfClass>,\
            Noesis::TypeClassCreator::Fill<SelfClass, ParentClass>));\
    }\
\
    return type;\
}\
\
const Noesis::TypeClass* classType::GetClassType() const\
{\
    return StaticGetClassType();\
}\
\
struct classType::Rebind_ \
{ \
    NS_DISABLE_COPY(Rebind_) \
    NS_FORCEINLINE Rebind_(Noesis::TypeClassCreator& helper_): helper(helper_) {} \
    template<class IFACE> NS_FORCEINLINE void Impl() { helper.Impl<classType, IFACE>(); } \
    Noesis::TypeClassCreator& helper; \
}; \
\
template <class VOID_> void classType::StaticFillClassType(Noesis::TypeClassCreator& helper)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements reflection for a templated class with one template param outside class definition
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_REFLECTION_T1(classType) \
template<class T1>\
const Noesis::TypeClass* classType<T1>::StaticGetClassType(Noesis::T2T<classType>*)\
{\
    static const Noesis::TypeClass* type;\
\
    if (type == 0)\
    {\
        type = static_cast<const Noesis::TypeClass*>(Noesis::TypeCreate::Create(\
            NS_TYPEID(classType<T1>),\
            Noesis::TypeClassCreator::Create<SelfClass>,\
            Noesis::TypeClassCreator::Fill<SelfClass, ParentClass>));\
    }\
\
    return type;\
}\
\
template<class T1>\
const Noesis::TypeClass* classType<T1>::GetClassType() const\
{\
    return StaticGetClassType();\
}\
\
template<class T1>\
struct classType<T1>::Rebind_ \
{ \
    NS_DISABLE_COPY(Rebind_) \
    NS_FORCEINLINE Rebind_(Noesis::TypeClassCreator& helper_): helper(helper_) {} \
    template<class IFACE> NS_FORCEINLINE void Impl() { helper.Impl<classType, IFACE>(); } \
    Noesis::TypeClassCreator& helper; \
}; \
\
template<class T1> template<class VOID_> \
void classType<T1>::StaticFillClassType(Noesis::TypeClassCreator& helper)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements reflection for a class inside class definition (templates must use this one)
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_INLINE_REFLECTION(classType, parentType) \
public:\
    static const Noesis::TypeClass* StaticGetClassType(Noesis::T2T<classType>* = 0)\
    {\
        static const Noesis::TypeClass* type;\
\
        if (type == 0)\
        {\
            type = static_cast<const Noesis::TypeClass*>(Noesis::TypeCreate::Create(\
                NS_TYPEID(classType),\
                Noesis::TypeClassCreator::Create<SelfClass>,\
                Noesis::TypeClassCreator::Fill<SelfClass, ParentClass>));\
        }\
\
        return type;\
    }\
\
    NS_WARNING_PUSH \
        NS_CLANG_WARNING_DISABLE("-Winconsistent-missing-override") \
        const Noesis::TypeClass* GetClassType() const\
        {\
            return StaticGetClassType();\
        }\
    NS_WARNING_POP \
\
private:\
    typedef classType SelfClass;\
    typedef parentType ParentClass;\
\
    friend class Noesis::TypeClassCreator;\
\
    struct Rebind_ \
    { \
        NS_DISABLE_COPY(Rebind_) \
        NS_FORCEINLINE Rebind_(Noesis::TypeClassCreator& helper_): helper(helper_) {} \
        template<class IFACE> NS_FORCEINLINE void Impl() { helper.Impl<classType, IFACE>(); } \
        Noesis::TypeClassCreator& helper; \
    }; \
\
    template <class VOID_> static void StaticFillClassType(Noesis::TypeClassCreator& helper)


#include <NsCore/Noesis.h> 
#include <NsCore/CompilerTools.h>
#include <NsCore/TypeCreate.h>
#include <NsCore/TypeClassCreator.h>
#include <NsCore/TypeOf.h>


#endif
