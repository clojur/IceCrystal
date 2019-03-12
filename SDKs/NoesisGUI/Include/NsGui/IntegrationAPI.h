
////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INTEGRATIONAPI_H__
#define __GUI_INTEGRATIONAPI_H__


#include <NsCore/Noesis.h>
#include <NsCore/Log.h>
#include <NsCore/Error.h>
#include <NsCore/Ptr.h>
#include <NsCore/DynamicCast.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Enums.h>


namespace Noesis
{

NS_INTERFACE IView;
class BaseComponent;
class MemoryAllocator;
class UIElement;
class FrameworkElement;
class ResourceDictionary;
class XamlProvider;
class TextureProvider;
class FontProvider;

namespace GUI
{

/// Initialization passing error handler and optional logging handler and memory allocator
/// This must be the first Noesis function invoked and Shutdown the last one
/// For now, only a pair of Init() and Shutdown() is supported per application execution
NS_GUI_CORE_API void Init(ErrorHandler errorHandler, LogHandler logHandler = 0,
    MemoryAllocator* allocator = 0);

/// Sets the provider in charge of loading XAML resources
NS_GUI_CORE_API void SetXamlProvider(XamlProvider* provider);

/// Sets the provider in charge of loading texture resources
NS_GUI_CORE_API void SetTextureProvider(TextureProvider* provider);

/// Sets the provider in charge of loading font resources
NS_GUI_CORE_API void SetFontProvider(FontProvider* provider);

/// Sets a collection of application-scope resources, such as styles and brushes. Provides a
/// simple way to support a consistent theme across your application
NS_GUI_CORE_API void SetApplicationResources(ResourceDictionary* resources);

/// Sets the callbacks used for showing and hiding the on-screen keyboard. Show keyboard callback
/// may return true if caret should be hidden while on-screen keyboard is visible
typedef bool (*ShowSoftwareKeyboardCallback)(void* user, UIElement* focusedElement);
typedef void (*HideSoftwareKeyboardCallback)(void* user);
NS_GUI_CORE_API void SetSoftwareKeyboardCallbacks(void* user, ShowSoftwareKeyboardCallback
    showCallback, HideSoftwareKeyboardCallback hideCallback);

/// Sets the callback used for updating the mouse cursor icon
typedef void (*UpdateCursorCallback)(void* user, Cursor cursor);
NS_GUI_CORE_API void SetCursorCallback(void* user, UpdateCursorCallback callback);

/// Loads a XAML file that is located at the specified uniform resource identifier
NS_GUI_CORE_API Ptr<BaseComponent> LoadXaml(const char* filename);
template<class T> Ptr<T> LoadXaml(const char* filename);

/// Loads a XAML file passing an object of the same type as the root element
NS_GUI_CORE_API void LoadComponent(BaseComponent* component, const char* filename);

/// Creates a view with the given root element
NS_GUI_CORE_API Ptr<IView> CreateView(FrameworkElement* content);

/// Frees allocated resources and shutdowns. Release all noesis objects before calling this
NS_GUI_CORE_API void Shutdown();

}

}

#include <NsGui/IntegrationAPI.inl>

#endif
