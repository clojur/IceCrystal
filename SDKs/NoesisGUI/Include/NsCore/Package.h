////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_PACKAGE_H__
#define __CORE_PACKAGE_H__


#include <NsCore/RegisterComponent.h>


NS_MSVC_WARNING_DISABLE(4100)


////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_REGISTER_REFLECTION(module, package) \
    extern "C" void NsRegisterReflection##module##package( \
    Noesis::ComponentFactory* factory, bool registerComponents) \

////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_REGISTER_COMPONENT(componentClass) \
    { \
        const Noesis::TypeClass* type = Noesis::TypeOf<componentClass>(); \
        if (registerComponents) \
        { \
            Noesis::RegisterComponent(type, NsComponentCreator<componentClass>); \
        } \
        else \
        { \
            Noesis::UnregisterComponent(type); \
        } \
    }

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef NS_TESTS_ENABLED
    #define NS_REGISTER_TEST(n) \
        extern void Noesis_Register##n##Test(bool doRegister); \
        Noesis_Register##n##Test(registerComponents);
#else
    #define NS_REGISTER_TEST(n)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_INIT_PACKAGE(module, package) \
    extern "C" void NsInitPackage##module##package()

////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_SHUTDOWN_PACKAGE(module, package) \
    extern "C" void NsShutdownPackage##module##package()

#endif
