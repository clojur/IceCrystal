////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "MainWindow.xaml.h"

#include <NsCore/Version.h>
#include <NsGui/Keyboard.h>
#include <NsGui/FreezableCollection.h>
#include <NsGui/Transform.h>
#include <NsGui/TransformGroup.h>
#include <NsGui/ScaleTransform.h>
#include <NsGui/TranslateTransform.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/CommandBinding.h>
#include <NsGui/Collection.h>
#include <NsGui/TextBlock.h>
#include <NsGui/ItemsControl.h>
#include <NsApp/Application.h>


using namespace Noesis;
using namespace NoesisApp;
using namespace XamlPlayer;


////////////////////////////////////////////////////////////////////////////////////////////////////
XamlPlayer::MainWindow::MainWindow(): mZoom(1.0f), mDragging(false)
{
    String::Copy(mActiveFilename, sizeof(mActiveFilename), "");
    Initialized() += MakeDelegate(this, &MainWindow::OnInitialized);
    Loaded() += MakeDelegate(this, &MainWindow::OnLoaded);
    InitializeComponent();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::LoadXAML(const char* filename)
{
    mErrorList->Clear();

    Ptr<FrameworkElement> content = GUI::LoadXaml<FrameworkElement>(filename);
    mContainer->SetContent(content);

    mHelpText->SetVisibility(Visibility_Collapsed);
    UpdateTitle(filename);
    String::Copy(mActiveFilename, sizeof(mActiveFilename), filename);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::OnPreviewMouseRightButtonDown(const MouseButtonEventArgs& e)
{
    ParentClass::OnPreviewMouseRightButtonDown(e);

    if (IsFileLoaded() && (GetKeyboard()->GetModifiers() & ModifierKeys_Control) != 0)
    {
        CaptureMouse();

        mDragging = true;
        mDraggingLastPosition = e.position;
        e.handled = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::OnPreviewMouseRightButtonUp(const MouseButtonEventArgs& e)
{
    ParentClass::OnPreviewMouseRightButtonUp(e);

    if (mDragging)
    {
        ReleaseMouseCapture();

        mDragging = false;
        e.handled = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::OnPreviewMouseMove(const MouseEventArgs& e)
{
    ParentClass::OnPreviewMouseMove(e);

    if (mDragging)
    {
        float x = mContainerTranslate->GetX();
        float y = mContainerTranslate->GetY();

        Point delta = e.position - mDraggingLastPosition;
        mDraggingLastPosition = e.position;

        mContainerTranslate->SetX(x + delta.x);
        mContainerTranslate->SetY(y + delta.y);

        e.handled = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::OnPreviewMouseWheel(const MouseWheelEventArgs& e)
{
    ParentClass::OnPreviewMouseWheel(e);

    if (IsFileLoaded() && (GetKeyboard()->GetModifiers() & ModifierKeys_Control) != 0)
    {
        mZoom = Clip(mZoom * (e.wheelRotation > 0 ? 1.05f : 0.952381f), 0.01f, 100.0f);

        float width = mContainer->GetActualWidth();
        float height = mContainer->GetActualHeight();

        Point center(width * 0.5f, height * 0.5f);
        Point point = mContainer->PointFromScreen(e.position);
        Point pointScaled = center + (point - center) * mZoom;
        Point offset = e.position - pointScaled;

        mContainerScale->SetScaleX(mZoom);
        mContainerScale->SetScaleY(mZoom);

        mContainerTranslate->SetX(offset.x);
        mContainerTranslate->SetY(offset.y);

        e.handled = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::OnFileDropped(const char* filename)
{
    if (String::EndsWith(filename, ".xaml", IgnoreCase_True))
    {
        LoadXAML(filename);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::ErrorHandler(const char* filename, uint32_t line, const char* desc,
    bool fatal)
{
    MainWindow* window = (MainWindow*)Application::Current()->GetMainWindow();
    window->mErrorList->Add(desc);
    window->mDefaultErrorHandler(filename, line, desc, fatal);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::InitializeComponent()
{
    GUI::LoadComponent(this, "XamlPlayer/MainWindow.xaml");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::UpdateTitle(const char* filename)
{
    char title[PATH_MAX] = "XamlPlayer ";

    if (!String::StartsWith(GetBuildVersion(), "0.0.0"))
    {
        String::Append(title, sizeof(title), "v");
        String::Append(title, sizeof(title), GetBuildVersion());
    }

    if (filename != nullptr)
    {
        String::Append(title, sizeof(title), " - ");
        String::Append(title, sizeof(title), filename);
    }

    SetTitle(title);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool XamlPlayer::MainWindow::IsFileLoaded() const
{
    return !String::IsNullOrEmpty(mActiveFilename);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::OnInitialized(BaseComponent*, const EventArgs&)
{
    UpdateTitle(nullptr);

    // Error list
    ItemsControl* errors = FindName<ItemsControl>("Errors");
    NS_ASSERT(errors != 0);
    mErrorList = *new Collection();
    errors->SetItemsSource(mErrorList);
    mDefaultErrorHandler = SetErrorHandler(ErrorHandler);

    CommandBindingCollection* commandBindings = GetCommandBindings();
    NS_ASSERT(commandBindings->Count() == 2);

    // Reset command
    CommandBinding* resetCmd = NsStaticCast<CommandBinding*>(commandBindings->Get(0));
    resetCmd->CanExecute() += MakeDelegate(this, &MainWindow::OnCanExecuteReset);
    resetCmd->Executed() += MakeDelegate(this, &MainWindow::OnExecuteReset);

    CommandBinding* updateCmd = NsStaticCast<CommandBinding*>(commandBindings->Get(1));
    updateCmd->CanExecute() += MakeDelegate(this, &MainWindow::OnCanExecuteUpdate);
    updateCmd->Executed() += MakeDelegate(this, &MainWindow::OnExecuteUpdate);

    mHelpText = FindName<TextBlock>("DragText");
    NS_ASSERT(mHelpText != 0);

#if defined(NS_PLATFORM_WINDOWS_DESKTOP) || defined(NS_PLATFORM_OSX) || defined(NS_PLATFORM_LINUX)
    mHelpText->SetText("Drag & Drop XAML files here\nCTRL + T for Settings");
#elif defined(NS_PLATFORM_PS4)
    mHelpText->SetText("Waiting for connection...\n'Options' for Settings");
#elif defined(NS_PLATFORM_XBOX_ONE)
    mHelpText->SetText("Waiting for connection...\n'Menu' for Settings");
#else
    mHelpText->SetText("Waiting for connection...\nSwipe from Left border for Settings");
#endif

    mContainer = FindName<ContentControl>("Container");
    NS_ASSERT(mContainer != 0);

    TransformGroup* group = NsStaticCast<TransformGroup*>(mContainer->GetRenderTransform());
    TransformCollection* children = group->GetChildren();
    NS_ASSERT(children->Count() == 2);

    mContainerScale = NsStaticCast<ScaleTransform*>(children->Get(0));
    NS_ASSERT(mContainerScale != 0);

    mContainerTranslate = NsStaticCast<TranslateTransform*>(children->Get(1));
    NS_ASSERT(mContainerTranslate != 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::OnLoaded(BaseComponent*, const Noesis::RoutedEventArgs&)
{
    PreviewToolbar();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::OnCanExecuteReset(BaseComponent*, const CanExecuteRoutedEventArgs& e)
{
    e.canExecute = true;
    e.handled = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::OnExecuteReset(BaseComponent*, const ExecutedRoutedEventArgs& e)
{
    mZoom = 1.0f;

    mContainerScale->SetScaleX(1.0f);
    mContainerScale->SetScaleY(1.0f);
    mContainerTranslate->SetX(0.0f);
    mContainerTranslate->SetY(0.0f);

    e.handled = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::OnCanExecuteUpdate(BaseComponent*, const CanExecuteRoutedEventArgs& e)
{
    e.canExecute = IsFileLoaded();
    e.handled = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XamlPlayer::MainWindow::OnExecuteUpdate(BaseComponent*, const ExecutedRoutedEventArgs&)
{
    LoadXAML(mActiveFilename);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_IMPLEMENT_REFLECTION(XamlPlayer::MainWindow)
{
    NsMeta<TypeId>("XamlPlayer.MainWindow");
}
