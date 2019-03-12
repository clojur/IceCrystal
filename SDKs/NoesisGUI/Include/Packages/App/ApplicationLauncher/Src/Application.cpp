////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsApp/Application.h>
#include <NsApp/Window.h>
#include <NsApp/Display.h>
#include <NsApp/DisplayLauncher.h>
#include <NsGui/DependencyProperty.h>
#include <NsGui/TypeConverterMetaData.h>
#include <NsGui/ResourceDictionary.h>
#include <NsGui/IntegrationAPI.h>
#include <NsCore/ReflectionImplement.h>
#include <NsCore/TypeId.h>
#include <NsRender/RenderContext.h>
#include <NsRender/RenderDevice.h>
#include <EASTL/algorithm.h>


using namespace Noesis;
using namespace NoesisApp;


namespace
{

Application* gInstance;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
Application::Application(): mOwner(0)
{
    // More than one instantation is possible, for example when dropping App.xaml into XamlPlayer
    if (gInstance == 0)
    {
        gInstance = this;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Application::~Application()
{
    mExit(this, EventArgs::Empty);

    if (mMainWindow != 0)
    {
        mMainWindow->Shutdown();
        mMainWindow.Reset();
    }

    mRenderContext.Reset();

    if (mResources != 0)
    {
        mResources->RemoveDictionaryChangedListeners();
        mResources.Reset();
    }

    if (gInstance == this)
    {
        gInstance = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Application* Application::Current()
{
    NS_ASSERT(gInstance != 0);
    return gInstance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const char* Application::GetStartupUri() const
{
    return mStartupUri.c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Application::SetStartupUri(const char* startupUri)
{
    mStartupUri = startupUri;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
ResourceDictionary* Application::GetResources() const
{
    EnsureResources();
    return mResources;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Application::SetResources(ResourceDictionary* resources)
{
    if (mResources != resources)
    {
        DisconnectNode(mResources, this);
        mResources.Reset(resources);
        ConnectNode(mResources, this);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Window* Application::GetMainWindow() const
{
    return mMainWindow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const CommandLine& Application::GetArguments() const
{
    return mArguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Application::Init(Display* display, const CommandLine& arguments)
{
    mArguments = arguments;

    // Application resources
    GUI::SetApplicationResources(GetResources());

    // load startup URI as main window
    if (!mStartupUri.empty())
    {
        Ptr<BaseComponent> root = GUI::LoadXaml(mStartupUri.c_str());
        mMainWindow = NsDynamicCast<Ptr<Window>>(root);

        // Non window roots are allowed
        if (mMainWindow == 0)
        {
            mMainWindow = *new Window();
            mMainWindow->DependencyObject::Init();
            mMainWindow->SetContent(root);
            mMainWindow->SetTitle(GetTitleOverride(NsDynamicCast<Ptr<UIElement>>(root)));
        }
    }
    else
    {
        NS_FATAL("Startup window not defined");
    }

    display->Activated() += MakeDelegate(this, &Application::OnActivated);
    display->Deactivated() += MakeDelegate(this, &Application::OnDeactivated);

    mRenderContext = GetRenderContextOverride();
    bool ppaa = GetPPAAOverride();
    uint32_t samples = GetSamplesOverride();
    bool vSync = GetVSyncOverride();
    bool sRGB = GetsRGBOverride();

    mRenderContext->Init(display->GetNativeHandle(), samples, vSync, sRGB);
    mRenderContext->GetDevice()->SetOffscreenSampleCount(samples);

    mMainWindow->Init(display, mRenderContext, eastl::max_alt(samples, 1U), ppaa);
    mStartUp(this, EventArgs::Empty);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Application::Tick(double time)
{
    mMainWindow->Render(time);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Application::Shutdown(int exitCode)
{
    ((DisplayLauncher*)Launcher::Current())->Quit(exitCode);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
EventHandler& Application::Activated()
{
    return mActivated;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
EventHandler& Application::Deactivated()
{
    return mDeactivated;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
EventHandler& Application::StartUp()
{
    return mStartUp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
EventHandler& Application::Exit()
{
    return mExit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
IUITreeNode* Application::GetNodeParent() const
{
    return mOwner;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Application::SetNodeParent(IUITreeNode* parent)
{
    mOwner = parent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BaseComponent* Application::FindNodeResource(IResourceKey* key, bool fullElementSearch) const
{
    BaseComponent* resource = 0;
    if (mResources && mResources->Find(key, resource))
    {
        return resource;
    }

    if (mOwner)
    {
        return mOwner->FindNodeResource(key, fullElementSearch);
    }

    return DependencyProperty::GetUnsetValue();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BaseComponent* Application::FindNodeName(const char* name) const
{
    if (mOwner)
    {
        return mOwner->FindNodeName(name);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
ObjectWithNameScope Application::FindNodeNameAndScope(const char* name) const
{
    if (mOwner)
    {
        return mOwner->FindNodeNameAndScope(name);
    }
    
    return ObjectWithNameScope();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const char* Application::GetTitleOverride(UIElement* root) const
{
    Window* window = NsDynamicCast<Window*>(root);
    return window ? window->GetTitle() : "";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Ptr<RenderContext> Application::GetRenderContextOverride() const
{
    if (mArguments.HasOption("render"))
    {
        return CreateRenderContext(mArguments.FindOption("render", ""));
    }
    else
    {
        return FindBestRenderContext();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool Application::GetPPAAOverride() const
{
    return !mArguments.HasOption("samples") && atoi(mArguments.FindOption("ppaa", "1")) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t Application::GetSamplesOverride() const
{
    return atoi(mArguments.FindOption("samples", "1"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool Application::GetVSyncOverride() const
{
    return atoi(mArguments.FindOption("vsync", "1")) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool Application::GetsRGBOverride() const
{
    return mArguments.HasOption("linear");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Application::EnsureResources() const
{
    if (mResources == 0)
    {
        mResources = *new ResourceDictionary();
        ConnectNode(mResources, this);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Application::OnActivated(Display*)
{
    mActivated(this, EventArgs::Empty);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Application::OnDeactivated(Display*)
{
    mDeactivated(this, EventArgs::Empty);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_IMPLEMENT_REFLECTION(Application)
{
    NsMeta<TypeId>("Application");

    NsImpl<IUITreeNode>();

    NsProp("Resources", &Application::GetResources, &Application::SetResources);
    NsProp("MainWindow", &Application::GetMainWindow);
    NsProp("StartupUri", &Application::mStartupUri)
        .Meta<TypeConverterMetaData>("Converter<XamlUri>");

    NsEvent("Activated", &Application::mActivated);
    NsEvent("Deactivated", &Application::mDeactivated);
    NsEvent("StartUp", &Application::mStartUp);
    NsEvent("Exit", &Application::mExit);
}
