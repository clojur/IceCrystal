////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/CompilerSettings.h>
#include <NsCore/ComponentFactory.h>


using namespace Noesis;


#ifdef NS_MULTIPLE_DLLS
    #define PACKAGE_EXPORT extern "C" NS_DLL_EXPORT
#else
    #define PACKAGE_EXPORT extern "C"
#endif

#define PACKAGE_REGISTER(MODULE, PACKAGE) \
    void NsRegisterReflection##MODULE##PACKAGE(ComponentFactory*, bool); \
    NsRegisterReflection##MODULE##PACKAGE(factory, doRegister)

#define PACKAGE_INIT(MODULE, PACKAGE) \
    void NsInitPackage##MODULE##PACKAGE(); \
    NsInitPackage##MODULE##PACKAGE()

#define PACKAGE_SHUTDOWN(MODULE, PACKAGE) \
    void NsShutdownPackage##MODULE##PACKAGE(); \
    NsShutdownPackage##MODULE##PACKAGE()


////////////////////////////////////////////////////////////////////////////////////////////////////
PACKAGE_EXPORT void NsRegisterReflection_NoesisApp(ComponentFactory* factory, bool doRegister)
{
    PACKAGE_REGISTER(App, DisplayLauncher);
    PACKAGE_REGISTER(Render, GLRenderContext);
    PACKAGE_REGISTER(App, ApplicationLauncher);
    PACKAGE_REGISTER(App, AndroidDisplay);
    PACKAGE_REGISTER(Render, GLRenderDevice);
    PACKAGE_REGISTER(Render, RenderContext);
    PACKAGE_REGISTER(App, Display);
    PACKAGE_REGISTER(App, Launcher);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
PACKAGE_EXPORT void NsInitPackages_NoesisApp()
{
    PACKAGE_INIT(App, DisplayLauncher);
    PACKAGE_INIT(Render, GLRenderContext);
    PACKAGE_INIT(App, ApplicationLauncher);
    PACKAGE_INIT(App, AndroidDisplay);
    PACKAGE_INIT(Render, GLRenderDevice);
    PACKAGE_INIT(Render, RenderContext);
    PACKAGE_INIT(App, Display);
    PACKAGE_INIT(App, Launcher);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
PACKAGE_EXPORT void NsShutdownPackages_NoesisApp()
{
    PACKAGE_SHUTDOWN(App, DisplayLauncher);
    PACKAGE_SHUTDOWN(Render, GLRenderContext);
    PACKAGE_SHUTDOWN(App, ApplicationLauncher);
    PACKAGE_SHUTDOWN(App, AndroidDisplay);
    PACKAGE_SHUTDOWN(Render, GLRenderDevice);
    PACKAGE_SHUTDOWN(Render, RenderContext);
    PACKAGE_SHUTDOWN(App, Display);
    PACKAGE_SHUTDOWN(App, Launcher);
}
