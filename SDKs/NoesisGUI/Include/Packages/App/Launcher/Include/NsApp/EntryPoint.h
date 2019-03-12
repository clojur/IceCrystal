////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_ENTRYPOINT_H__
#define __APP_ENTRYPOINT_H__


#include <NsCore/Noesis.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
// Common main entry point for all Noesis applications
////////////////////////////////////////////////////////////////////////////////////////////////////
int NsMain(int argc, char** argv);


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Platform dependent application entry point
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef NS_PLATFORM_WINDOWS_DESKTOP

    #include <windows.h>
    #include <stdlib.h>

    ////////////////////////////////////////////////////////////////////////////////////////////////
    int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
    {
        return NsMain(__argc, __argv);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    int main(int argc, char** argv)
    {
        return NsMain(argc, argv);
    }

#elif defined(NS_PLATFORM_IPHONE)

    #import <UIKit/UIKit.h>

    static int __argc;
    static char** __argv;

    @interface AppDelegate: NSObject<UIApplicationDelegate>
    @end

    @implementation AppDelegate

    - (void)applicationDidFinishLaunching:(UIApplication *)application
    {
        [[UIApplication sharedApplication] setStatusBarHidden:YES];
        [self performSelector:@selector(performInit:) withObject:nil afterDelay:0.2f];
    }

    - (void)performInit:(id)object
    {
        exit(NsMain(__argc, __argv));
    }

    @end

    ////////////////////////////////////////////////////////////////////////////////////////////////
    int main(int argc, char** argv)
    {
        __argc = argc;
        __argv = argv;

        // Set working directory to main bundle
        CFBundleRef bundle = CFBundleGetMainBundle();
        CFURLRef url = CFBundleCopyBundleURL(bundle);
        CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);

        char uri[PATH_MAX];
        CFStringGetCString(path, uri, sizeof(uri), kCFStringEncodingUTF8);
        CFRelease(path);
        CFRelease(url);

        chdir(uri);

        return UIApplicationMain(argc, argv, nil, @"AppDelegate");
    }

#elif defined(NS_PLATFORM_OSX)

    ////////////////////////////////////////////////////////////////////////////////////////////////
    int main(int argc, char** argv)
    {
        return NsMain(argc, argv);
    }

#elif defined(NS_PLATFORM_ANDROID)

    #include <NsApp/Display.h>
    #include <NsCore/Log.h>

    #include <android_native_app_glue.h>
    #include <android/log.h>

    #define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "Noesis", __VA_ARGS__))

    ////////////////////////////////////////////////////////////////////////////////////////////////
    void OnAppCmd(android_app* app, int32_t cmd)
    {
        switch (cmd)
        {
            //case APP_CMD_CONTENT_RECT_CHANGED:
            case APP_CMD_INIT_WINDOW:
            {
                if (app->window != 0)
                {
                    bool* windowReady = (bool*)app->userData;
                    *windowReady = true;
                }
                break;
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    void android_main(android_app* app)
    {
        LOGD("android_main BEGIN");

        // Ensure app wrapper code is not discarded
        app_dummy();

        // Store current app environment
        NoesisApp::Display::SetPrivateData(app);

        // Register into app command queue to wait for window creation
        app->onAppCmd = OnAppCmd;

        // Wait for creation of activity main window
        bool windowReady = false;
        app->userData = &windowReady;
        while (!windowReady)
        {
            // Read all pending events
            int events;
            android_poll_source* source;
            while (ALooper_pollAll(0, 0, &events, (void**)&source) >= 0)
            {
                // Process this event
                if (source != 0)
                {
                    source->process(app, source);
                }

                // Check if exiting occurs before window was created
                if (app->destroyRequested)
                {
                    LOGD("App destroyed");
                    LOGD("android_main END");

                    return;
                }
                else if (windowReady)
                {
                    // Window was created, continue
                    break;
                }
            }
        }

        // Reset app data
        app->onAppCmd = 0;
        app->userData = 0;

        // We are ready to launch Noesis application
        const char* cargv[] = { "/system/bin/app_process", "" };
        int argc = sizeof(cargv)/sizeof(const char*);
        char** argv = const_cast<char**>(cargv);

        LOGD("Noesis Main");
        NsMain(argc, argv);
        LOGD("android_main END");

        exit(0);
    }

#elif defined(NS_PLATFORM_LINUX)

    ////////////////////////////////////////////////////////////////////////////////////////////////
    int main(int argc, char** argv)
    {
        return NsMain(argc, argv);
    }

#else
    #error Unknown platform
#endif


#endif
