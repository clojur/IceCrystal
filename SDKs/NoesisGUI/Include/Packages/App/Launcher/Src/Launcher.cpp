////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsApp/Launcher.h>
#include <NsCore/Kernel.h>
#include <NsCore/Error.h>
#include <NsCore/Log.h>
#include <NsCore/StringUtils.h>
#include <NsApp/CommandLine.h>

#ifdef NS_PLATFORM_WINDOWS_DESKTOP
    #include <windows.h>
#endif

#ifdef NS_PLATFORM_ANDROID
    #include <android/log.h>
    #define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "Noesis", __VA_ARGS__))
    #define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Noesis", __VA_ARGS__))
    #define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Noesis", __VA_ARGS__))
    #define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "Noesis", __VA_ARGS__))
#endif


#ifdef NS_APP_FRAMEWORK
    extern "C" void NsRegisterReflection_NoesisApp(Noesis::ComponentFactory*, bool);
    extern "C" void NsInitPackages_NoesisApp();
    extern "C" void NsShutdownPackages_NoesisApp();
#endif


using namespace Noesis;
using namespace NoesisApp;


namespace
{

Launcher* gInstance;
CommandLine gCommandLine;

}


////////////////////////////////////////////////////////////////////////////////////////////////////
Launcher::Launcher()
{
    NS_ASSERT(gInstance == 0);
    gInstance = this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Launcher::~Launcher()
{
#ifdef NS_APP_FRAMEWORK
    NsShutdownPackages_NoesisApp();
#endif
    NsGetKernel()->Shutdown();
    NS_ASSERT(gInstance != 0);
    gInstance = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Launcher* Launcher::Current()
{
    NS_ASSERT(gInstance != 0);
    return gInstance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Launcher::RegisterAppComponents()
{
#ifdef NS_APP_FRAMEWORK
    NsRegisterReflection_NoesisApp(nullptr, true);
    NsInitPackages_NoesisApp();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Launcher::SetArguments(int argc, char** argv)
{
    gCommandLine = CommandLine(argc, argv);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const CommandLine& Launcher::GetArguments() const
{
    return gCommandLine;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Launcher::Init() const
{
    SetLogHandler(Launcher::LoggingHandler);
    NsGetKernel()->Init(nullptr);

    RegisterAppComponents();
    RegisterComponents();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Launcher::LoggingHandler(const char* file, uint32_t line, uint32_t level, const char* channel,
    const char* message)
{
    NS_UNUSED(file, line);

    // By default only global channel is dumped
    bool filter = !String::IsNullOrEmpty(channel);

    // Enable "Binding" channel by command line
    if (gCommandLine.HasOption("log_binding"))
    {
        if (String::Equals(channel, "Binding"))
        {
            filter = false;
        }
    }

    if (!filter)
    {
        char out[512];
        const char* prefixes[] = { "T", "D", "I", "W", "E" };
        const char* prefix = level < NS_COUNTOF(prefixes) ? prefixes[level] : " ";
        String::FormatBuffer(out, sizeof(out), "[NOESIS/%s] %s\n", prefix, message);

#if defined(NS_PLATFORM_WINDOWS_DESKTOP)
        OutputDebugString(out);
#elif defined(NS_PLATFORM_ANDROID)
        switch (level)
        {
            case NS_LOG_LEVEL_TRACE:
            case NS_LOG_LEVEL_DEBUG: LOGD("%s", message); break;
            case NS_LOG_LEVEL_INFO: LOGI("%s", message); break;
            case NS_LOG_LEVEL_WARNING: LOGW("%s", message); break;
            case NS_LOG_LEVEL_ERROR: LOGE("%s", message); break;
        }
#else
        fprintf(stderr, "%s", out);
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Launcher::RegisterComponents() const
{
}
