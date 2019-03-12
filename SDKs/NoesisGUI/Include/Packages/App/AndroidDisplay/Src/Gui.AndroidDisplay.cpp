////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Package.h>

#include "AndroidDisplay.h"


using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
NS_REGISTER_REFLECTION(App, AndroidDisplay)
{
    NS_REGISTER_COMPONENT(AndroidDisplay)
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INIT_PACKAGE(App, AndroidDisplay)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_SHUTDOWN_PACKAGE(App, AndroidDisplay)
{
}
