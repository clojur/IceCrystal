####################################################################################################
# PROJECT: NoesisApp
####################################################################################################

LOCAL_PATH := $(call my-dir)

# NoesisApp: Render/RenderContext Render/GLRenderDevice App/Display Render/GLRenderContext App/AndroidDisplay App/Launcher App/DisplayLauncher App/ApplicationLauncher 
include $(CLEAR_VARS)
LOCAL_MODULE := NoesisApp
LOCAL_CFLAGS := -DNS_BUILD -DNS_CONFIG=Debug -DNS_VERSION=2.1.0f1 -DNS_APP_FRAMEWORK -DNS_DEBUG_CFG -DNS_ASSERTS_ENABLED=1 -DNS_INSTRUMENTED_ENABLED=1 -DNS_MINIMUM_LOG_LEVEL=1 -DNS_RENDER_RENDERCONTEXT_EXPORTS -DNS_RENDER_GLRENDERDEVICE_EXPORTS -DNS_APP_DISPLAY_EXPORTS -DNS_RENDER_GLRENDERCONTEXT_PRIVATE -DNS_APP_ANDROIDDISPLAY_PRIVATE -DNS_APP_LAUNCHER_EXPORTS -DNS_APP_DISPLAYLAUNCHER_EXPORTS -DNS_APP_APPLICATIONLAUNCHER_EXPORTS
LOCAL_SRC_FILES := init.cpp ../../../Render/RenderContext/Src/Image.cpp ../../../Render/RenderContext/Src/Render.RenderContext.cpp ../../../Render/RenderContext/Src/RenderContext.cpp ../../../Render/GLRenderDevice/Src/GLRenderDevice.cpp ../../../Render/GLRenderDevice/Src/Render.GLRenderDevice.cpp ../../../App/Display/Src/App.Display.cpp ../../../App/Display/Src/Display.cpp ../../../Render/GLRenderContext/Src/GLRenderContext.cpp ../../../Render/GLRenderContext/Src/Render.GLRenderContext.cpp ../../../App/AndroidDisplay/Src/AndroidDisplay.cpp ../../../App/AndroidDisplay/Src/Gui.AndroidDisplay.cpp ../../../App/Launcher/Src/App.Launcher.cpp ../../../App/Launcher/Src/CommandLine.cpp ../../../App/Launcher/Src/EmbeddedFontProvider.cpp ../../../App/Launcher/Src/EmbeddedTextureProvider.cpp ../../../App/Launcher/Src/EmbeddedXamlProvider.cpp ../../../App/Launcher/Src/FileTextureProvider.cpp ../../../App/Launcher/Src/Launcher.cpp ../../../App/Launcher/Src/LocalFontProvider.cpp ../../../App/Launcher/Src/LocalTextureProvider.cpp ../../../App/Launcher/Src/LocalXamlProvider.cpp ../../../App/DisplayLauncher/Src/App.DisplayLauncher.cpp ../../../App/DisplayLauncher/Src/DisplayLauncher.cpp ../../../App/ApplicationLauncher/Src/App.ApplicationLauncher.cpp ../../../App/ApplicationLauncher/Src/Application.cpp ../../../App/ApplicationLauncher/Src/ApplicationLauncher.cpp ../../../App/ApplicationLauncher/Src/Window.cpp 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../../Include $(LOCAL_PATH)/../../../Render/RenderContext/Include $(LOCAL_PATH)/../../../Render/GLRenderDevice/Include $(LOCAL_PATH)/../../../App/Display/Include $(LOCAL_PATH)/../../../Render/GLRenderContext/Include $(LOCAL_PATH)/../../../App/AndroidDisplay/Include $(LOCAL_PATH)/../../../App/Launcher/Include $(LOCAL_PATH)/../../../Gui/Providers/Include $(LOCAL_PATH)/../../../App/DisplayLauncher/Include $(LOCAL_PATH)/../../../App/ApplicationLauncher/Include $(LOCAL_PATH)/../../../../../../../../NoesisSDK/3rdParty/EASTL/local/include 
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../../Include $(LOCAL_PATH)/../../../Render/RenderContext/Include $(LOCAL_PATH)/../../../Render/GLRenderDevice/Include $(LOCAL_PATH)/../../../Render/RenderDevice/Include $(LOCAL_PATH)/../../../App/Display/Include $(LOCAL_PATH)/../../../Gui/Core/Include $(LOCAL_PATH)/../../../Render/GLRenderContext/Include $(LOCAL_PATH)/../../../App/AndroidDisplay/Include $(LOCAL_PATH)/../../../App/Launcher/Include $(LOCAL_PATH)/../../../Gui/Providers/Include $(LOCAL_PATH)/../../../Gui/CachedFontProvider/Include $(LOCAL_PATH)/../../../App/DisplayLauncher/Include $(LOCAL_PATH)/../../../App/ApplicationLauncher/Include $(LOCAL_PATH)/../../../../../../../../NoesisSDK/3rdParty/EASTL/local/include 
LOCAL_STATIC_LIBRARIES := android_native_app_glue 
LOCAL_SHARED_LIBRARIES := Noesis 
LOCAL_LDLIBS := -lGLESv2 -lEGL -landroid -llog 
LOCAL_SHORT_COMMANDS := true
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
