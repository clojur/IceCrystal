////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_DISPLAYLAUNCHER_H__
#define __APP_DISPLAYLAUNCHER_H__


#include <NsCore/Noesis.h>
#include <NsApp/DisplayLauncherApi.h>
#include <NsApp/Launcher.h>
#include <NsCore/Ptr.h>


namespace NoesisApp
{

class Display;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Windowed application launcher
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_APP_DISPLAYLAUNCHER_API DisplayLauncher: public Launcher
{
public:
    NS_DISABLE_COPY(DisplayLauncher)

    DisplayLauncher();
    ~DisplayLauncher();

    /// Starts application
    int Run();

    /// Forces the application to quit at the end of current application loop iteration
    void Quit(int exitCode = 0);

    /// Gets application main window
    Display* GetDisplay() const;

private:
    virtual void OnStartUp();
    virtual void OnTick(double time);
    virtual void OnExit();

private:
    Noesis::Ptr<Display> mDisplay;
    int mExitCode;
};

NS_WARNING_POP

}

#endif
