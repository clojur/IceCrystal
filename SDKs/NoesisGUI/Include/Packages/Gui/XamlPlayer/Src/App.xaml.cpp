////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "App.xaml.h"
#include "MainWindow.xaml.h"

#include <NsCore/ReflectionImplement.h>


using namespace Noesis;
using namespace NoesisApp;
using namespace XamlPlayer;


////////////////////////////////////////////////////////////////////////////////////////////////////
App::App()
{
    StartUp() += MakeDelegate(this, &App::OnStartUp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void App::OnStartUp(BaseComponent*, const EventArgs&)
{
    // Load xaml passed in command line, if any
    const CommandLine& commandLine = GetArguments();

    if (commandLine.GetNumArgs() > 1)
    {
        const char* arg = commandLine.GetArg(1);
        if (String::EndsWith(arg, ".xaml", IgnoreCase_True))
        {
            MainWindow* window = (MainWindow*)GetMainWindow();
            window->LoadXAML(arg);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_IMPLEMENT_REFLECTION(XamlPlayer::App)
{
    NsMeta<TypeId>("XamlPlayer.App");
}
