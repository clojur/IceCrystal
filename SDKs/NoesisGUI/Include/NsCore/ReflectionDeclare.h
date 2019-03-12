////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_REFLECTIONDECLARE_H__
#define __CORE_REFLECTIONDECLARE_H__


#include <NsCore/CompilerTools.h>


namespace Noesis
{

class TypeClass;
class TypeClassCreator;
typedef NullType NoParent;
template<class T> struct T2T;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_DECLARE_STATIC_REFLECTION(classType, parentType) \
    static const Noesis::TypeClass* StaticGetClassType(Noesis::T2T<classType>* = 0);\
\
private:\
    typedef classType SelfClass;\
    typedef parentType ParentClass;\
\
    friend class Noesis::TypeClassCreator;\
    struct Rebind_;\
\
    /* This is templatized on purpose to avoid this function being exported */ \
    template<class VOID_> static void StaticFillClassType(Noesis::TypeClassCreator& helper);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Declares reflection for a class (interface, component or simple class/struct)
/// Note: StaticFillClassType is templatized but always instanced with the type void. This way
/// we can use always the .template keyword in gcc when T is indistinctly dependent or not
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_DECLARE_REFLECTION(classType, parentType) \
public:\
    NS_WARNING_PUSH \
        NS_CLANG_WARNING_DISABLE("-Winconsistent-missing-override") \
        const Noesis::TypeClass* GetClassType() const;\
    NS_WARNING_POP \
    NS_DECLARE_STATIC_REFLECTION(classType, parentType)


#endif
