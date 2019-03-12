////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Package.h>
#include <NsCore/EnumConverter.h>
#include <NsApp/Application.h>
#include <NsApp/Window.h>


using namespace Noesis;
using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
NS_REGISTER_REFLECTION(App, ApplicationLauncher)
{
    NS_REGISTER_COMPONENT(Application)
    NS_REGISTER_COMPONENT(Window)

    NS_REGISTER_COMPONENT(EnumConverter<ResizeMode>)
    NS_REGISTER_COMPONENT(EnumConverter<WindowState>)
    NS_REGISTER_COMPONENT(EnumConverter<WindowStyle>)
    NS_REGISTER_COMPONENT(EnumConverter<WindowStartupLocation>)
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INIT_PACKAGE(App, ApplicationLauncher)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_SHUTDOWN_PACKAGE(App, ApplicationLauncher)
{
}
